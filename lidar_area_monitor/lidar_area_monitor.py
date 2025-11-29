#!/usr/bin/env python3
# pyright: reportMissingImports=false
import math
import struct
import json
import importlib.util
import numpy as np

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy, HistoryPolicy

from std_msgs.msg import Header, String
from sensor_msgs.msg import LaserScan, PointCloud2, PointField
from visualization_msgs.msg import Marker, MarkerArray
from rcl_interfaces.msg import SetParametersResult
from builtin_interfaces.msg import Duration

from shapely.geometry import Polygon, Point, box as shapely_box
from shapely.prepared import prep

from ros2_lam_interfaces.srv import SetExclusionArea
from std_srvs.srv import Trigger

# ===================== Aggiunte per UWB/seriale & calibrazione =====================
import threading, queue, time, re
from collections import deque
try:
    import serial  # fornito da python3-serial (APT)
except ImportError:
    serial = None
# ================================================================================

### NEW: TF2 per pubblicare il frame dell'antenna (static transform)
from tf2_ros import StaticTransformBroadcaster
from geometry_msgs.msg import TransformStamped


class LidarAreaMonitor(Node):
    def __init__(self, node_name="lidar_area_monitor_node"):
        super().__init__(node_name)

        # === Sottoscrizioni/pubblicazioni ===
        self.lidar_sub = self.create_subscription(LaserScan, "/scan", self.scan_callback, 10)
        self.points_pub = self.create_publisher(PointCloud2, "/points_inside", 10)

        # Publisher "latched" per i Marker (RViz li vede anche se parte dopo)
        qos = QoSProfile(
            depth=1,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
        )
        self.marker_pub = self.create_publisher(MarkerArray, "/visualization_marker_array", qos)

        # --- PUB JSON per web bridge (PRIMA di disegnare le aree!) ---
        qos_lat = QoSProfile(
            depth=1,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
        )
        # /lam/areas: ultimo stato aree (latched)
        self.pub_areas = self.create_publisher(String, "/lam/areas", qos_lat)
        # /lam/tracks: stream dei cluster tracciati
        self.pub_tracks = self.create_publisher(String, "/lam/tracks", 10)
        self.pub_assocs = self.create_publisher(String, "/lam/assocs", 10)

        # ===================== PUB aggiuntivo: UWB tag stream =====================
        # /lam/uwb_tags: stream dei tag UWB (JSON)
        self.pub_uwb = self.create_publisher(String, "/lam/uwb_tags", 10)
        # ========================================================================

        # === Servizi aree ===
        self.area_srv = self.create_service(SetExclusionArea, "set_exclusion_area", self.set_exclusion_area)
        self.add_area_srv = self.create_service(SetExclusionArea, "add_exclusion_area", self.add_exclusion_area)
        self.clear_areas_srv = self.create_service(Trigger, "clear_exclusion_areas", self.clear_exclusion_areas)

        # ===================== Servizi calibrazione UWB (assistita) =====================
        self.uwb_calib_reset_srv = self.create_service(Trigger, "uwb_calib_reset", self.uwb_calib_reset)
        self.uwb_calib_capture_srv = self.create_service(Trigger, "uwb_calib_capture", self.uwb_calib_capture)
        self.uwb_calib_finish_srv = self.create_service(Trigger, "uwb_calib_finish", self.uwb_calib_finish)
        # ==============================================================================

        # === Aree: id -> {'poly','prep','bounds','is_axis_rect'} ===
        self.areas = {}
        self.next_area_id = 1
        # Area di default (rettangolo allineato)
        default_coords = [(0.5, 0.5), (0.5, -0.5), (0.1, -0.5), (0.1, 0.5)]
        self._set_single_area(Polygon(default_coords))

        # === Parametri RViz/marker (modificabili a runtime) ===
        self.declare_parameter("marker_frame", "laser")
        self.declare_parameter("line_thickness", 0.02)  # m
        self.declare_parameter("line_color", [1.0, 0.35, 0.0, 1.0])  # RGBA contorno area
        self.declare_parameter("sphere_size", 0.0)  # m (diametro; 0 => disabilitato)
        self.declare_parameter("sphere_color", [1.0, 0.35, 0.0, 1.0])  # RGBA vertici area

        # === Parametri clustering/tracking ===
        self.declare_parameter("cluster_eps", 0.15)
        self.declare_parameter("cluster_min_pts", 3)
        self.declare_parameter("max_assoc_dist", 0.30)
        self.declare_parameter("track_miss_tolerance", 3)

        # === Parametri visualizzazione cluster ===
        self.declare_parameter("centroid_diam", 0.08)
        self.declare_parameter("centroid_color", [0.0, 0.8, 0.2, 1.0])
        self.declare_parameter("label_scale", 0.12)

        # === Parametri Bounding Box & Velocità ===
        self.declare_parameter("bbox_line_thickness", 0.02)
        self.declare_parameter("bbox_color", [0.1, 0.6, 1.0, 1.0])
        self.declare_parameter("vel_arrow_scale", 0.6)
        self.declare_parameter("vel_color", [1.0, 1.0, 0.2, 0.95])
        self.declare_parameter("vel_smooth", 0.6)

        # === Parametri performance (Raspberry) ===
        self.declare_parameter("scan_stride", 2)
        self.declare_parameter("max_points_inside", 800)
        self.declare_parameter("print_every_n", 1)

        # ===================== Parametri Antenna/UWB & Calibrazione =====================
        self.declare_parameter("antenna_port", "/dev/ttyACM0")
        self.declare_parameter("antenna_baud", 115200)
        self.declare_parameter("antenna_yaw_deg", 0.0)   # rotazione antenna->laser (gradi)
        self.declare_parameter("antenna_dx", 0.0)        # m (offset x)
        self.declare_parameter("antenna_dy", 0.0)        # m (offset y)
        self.declare_parameter("dist_a", 1.0)            # D_reale_cm = a * D_cm + b
        self.declare_parameter("dist_b", 0.0)
        self.declare_parameter("tag_ttl_sec", 0.6)
        self.declare_parameter("tag_assoc_dist", 1.00)   # m -> raggio per match TAG↔cluster 0.25
        self.declare_parameter("verified_color", [0.0, 1.0, 0.0, 1.0])
        self.declare_parameter("antenna_axis_len", 0.35) # m
        self.declare_parameter("lidar_axis_len", 0.35)   # m
        # Calibrazione assistita
        self.declare_parameter("calib_true_m", 1.0)      # distanza reale corrente (m)
        self.declare_parameter("calib_window", 50)       # quanti raw-D mediar per sample

        ### NEW: Parametri aggiuntivi
        self.declare_parameter("verify_mode", "both")  # "centroid_radius" | "bbox" | "both"
        self.declare_parameter("verify_bbox_margin", 0.02)  # m
        self.declare_parameter("vel_smooth_tag", 0.6)  # filtro esponenziale sui tag
        self.declare_parameter("lidar_axis_color", [1.0, 0.7, 0.1, 0.95])
        self.declare_parameter("antenna_axis_color", [0.3, 1.0, 0.3, 0.95])
        self.declare_parameter("antenna_body_color", [0.2, 0.8, 1.0, 0.9])
        self.declare_parameter("tag_color", [0.2, 1.0, 0.8, 0.9])
        self.declare_parameter("tag_label_scale", 0.12)
        # === NEW: usa sorgente UWB esterna (bridge) invece della seriale interna ===
        self.declare_parameter("use_external_uwb", True)          # True => subscribe a uwb_topic
        self.declare_parameter("uwb_topic", "/lam/uwb_tags")      # topic del bridge UWB

        # --- Carica parametri ---
        self._reload_style_params()
        self._reload_cluster_params()
        self._reload_bbox_vel_params()
        self._reload_perf_params()
        self._reload_antenna_params()
        self.add_on_set_parameters_callback(self._on_params)
        self.use_external_uwb = bool(self.get_parameter("use_external_uwb").value)
        self.uwb_topic = str(self.get_parameter("uwb_topic").value)

        ### NEW: Broadcaster TF statico (laser->antenna)
        self._tf_static = StaticTransformBroadcaster(self)
        self._publish_antenna_tf()

        # === Stato tracking ===
        self.next_track_id = 1
        self.tracks = {}  # tid -> {...}
        self._prev_cluster_marker_ids = set()
        self._frame_count = 0

        # tracking dei marker area per cancellare quelli rimossi
        self._prev_area_ns_ids = set()  # {(ns, id), ...}
        # tracking marker TAG (tupla ns,id)
        self._prev_tag_marker_ids = set()  # <<< IMPORTANT: inizializzato PRIMA di timer/callback

        # === DBSCAN backend ===
        self._has_sklearn = importlib.util.find_spec("sklearn.cluster") is not None
        if not self._has_sklearn:
            self.get_logger().warn("scikit-learn non trovato: uso DBSCAN a griglia (veloce su ARM).")

        # ===================== Stato UWB/antenna =====================
        self._uwb_queue = queue.Queue(maxsize=200)
        self._last_tags = {}             # tag_id -> {'x':m,'y':m,'d':m,'stamp':sec}
        self._antenna_marker_ids = set() # tracking marker antenna
        self._raw_d_buffer = deque(maxlen=2000)   # valori raw D cm
        self._calib_pairs = []           # (meas_cm, true_cm)
        self._tag_smooth = {}            # tag_id -> (sx, sy)

        # === NEW: sorgente UWB: esterna (topic) o seriale interna ===
        if self.use_external_uwb:
            self.get_logger().info(f"UWB: uso sorgente esterna su topic {self.uwb_topic}")
            self.uwb_sub = self.create_subscription(String, self.uwb_topic, self._uwb_tags_callback, 10)
        else:
            if serial is None:
                self.get_logger().warn("pyserial non disponibile. Installa 'python3-serial'.")
            else:
                self._antenna_thread = threading.Thread(
                    target=self._antenna_reader,
                    args=(self.antenna_port, self.antenna_baud),
                    daemon=True
                )
                self._antenna_thread.start()
                self.get_logger().info(f"Antenna serial reader (auto-reconnect) su {self.antenna_port}@{self.antenna_baud}")

        # --- Timers ---
        self.create_timer(0.02, self._process_uwb)  # UWB processing
        self.create_timer(1.0, self._areas_keepalive)
        self.create_timer(0.2, self._tag_keepalive)

        # Pubblica subito i marker delle aree (e lo stato JSON)
        self.create_area_markers()

    # -------------------- Parametri --------------------
    def _reload_style_params(self):
        self.marker_frame = str(self.get_parameter("marker_frame").value)
        self.line_thickness = float(self.get_parameter("line_thickness").value)
        self.line_color = [float(x) for x in self.get_parameter("line_color").value]
        self.sphere_size = float(self.get_parameter("sphere_size").value)
        self.sphere_color = [float(x) for x in self.get_parameter("sphere_color").value]

    def _reload_cluster_params(self):
        self.cluster_eps = float(self.get_parameter("cluster_eps").value)
        self.cluster_min_pts = int(self.get_parameter("cluster_min_pts").value)
        self.max_assoc_dist = float(self.get_parameter("max_assoc_dist").value)
        self.track_miss_tolerance = int(self.get_parameter("track_miss_tolerance").value)

        self.centroid_diam = float(self.get_parameter("centroid_diam").value)
        self.centroid_color = [float(x) for x in self.get_parameter("centroid_color").value]
        self.label_scale = float(self.get_parameter("label_scale").value)

    def _reload_bbox_vel_params(self):
        self.bbox_line_thickness = float(self.get_parameter("bbox_line_thickness").value)
        self.bbox_color = [float(x) for x in self.get_parameter("bbox_color").value]
        self.vel_arrow_scale = float(self.get_parameter("vel_arrow_scale").value)
        self.vel_color = [float(x) for x in self.get_parameter("vel_color").value]
        self.vel_smooth = float(self.get_parameter("vel_smooth").value)

    def _reload_perf_params(self):
        self.scan_stride = max(1, int(self.get_parameter("scan_stride").value))
        self.max_points_inside = int(self.get_parameter("max_points_inside").value)
        self.print_every_n = max(1, int(self.get_parameter("print_every_n").value))

    # ===================== Parametri antenna/UWB & calibrazione =====================
    def _reload_antenna_params(self):
        self.antenna_port = str(self.get_parameter("antenna_port").value)
        self.antenna_baud = int(self.get_parameter("antenna_baud").value)
        self.antenna_yaw_deg = float(self.get_parameter("antenna_yaw_deg").value)
        self.antenna_dx = float(self.get_parameter("antenna_dx").value)
        self.antenna_dy = float(self.get_parameter("antenna_dy").value)
        self.dist_a = float(self.get_parameter("dist_a").value)
        self.dist_b = float(self.get_parameter("dist_b").value)
        self.tag_ttl_sec = float(self.get_parameter("tag_ttl_sec").value)
        self.tag_assoc_dist = float(self.get_parameter("tag_assoc_dist").value)
        self.verified_color = [float(x) for x in self.get_parameter("verified_color").value]
        self.antenna_axis_len = float(self.get_parameter("antenna_axis_len").value)
        self.lidar_axis_len = float(self.get_parameter("lidar_axis_len").value)
        self.calib_true_m = float(self.get_parameter("calib_true_m").value)
        self.calib_window = max(1, int(self.get_parameter("calib_window").value))
        # extra
        self.verify_mode = str(self.get_parameter("verify_mode").value)
        self.verify_bbox_margin = float(self.get_parameter("verify_bbox_margin").value)
        self.vel_smooth_tag = float(self.get_parameter("vel_smooth_tag").value)
        self.lidar_axis_color = [float(x) for x in self.get_parameter("lidar_axis_color").value]
        self.antenna_axis_color = [float(x) for x in self.get_parameter("antenna_axis_color").value]
        self.antenna_body_color = [float(x) for x in self.get_parameter("antenna_body_color").value]
        self.tag_color = [float(x) for x in self.get_parameter("tag_color").value]
        self.tag_label_scale = float(self.get_parameter("tag_label_scale").value)

    def _on_params(self, params):
        changed_areas = False
        changed_axes = False
        for p in params:
            if p.name == "marker_frame":
                self.marker_frame = str(p.value); changed_areas = True
            elif p.name == "line_thickness":
                self.line_thickness = float(p.value); changed_areas = True
            elif p.name == "line_color":
                self.line_color = [float(x) for x in p.value]; changed_areas = True
            elif p.name == "sphere_size":
                self.sphere_size = float(p.value); changed_areas = True
            elif p.name == "sphere_color":
                self.sphere_color = [float(x) for x in p.value]; changed_areas = True
            elif p.name == "cluster_eps":
                self.cluster_eps = float(p.value)
            elif p.name == "cluster_min_pts":
                self.cluster_min_pts = int(p.value)
            elif p.name == "max_assoc_dist":
                self.max_assoc_dist = float(p.value)
            elif p.name == "track_miss_tolerance":
                self.track_miss_tolerance = int(p.value)
            elif p.name == "centroid_diam":
                self.centroid_diam = float(p.value)
            elif p.name == "centroid_color":
                self.centroid_color = [float(x) for x in p.value]
            elif p.name == "label_scale":
                self.label_scale = float(p.value)
            elif p.name == "bbox_line_thickness":
                self.bbox_line_thickness = float(p.value); changed_areas = True
            elif p.name == "bbox_color":
                self.bbox_color = [float(x) for x in p.value]; changed_areas = True
            elif p.name == "vel_arrow_scale":
                self.vel_arrow_scale = float(p.value)
            elif p.name == "vel_color":
                self.vel_color = [float(x) for x in p.value]
            elif p.name == "vel_smooth":
                self.vel_smooth = float(p.value)
            elif p.name == "scan_stride":
                self.scan_stride = max(1, int(p.value))
            elif p.name == "max_points_inside":
                self.max_points_inside = int(p.value)
            elif p.name == "print_every_n":
                self.print_every_n = max(1, int(p.value))
            # --------- nuovi parametri UWB/calibrazione ----------
            elif p.name == "antenna_port":
                self.antenna_port = str(p.value)
            elif p.name == "antenna_baud":
                self.antenna_baud = int(p.value)
            elif p.name == "antenna_yaw_deg":
                self.antenna_yaw_deg = float(p.value); changed_axes = True
            elif p.name == "antenna_dx":
                self.antenna_dx = float(p.value); changed_axes = True
            elif p.name == "antenna_dy":
                self.antenna_dy = float(p.value); changed_axes = True
            elif p.name == "dist_a":
                self.dist_a = float(p.value)
            elif p.name == "dist_b":
                self.dist_b = float(p.value)
            elif p.name == "tag_ttl_sec":
                self.tag_ttl_sec = float(p.value)
            elif p.name == "tag_assoc_dist":
                self.tag_assoc_dist = float(p.value)
            elif p.name == "verified_color":
                self.verified_color = [float(x) for x in p.value]
            elif p.name == "antenna_axis_len":
                self.antenna_axis_len = float(p.value)
            elif p.name == "lidar_axis_len":
                self.lidar_axis_len = float(p.value)
            elif p.name == "calib_true_m":
                self.calib_true_m = float(p.value)
            elif p.name == "calib_window":
                self.calib_window = max(1, int(p.value))
            # --------- NEW: parametri extra ----------
            elif p.name == "verify_mode":
                self.verify_mode = str(p.value)
            elif p.name == "verify_bbox_margin":
                self.verify_bbox_margin = float(p.value)
            elif p.name == "vel_smooth_tag":
                self.vel_smooth_tag = float(p.value)
            elif p.name == "lidar_axis_color":
                self.lidar_axis_color = [float(x) for x in p.value]; changed_axes = True
            elif p.name == "antenna_axis_color":
                self.antenna_axis_color = [float(x) for x in p.value]; changed_axes = True
            elif p.name == "antenna_body_color":
                self.antenna_body_color = [float(x) for x in p.value]; changed_axes = True
            elif p.name == "tag_color":
                self.tag_color = [float(x) for x in p.value]
            elif p.name == "tag_label_scale":
                self.tag_label_scale = float(p.value)
            # --------- NEW: sorgente UWB esterna ----------
            elif p.name == "use_external_uwb":
                self.use_external_uwb = bool(p.value)
                self.get_logger().warn("Modifica 'use_external_uwb' applicata ma richiede riavvio per cambiare sorgente.")
            elif p.name == "uwb_topic":
                self.uwb_topic = str(p.value)
                self.get_logger().warn("Modifica 'uwb_topic' applicata; riavvia per ri-sottoscrivere il topic.")

        if changed_areas:
            self.create_area_markers()
        if changed_axes:
            self._publish_antenna_tf()
            self._publish_antenna_markers()
        return SetParametersResult(successful=True)

    # -------------------- Gestione aree --------------------
    def _prepare_area_cache(self, poly: Polygon):
        bounds = poly.bounds  # (minx, miny, maxx, maxy)
        return {
            "poly": poly,
            "prep": prep(poly),
            "bounds": bounds,
            "is_axis_rect": poly.equals(shapely_box(*bounds))
        }

    def _set_single_area(self, poly: Polygon):
        self.areas.clear()
        self.next_area_id = 1
        self.areas[self.next_area_id] = self._prepare_area_cache(poly)
        self.next_area_id += 1

    def set_exclusion_area(self, request, response):
        """Sostituisce tutte le aree con una sola (retro-compatibile)."""
        try:
            coords = [(p.x, p.y) for p in request.point_list]
            if len(coords) < 3:
                raise ValueError("Servono almeno 3 punti per definire un poligono.")
            poly = Polygon(coords)
            if (poly.is_empty or not poly.is_valid or poly.area == 0.0):
                poly = poly.buffer(0)
            if (poly.is_empty or not poly.is_valid or poly.area == 0.0):
                raise ValueError("Poligono non valido (self-intersection o area nulla).")
            self._set_single_area(poly)
            self.create_area_markers()
            self.get_logger().info(f"Sostituita area unica: bounds={self.areas[1]['bounds']}")
            response.is_ok = True; response.error_msg = ""
        except Exception as ex:
            self.get_logger().error(f"set_exclusion_area: {ex}")
            response.is_ok = False; response.error_msg = str(ex)
        return response

    def add_exclusion_area(self, request, response):
        """Aggiunge una nuova area."""
        try:
            coords = [(p.x, p.y) for p in request.point_list]
            if len(coords) < 3:
                raise ValueError("Servono almeno 3 punti per definire un poligono.")
            poly = Polygon(coords)
            if (poly.is_empty or not poly.is_valid or poly.area == 0.0):
                poly = poly.buffer(0)
            if (poly.is_empty or not poly.is_valid or poly.area == 0.0):
                raise ValueError("Poligono non valido (self-intersection o area nulla).")
            aid = self.next_area_id
            self.areas[aid] = self._prepare_area_cache(poly)
            self.next_area_id += 1
            self.create_area_markers()
            self.get_logger().info(f"Aggiunta area id={aid} bounds={self.areas[aid]['bounds']}")
            response.is_ok = True; response.error_msg = ""
        except Exception as ex:
            self.get_logger().error(f"add_exclusion_area: {ex}")
            response.is_ok = False; response.error_msg = str(ex)
        return response

    def clear_exclusion_areas(self, request, response):
        """Cancella tutte le aree."""
        self.areas.clear()
        self.next_area_id = 1
        # cancella marker delle aree attuali
        self._delete_all_area_markers()
        self._publish_areas_json()
        response.success = True
        response.message = "Tutte le aree rimosse."
        return response

    # -------------------- Marker aree --------------------
    def _delete_all_area_markers(self):
        arr = MarkerArray()
        now = self.get_clock().now().to_msg()
        for ns, mid in list(self._prev_area_ns_ids):
            dm = Marker()
            dm.header.frame_id = self.marker_frame
            dm.header.stamp = now
            dm.ns = ns
            dm.id = mid
            dm.action = Marker.DELETE
            arr.markers.append(dm)
        self._prev_area_ns_ids.clear()
        if arr.markers:
            self.marker_pub.publish(arr)

    def create_area_markers(self):
        """Disegna tutte le aree: contorno + (fill axis-aligned) + (vertici opzionali)."""
        from geometry_msgs.msg import Point as GeoPoint
        from std_msgs.msg import ColorRGBA

        arr = MarkerArray()
        now = self.get_clock().now().to_msg()
        current = set()

        for aid, data in self.areas.items():
            poly = data["poly"]
            bounds = data["bounds"]
            coords = list(poly.exterior.coords)[:-1]
            closed = coords + [coords[0]]
            ns = f"exclusion_area_{aid}"

            # contorno
            line = Marker()
            line.header.frame_id = self.marker_frame
            line.header.stamp = now
            line.ns = ns
            line.id = 0
            line.type = Marker.LINE_STRIP
            line.action = Marker.ADD
            line.pose.orientation.w = 1.0
            line.scale.x = self.line_thickness
            line.color.r, line.color.g, line.color.b, line.color.a = self.line_color
            line.frame_locked = True
            for x, y in closed:
                line.points.append(GeoPoint(x=float(x), y=float(y), z=0.0))
            arr.markers.append(line); current.add((ns, 0))

            # vertici opzionali
            if self.sphere_size > 0.0:
                spheres = Marker()
                spheres.header = line.header
                spheres.ns = ns
                spheres.id = 1
                spheres.type = Marker.SPHERE_LIST
                spheres.action = Marker.ADD
                spheres.scale.x = self.sphere_size
                spheres.scale.y = self.sphere_size
                spheres.scale.z = self.sphere_size
                spheres.color.r, spheres.color.g, spheres.color.b, spheres.color.a = self.sphere_color
                for x, y in coords:
                    spheres.points.append(GeoPoint(x=float(x), y=float(y), z=0.0))
                    spheres.colors.append(ColorRGBA(
                        r=self.sphere_color[0], g=self.sphere_color[1],
                        b=self.sphere_color[2], a=self.sphere_color[3]
                    ))
                arr.markers.append(spheres); current.add((ns, 1))

            # fill: CUBE sottile (axis-aligned)
            minx, miny, maxx, maxy = bounds
            fill = Marker()
            fill.header = line.header
            fill.ns = ns
            fill.id = 2
            fill.type = Marker.CUBE
            fill.action = Marker.ADD
            fill.pose.position.x = (minx + maxx) / 2.0
            fill.pose.position.y = (miny + maxy) / 2.0
            fill.pose.position.z = -0.001
            fill.scale.x = maxx - minx
            fill.scale.y = maxy - miny
            fill.scale.z = 0.001
            fill.color.r, fill.color.g, fill.color.b = self.line_color[:3]
            fill.color.a = 0.18
            arr.markers.append(fill); current.add((ns, 2))

        # elimina marker di aree rimosse
        stale = self._prev_area_ns_ids - current
        for ns, mid in stale:
            dm = Marker()
            dm.header.frame_id = self.marker_frame
            dm.header.stamp = now
            dm.ns = ns
            dm.id = mid
            dm.action = Marker.DELETE
            arr.markers.append(dm)

        self._prev_area_ns_ids = current
        if arr.markers:
            self.marker_pub.publish(arr)
        self._publish_areas_json()  # JSON per frontend

    def _areas_keepalive(self):
        """Ripubblica i marker delle aree a cadenza fissa (RViz avviato dopo le vede)."""
        try:
            self.create_area_markers()
        except Exception as e:
            try:
                self.get_logger().warn(f"areas keepalive err: {e}")
            except Exception:
                pass

    def _tag_keepalive(self):
        """Ripubblica i marker dei TAG anche senza nuovi pacchetti (sempre visibili)."""
        try:
            self._publish_tag_markers()
        except Exception as e:
            try:
                self.get_logger().warn(f"tag keepalive err: {e}")
            except Exception:
                pass

    # -------------------- Utilità: PointCloud2 --------------------
    def create_point_cloud2_message(self, header: Header, points_xyz):
        n = len(points_xyz)
        if n == 0:
            return None
        arr = np.asarray(points_xyz, dtype=np.float32).reshape(-1, 3)
        cloud_data = arr.tobytes(order="C")
        pc2_msg = PointCloud2()
        pc2_msg.header = header
        pc2_msg.height = 1
        pc2_msg.width = n
        pc2_msg.is_dense = True
        pc2_msg.is_bigendian = False
        pc2_msg.fields = [
            PointField(name="x", offset=0, datatype=PointField.FLOAT32, count=1),
            PointField(name="y", offset=4, datatype=PointField.FLOAT32, count=1),
            PointField(name="z", offset=8, datatype=PointField.FLOAT32, count=1),
        ]
        pc2_msg.point_step = 12
        pc2_msg.row_step = pc2_msg.point_step * n
        pc2_msg.data = cloud_data
        return pc2_msg

    # -------------------- CLUSTERING --------------------
    def _run_dbscan(self, pts_xy: np.ndarray) -> np.ndarray:
        if pts_xy.shape[0] == 0:
            return np.empty((0,), dtype=np.int32)
        eps = float(self.cluster_eps)
        min_pts = int(self.cluster_min_pts)
        if self._has_sklearn:
            try:
                from sklearn.cluster import DBSCAN as SK_DBSCAN  # type: ignore
                db = SK_DBSCAN(eps=eps, min_samples=min_pts, metric="euclidean").fit(pts_xy)
                return db.labels_.astype(np.int32)
            except Exception:
                self._has_sklearn = False
        return self._dbscan_grid(pts_xy, eps, min_pts)

    def _dbscan_grid(self, pts: np.ndarray, eps: float, min_pts: int) -> np.ndarray:
        N = pts.shape[0]
        labels = np.full(N, -1, dtype=np.int32)
        visited = np.zeros(N, dtype=bool)
        cluster_id = 0
        eps2 = eps * eps
        cell = np.floor(pts / eps).astype(np.int32)
        grid = {}
        for i, (cx, cy) in enumerate(cell):
            grid.setdefault((int(cx), int(cy)), []).append(i)

        def neighbors(idx):
            cx, cy = cell[idx]
            cand = []
            for dx in (-1, 0, 1):
                for dy in (-1, 0, 1):
                    cand.extend(grid.get((int(cx + dx), int(cy + dy)), []))
            if not cand:
                return []
            C = np.array(cand, dtype=np.int32)
            d2 = np.sum((pts[C] - pts[idx]) ** 2, axis=1)
            return C[d2 <= eps2].tolist()

        for i in range(N):
            if visited[i]:
                continue
            visited[i] = True
            neigh = neighbors(i)
            if len(neigh) < min_pts:
                labels[i] = -1
                continue
            labels[i] = cluster_id
            seeds = [n for n in neigh if n != i]
            k = 0
            while k < len(seeds):
                j = seeds[k]
                if not visited[j]:
                    visited[j] = True
                    nj = neighbors(j)
                    if len(nj) >= min_pts:
                        for u in nj:
                            if u not in seeds:
                                seeds.append(u)
                if labels[j] == -1:
                    labels[j] = cluster_id
                k += 1
            cluster_id += 1
        return labels

    def _compute_centroids_and_bboxes(self, pts_xy: np.ndarray, labels: np.ndarray):
        if pts_xy.shape[0] == 0 or not np.any(labels >= 0):
            return [], []
        K = labels.max() + 1
        cents, bboxes = [], []
        for k in range(K):
            mask = (labels == k)
            cnt = np.count_nonzero(mask)
            if cnt >= self.cluster_min_pts:
                pts = pts_xy[mask]
                c = pts.mean(axis=0)
                mins = pts.min(axis=0)
                maxs = pts.max(axis=0)
                cents.append((float(c[0]), float(c[1])))
                bboxes.append((float(mins[0]), float(mins[1]), float(maxs[0]), float(maxs[1])))
        return cents, bboxes

    # -------------------- TRACKING --------------------
    @staticmethod
    def _stamp_to_sec(stamp) -> float:
        return float(stamp.sec) + float(stamp.nanosec) * 1e-9

    def _update_tracks(self, centroids, bboxes, now_t: float):
        C = np.array(centroids, dtype=np.float32) if len(centroids) else np.zeros((0, 2), dtype=np.float32)
        track_ids = list(self.tracks.keys())
        T = np.array([self.tracks[tid]["centroid"] for tid in track_ids], dtype=np.float32) if track_ids else np.zeros((0, 2), dtype=np.float32)
        unmatched_tracks = set(track_ids)
        unmatched_centroids = set(range(C.shape[0]))
        matches = []
        if T.size and C.size:
            D = np.linalg.norm(T[:, None, :] - C[None, :, :], axis=2)
            used_t, used_c = set(), set()
            while True:
                i, j = np.unravel_index(np.argmin(D), D.shape) if D.size else (None, None)
                if D.size == 0 or not np.isfinite(D[i, j]) or D[i, j] > self.max_assoc_dist:
                    break
                if i in used_t or j in used_c:
                    D[i, j] = np.inf
                    continue
                tid = track_ids[i]
                matches.append((tid, j))
                used_t.add(i); used_c.add(j)
                unmatched_tracks.discard(tid); unmatched_centroids.discard(j)
                D[i, :] = np.inf; D[:, j] = np.inf
        for tid, j in matches:
            prev_c = self.tracks[tid]["centroid"].copy()
            prev_t = self.tracks[tid]["last_t"]
            dt = max(1e-3, now_t - prev_t)
            inst_v = (C[j] - prev_c) / dt
            alpha = float(self.vel_smooth)
            self.tracks[tid]["vel"] = alpha * self.tracks[tid]["vel"] + (1.0 - alpha) * inst_v
            self.tracks[tid]["centroid"] = C[j]
            self.tracks[tid]["bbox"] = bboxes[j]
            self.tracks[tid]["age"] += 1
            self.tracks[tid]["missed"] = 0
            self.tracks[tid]["last_t"] = now_t
        for j in unmatched_centroids:
            tid = self.next_track_id; self.next_track_id += 1
            self.tracks[tid] = {
                "centroid": C[j], "vel": np.zeros(2, dtype=np.float32),
                "bbox": bboxes[j], "age": 1, "missed": 0, "last_t": now_t
            }
        for tid in list(unmatched_tracks):
            self.tracks[tid]["missed"] += 1
            self.tracks[tid]["last_t"] = now_t
            if self.tracks[tid]["missed"] > self.track_miss_tolerance:
                del self.tracks[tid]

    # -------------------- Marker cluster --------------------
    def _bbox_to_line_list_points(self, bb):
        minx, miny, maxx, maxy = bb
        return [self._pt(minx, miny), self._pt(maxx, miny),
                self._pt(maxx, miny), self._pt(maxx, maxy),
                self._pt(maxx, maxy), self._pt(minx, maxy),
                self._pt(minx, maxy), self._pt(minx, miny)]

    @staticmethod
    def _pt(x, y, z=0.0):
        from geometry_msgs.msg import Point as GeoPoint
        return GeoPoint(x=float(x), y=float(y), z=float(z))

    def _publish_cluster_markers(self):
        arr = MarkerArray()
        now = self.get_clock().now().to_msg()
        lifetime = Duration(sec=1, nanosec=0)
        current_ids = set()
        for tid, data in self.tracks.items():
            cx, cy = float(data["centroid"][0]), float(data["centroid"][1])
            vx, vy = float(data["vel"][0]), float(data["vel"][1])
            speed = math.hypot(vx, vy)
            bb = data.get("bbox", None)
            # centroide
            m = Marker()
            m.header.frame_id = self.marker_frame; m.header.stamp = now
            m.ns = "clusters"; m.id = 1000 + int(tid)
            m.type = Marker.SPHERE; m.action = Marker.ADD
            m.pose.position.x = cx; m.pose.position.y = cy; m.pose.position.z = 0.0
            m.pose.orientation.w = 1.0
            m.scale.x = self.centroid_diam; m.scale.y = self.centroid_diam; m.scale.z = self.centroid_diam

            # Verifica cluster con TAG
            verified, closest_tag, min_d = self._verify_cluster_with_tags(cx, cy, bb)
            if verified:
                m.color.r, m.color.g, m.color.b, m.color.a = self.verified_color
            else:
                m.color.r, m.color.g, m.color.b, m.color.a = self.centroid_color

            m.lifetime = lifetime
            arr.markers.append(m); current_ids.add(m.id)
            # testo
            t = Marker()
            t.header.frame_id = self.marker_frame; t.header.stamp = now
            t.ns = "clusters"; t.id = 2000 + int(tid)
            t.type = Marker.TEXT_VIEW_FACING; t.action = Marker.ADD
            t.pose.position.x = cx; t.pose.position.y = cy; t.pose.position.z = self.centroid_diam * 0.75
            t.pose.orientation.w = 1.0
            t.scale.z = self.label_scale
            t.color.r = 1.0; t.color.g = 1.0; t.color.b = 1.0; t.color.a = 0.95

            note = f"ID {tid}\n({cx:.2f},{cy:.2f})\n|v|={speed:.2f} m/s"
            if closest_tag is not None and min_d <= self.tag_assoc_dist:
                note += f"\nTAG {closest_tag}"
            t.text = note

            t.lifetime = lifetime
            arr.markers.append(t); current_ids.add(t.id)
            # bbox
            if bb is not None:
                b = Marker()
                b.header.frame_id = self.marker_frame; b.header.stamp = now
                b.ns = "clusters"; b.id = 3000 + int(tid)
                b.type = Marker.LINE_LIST; b.action = Marker.ADD
                b.scale.x = self.bbox_line_thickness
                bc = self.verified_color if verified else self.bbox_color
                b.color.r, b.color.g, b.color.b, b.color.a = bc
                b.points.extend(self._bbox_to_line_list_points(bb))
                b.lifetime = lifetime
                arr.markers.append(b); current_ids.add(b.id)

            # freccia velocità
            if speed > 1e-3:
                end_x = cx + vx * self.vel_arrow_scale
                end_y = cy + vy * self.vel_arrow_scale
                a = Marker()
                a.header.frame_id = self.marker_frame; a.header.stamp = now
                a.ns = "clusters"; a.id = 4000 + int(tid)
                a.type = Marker.ARROW; a.action = Marker.ADD
                a.scale.x = self.bbox_line_thickness * 0.9
                a.scale.y = self.bbox_line_thickness * 1.8
                a.scale.z = self.bbox_line_thickness * 3.0
                a.color.r, a.color.g, a.color.b, a.color.a = self.vel_color
                a.points.append(self._pt(cx, cy, 0.0)); a.points.append(self._pt(end_x, end_y, 0.0))
                a.lifetime = lifetime
                arr.markers.append(a); current_ids.add(a.id)
            else:
                d = Marker()
                d.header.frame_id = self.marker_frame; d.header.stamp = now
                d.ns = "clusters"; d.id = 4000 + int(tid)
                d.action = Marker.DELETE
                arr.markers.append(d)
        # elimina marker vecchi
        stale = self._prev_cluster_marker_ids - current_ids
        for mid in stale:
            dm = Marker()
            dm.header.frame_id = self.marker_frame; dm.header.stamp = now
            dm.ns = "clusters"; dm.id = int(mid); dm.action = Marker.DELETE
            arr.markers.append(dm)
        self._prev_cluster_marker_ids = current_ids
        if arr.markers:
            self.marker_pub.publish(arr)

    ### NEW: helper per verifica cluster-tag
    def _verify_cluster_with_tags(self, cx, cy, bb):
        closest_tag = None
        min_d = float("inf")
        for tid_tag, tg in self._last_tags.items():
            d = math.hypot(cx - tg["x"], cy - tg["y"])
            if d < min_d:
                min_d = d; closest_tag = tid_tag
        if closest_tag is None:
            return False, None, float("inf")
        tag = self._last_tags[closest_tag]
        by_radius = (min_d <= self.tag_assoc_dist)
        by_bbox = False
        if bb is not None:
            minx, miny, maxx, maxy = bb
            m = float(self.verify_bbox_margin)
            by_bbox = (minx - m <= tag["x"] <= maxx + m) and (miny - m <= tag["y"] <= maxy + m)
        mode = (self.verify_mode or "both").lower()
        if mode == "centroid_radius":
            return by_radius, closest_tag, min_d
        elif mode == "bbox":
            return by_bbox, closest_tag, min_d
        else:  # both
            return (by_radius or by_bbox), closest_tag, min_d

    # -------------------- Helpers --------------------
    def _points_inside_any_area(self, x: np.ndarray, y: np.ndarray):
        """Maschera OR di 'punto dentro' su tutte le aree, con pre-filtro bbox."""
        if not self.areas:
            return np.zeros_like(x, dtype=bool)
        mask_any = np.zeros_like(x, dtype=bool)
        for aid, data in self.areas.items():
            minx, miny, maxx, maxy = data["bounds"]
            cand = (~mask_any) & (x >= minx) & (x <= maxx) & (y >= miny) & (y <= maxy)
            if not np.any(cand):
                continue
            if data["is_axis_rect"]:
                mask_any |= cand
            else:
                idx = np.where(cand)[0]
                prep_poly = data["prep"]
                for i in idx:
                    if prep_poly.contains(Point(float(x[i]), float(y[i]))):
                        mask_any[i] = True
        return mask_any

    # -------------------- Callback Scan --------------------
    def scan_callback(self, msg: LaserScan):
        self._frame_count += 1
        stamp_t = self._stamp_to_sec(msg.header.stamp)

        rng = np.asarray(msg.ranges, dtype=np.float32)
        n = rng.shape[0]
        idx = np.arange(0, n, self.scan_stride, dtype=np.int32)
        rng = rng[idx]
        angles = (msg.angle_min + idx * msg.angle_increment).astype(np.float32)

        valid = np.isfinite(rng) & (rng > max(0.0, msg.range_min)) & (rng < (msg.range_max if msg.range_max > 0 else np.inf))
        if not np.any(valid):
            return
        rng = rng[valid]; angles = angles[valid]

        x = rng * np.cos(angles)
        y = rng * np.sin(angles)

        mask_inside = self._points_inside_any_area(x, y)
        if not np.any(mask_inside):
            if self._prev_cluster_marker_ids:
                self._update_tracks([], [], stamp_t)
                self._publish_cluster_markers()
                self._publish_tracks_json(stamp_t)  # stream vuoto
            return

        x_in = x[mask_inside]; y_in = y[mask_inside]
        pts_inside = np.stack((x_in, y_in), axis=1)

        if self.max_points_inside > 0 and pts_inside.shape[0] > self.max_points_inside:
            sel = np.random.choice(pts_inside.shape[0], self.max_points_inside, replace=False)
            pts_inside = pts_inside[sel]

        header = Header()
        header.frame_id = msg.header.frame_id
        header.stamp = msg.header.stamp
        pc2_msg = self.create_point_cloud2_message(
            header=header,
            points_xyz=np.c_[pts_inside, np.zeros((pts_inside.shape[0], 1), dtype=np.float32)]
        )
        if pc2_msg is not None:
            self.points_pub.publish(pc2_msg)

        labels = self._run_dbscan(pts_inside.astype(np.float32))
        centroids, bboxes = self._compute_centroids_and_bboxes(pts_inside, labels)

        self._update_tracks(centroids, bboxes, stamp_t)

        if (self._frame_count % self.print_every_n) == 0 and self.tracks:
            for tid, data in sorted(self.tracks.items()):
                cx, cy = float(data["centroid"][0]), float(data["centroid"][1])
                vx, vy = float(data["vel"][0]), float(data["vel"][1])
                speed = math.hypot(vx, vy)
                bb = data.get("bbox", None)
                if bb is not None:
                    minx, miny, maxx, maxy = bb
                    w, h = (maxx - minx), (maxy - miny)
                    self.get_logger().info(
                        f"Cluster ID={tid}  X={cx:.3f}  Y={cy:.3f}  "
                        f"Vx={vx:.3f}  Vy={vy:.3f}  |v|={speed:.3f} m/s  "
                        f"BBox=[({minx:.3f},{miny:.3f})-({maxx:.3f},{maxy:.3f}) {w:.3f}x{h:.3f} m]"
                    )
                else:
                    self.get_logger().info(
                        f"Cluster ID={tid}  X={cx:.3f}  Y={cy:.3f}  Vx={vx:.3f}  Vy={vy:.3f}  |v|={speed:.3f} m/s"
                    )

        self._publish_cluster_markers()
        self._publish_tracks_json(stamp_t)  # stream JSON
        self._publish_assocs_json()

    # -------------------- Pubblicazione JSON (aree & tracks) --------------------
    def _publish_areas_json(self):
        """Pubblica lo stato aree su /lam/areas (latched)."""
        areas = []
        for aid, data in self.areas.items():
            coords = [[float(x), float(y)] for (x, y) in list(data["poly"].exterior.coords)[:-1]]
            minx, miny, maxx, maxy = data["bounds"]
            areas.append({
                "id": int(aid),
                "coords": coords,
                "bounds": [float(minx), float(miny), float(maxx), float(maxy)]
            })
        msg = String()
        msg.data = json.dumps({"areas": areas})
        self.pub_areas.publish(msg)

    def _publish_tracks_json(self, now_t: float):
        """Pubblica le tracce su /lam/tracks (stream)."""
        tracks = []
        for tid, t in self.tracks.items():
            cx, cy = float(t["centroid"][0]), float(t["centroid"][1])
            vx, vy = float(t["vel"][0]), float(t["vel"][1])
            speed = float((vx * vx + vy * vy) ** 0.5)
            bb = t.get("bbox", None)
            bbox = [float(bb[0]), float(bb[1]), float(bb[2]), float(bb[3])] if bb is not None else None
            tracks.append({
                "id": int(tid),
                "x": cx, "y": cy,
                "vx": vx, "vy": vy, "speed": speed,
                "bbox": bbox
            })
        msg = String()
        msg.data = json.dumps({"stamp": float(now_t), "tracks": tracks})
        self.pub_tracks.publish(msg)

    def _publish_assocs_json(self):
        """Per ogni track, pubblica l'associazione al TAG più vicino e se è 'verified'."""
        payload = {"stamp": time.time(), "assocs": []}
        for tid, t in self.tracks.items():
            cx, cy = float(t["centroid"][0]), float(t["centroid"][1])
            bb = t.get("bbox", None)
            verified, tag_id, dist = self._verify_cluster_with_tags(cx, cy, bb)
            payload["assocs"].append({
                "track_id": int(tid),
                "tag_id": ("" if tag_id is None else str(tag_id)),
                "distance": float(dist if math.isfinite(dist) else 1e9),
                "verified": bool(verified)
            })
        msg = String(); msg.data = json.dumps(payload)
        self.pub_assocs.publish(msg)

    # ===================== Lettura & gestione UWB/antenna =====================
    _JSON_RE = re.compile(r'(\{.*?\})')  # NEW: non-greedy per match del solo blob JSON

    def _antenna_reader(self, port, baud):
        """Thread di lettura seriale antenna: parse righe JSxxxx{...} e push in coda. Auto-riconnessione."""
        while rclpy.ok():
            try:
                with serial.Serial(port, baudrate=baud, timeout=0.2) as ser:
                    self.get_logger().info(f"Seriale aperta: {port}@{baud}")
                    while rclpy.ok():
                        try:
                            line = ser.readline().decode('utf-8', errors='ignore').strip()
                            if not line:
                                continue
                            # Estrai il blob JSON {...}
                            m = self._JSON_RE.search(line)
                            if not m:
                                continue
                            data = json.loads(m.group(1))
                            twr = data.get("TWR", {})
                            tag_id = str(twr.get("a16")) if "a16" in twr else None
                            Xcm, Ycm, Dcm = twr.get("Xcm"), twr.get("Ycm"), twr.get("D")
                            if tag_id is None or Xcm is None or Ycm is None:
                                continue

                            # ---- distanza "raw" per calibrazione (cm) ----
                            if Dcm is None:
                                raw_d_cm = float((float(Xcm)**2 + float(Ycm)**2) ** 0.5)
                            else:
                                raw_d_cm = float(Dcm)
                            # Accumula nel buffer per calibrazione
                            self._raw_d_buffer.append(raw_d_cm)

                            # ---- distanza calibrata -> metri ----
                            d_cm = self.dist_a * raw_d_cm + self.dist_b
                            d_m = d_cm / 100.0

                            # coordinate antenna->laser (cm -> m + rototrasformazione)
                            x_m = float(Xcm) / 100.0
                            y_m = float(Ycm) / 100.0
                            lx, ly = self._antenna_to_laser(x_m, y_m)

                            # NEW: smoothing esponenziale lato nodo (riduce jitter)
                            old = self._tag_smooth.get(tag_id, (lx, ly))
                            alpha = float(self.vel_smooth_tag)
                            sx = alpha * old[0] + (1.0 - alpha) * lx
                            sy = alpha * old[1] + (1.0 - alpha) * ly
                            self._tag_smooth[tag_id] = (sx, sy)

                            self._uwb_queue.put_nowait({"id": tag_id, "x": sx, "y": sy, "d": d_m, "stamp": time.time()})
                        except queue.Full:
                            pass
                        except Exception as e:
                            self.get_logger().warn(f"antenna_reader loop: {e}")
            except Exception as e:
                self.get_logger().error(f"Impossibile aprire {port}@{baud}: {e}; retry tra 1s")
                time.sleep(1.0)

    def _antenna_to_laser(self, x_m, y_m):
        """Rototrasformazione piano (yaw) + offset antenna rispetto al frame 'laser'."""
        th = math.radians(self.antenna_yaw_deg)
        c, s = math.cos(th), math.sin(th)
        xr = c * x_m - s * y_m
        yr = s * x_m + c * y_m
        return xr + self.antenna_dx, yr + self.antenna_dy

    # === NEW: callback per ricevere TAG dal bridge esterno (/lam/uwb_tags) ===
    def _uwb_tags_callback(self, msg: String):
        """Aggiorna self._last_tags dai messaggi JSON del bridge UWB esterno.
        Accetta sia 'cm' che 'm'. Se presenti, preferisce i campi x_m/y_m/d_m.
        Internamente salviamo sempre in METRI.
        """
        try:
            data = json.loads(msg.data)
            now = float(data.get("stamp", time.time()))
            units = str(data.get("units", "m")).lower()  # "cm" | "m"

            for t in data.get("tags", []):
                tid = str(t.get("id", "")).strip()
                if not tid:
                    continue

                # Se il bridge include i metri, usa quelli
                if all(k in t for k in ("x_m", "y_m", "d_m")):
                    x = float(t["x_m"]); y = float(t["y_m"]); d = float(t["d_m"])
                else:
                    # Altrimenti usa x,y,d e converti se in cm
                    x = float(t.get("x", 0.0))
                    y = float(t.get("y", 0.0))
                    d = float(t.get("d", 0.0))
                    if units == "cm":
                        x *= 0.01; y *= 0.01; d *= 0.01

                self._last_tags[tid] = {"x": x, "y": y, "d": d, "stamp": now}

            # TTL: rimuovi tag scaduti rispetto all'orologio locale
            cut = time.time() - self.tag_ttl_sec
            old = [tid for tid, tt in self._last_tags.items() if tt["stamp"] < cut]
            for tid in old:
                self._last_tags.pop(tid, None)

            # Aggiorna marker e (eventualmente) colore cluster verificati
            self._publish_tag_markers()
            if self.tracks:
                self._publish_cluster_markers()
                self._publish_assocs_json()

        except Exception as e:
            try:
                self.get_logger().warn(f"uwb_tags_callback parse err: {e}")
            except Exception:
                pass

    def _process_uwb(self):
        """Consuma coda UWB, pubblica JSON, disegna marker antenna/tag e trigger colore cluster."""
        now = time.time()
        # svuota coda
        while True:
            try:
                item = self._uwb_queue.get_nowait()
                self._last_tags[item["id"]] = item
            except queue.Empty:
                break
        # scadenza tag vecchi
        old = [tid for tid, t in self._last_tags.items() if (now - t["stamp"]) > self.tag_ttl_sec]
        for tid in old:
            del self._last_tags[tid]
        # stream JSON per web bridge
        msg = String()
        msg.data = json.dumps({
            "stamp": now,
            "tags": [{"id": tid, "x": t["x"], "y": t["y"], "d": t["d"]} for tid, t in self._last_tags.items()]
        })

        # pubblica su /lam/uwb_tags solo se stai usando la seriale interna
        if not self.use_external_uwb:
            self.pub_uwb.publish(msg)

        # marker antenna e tag
        self._publish_antenna_markers()
        self._publish_tag_markers()

        # ridisegna i cluster (per colorazione “verified”)
        if self.tracks:
            self._publish_cluster_markers()
            self._publish_assocs_json()

    def _publish_antenna_markers(self):
        """Mostra posizione/orientamento LiDAR e antenna."""
        arr = MarkerArray()
        now = self.get_clock().now().to_msg()
        ns = "antenna"
        current = set()

        # cilindro antenna
        m = Marker()
        m.header.frame_id = self.marker_frame; m.header.stamp = now
        m.ns = ns; m.id = 1
        m.type = Marker.CYLINDER; m.action = Marker.ADD
        m.pose.position.x = self.antenna_dx; m.pose.position.y = self.antenna_dy; m.pose.position.z = 0.0
        m.pose.orientation.w = 1.0
        m.scale.x = 0.06; m.scale.y = 0.06; m.scale.z = 0.02
        m.color.r, m.color.g, m.color.b, m.color.a = self.antenna_body_color
        m.frame_locked = True
        arr.markers.append(m); current.add(m.id)

        # asse +X del LiDAR
        la = Marker()
        la.header.frame_id = self.marker_frame; la.header.stamp = now
        la.ns = ns; la.id = 2
        la.type = Marker.ARROW; la.action = Marker.ADD
        la.scale.x = self.line_thickness * 0.9
        la.scale.y = self.line_thickness * 1.8
        la.scale.z = self.line_thickness * 3.0
        la.color.r, la.color.g, la.color.b, la.color.a = self.lidar_axis_color
        la.points.append(self._pt(0.0, 0.0, 0.0))
        la.points.append(self._pt(self.lidar_axis_len, 0.0, 0.0))
        arr.markers.append(la); current.add(la.id)

        # orientamento antenna (freccia)
        ax = Marker()
        ax.header.frame_id = self.marker_frame; ax.header.stamp = now
        ax.ns = ns; ax.id = 3
        ax.type = Marker.ARROW; ax.action = Marker.ADD
        ax.scale.x = self.line_thickness * 0.9
        ax.scale.y = self.line_thickness * 1.8
        ax.scale.z = self.line_thickness * 3.0
        ax.color.r, ax.color.g, ax.color.b, ax.color.a = self.antenna_axis_color
        x2 = self.antenna_dx + self.antenna_axis_len * math.cos(math.radians(self.antenna_yaw_deg))
        y2 = self.antenna_dy + self.antenna_axis_len * math.sin(math.radians(self.antenna_yaw_deg))
        ax.points.append(self._pt(self.antenna_dx, self.antenna_dy, 0.0))
        ax.points.append(self._pt(x2, y2, 0.0))
        arr.markers.append(ax); current.add(ax.id)

        # cleanup vecchi
        stale = self._antenna_marker_ids - current
        for mid in stale:
            dm = Marker()
            dm.header.frame_id = self.marker_frame; dm.header.stamp = now
            dm.ns = ns; dm.id = int(mid); dm.action = Marker.DELETE
            arr.markers.append(dm)
        self._antenna_marker_ids = current
        if arr.markers:
            self.marker_pub.publish(arr)

    def _publish_tag_markers(self):
        """Mostra i tag UWB correnti con ID stabili (sfere + label) e cancella i vecchi."""
        arr = MarkerArray()
        now = self.get_clock().now().to_msg()
        ns = "uwb_tags"
        lifetime = Duration(sec=0, nanosec=350_000_000)  # ~0.35 s: movimento più fluido
        current = set()

        def _mid(tid: str) -> int:
            # ID stabili nel processo per non “saltare” fra ri-pubblicazioni
            return (hash(tid) & 0x0FFF)

        for tid, t in sorted(self._last_tags.items()):
            base = _mid(tid)

            # Sfera
            m = Marker()
            m.header.frame_id = self.marker_frame; m.header.stamp = now
            m.ns = ns; m.id = 5000 + base
            m.type = Marker.SPHERE; m.action = Marker.ADD
            m.pose.position.x = t["x"]; m.pose.position.y = t["y"]; m.pose.position.z = 0.0
            m.scale.x = 0.06; m.scale.y = 0.06; m.scale.z = 0.06
            m.color.r, m.color.g, m.color.b, m.color.a = self.tag_color
            m.frame_locked = True
            m.lifetime = lifetime
            arr.markers.append(m); current.add((ns, m.id))

            # Label
            l = Marker()
            l.header.frame_id = self.marker_frame; l.header.stamp = now
            l.ns = ns; l.id = 6000 + base
            l.type = Marker.TEXT_VIEW_FACING; l.action = Marker.ADD
            l.pose.position.x = t["x"]; l.pose.position.y = t["y"]; l.pose.position.z = 0.10
            l.scale.z = self.tag_label_scale
            l.color.r, l.color.g, l.color.b, l.color.a = (1.0, 1.0, 1.0, 0.95)
            l.text = f"TAG {tid}\n({t['x']:.2f},{t['y']:.2f}) m"
            l.frame_locked = True
            l.lifetime = lifetime
            arr.markers.append(l); current.add((ns, l.id))

        # Cancella i marker dei tag che non ci sono più
        stale = self._prev_tag_marker_ids - current
        for (sns, mid) in stale:
            d = Marker()
            d.header.frame_id = self.marker_frame; d.header.stamp = now
            d.ns = sns; d.id = mid; d.action = Marker.DELETE
            arr.markers.append(d)

        self._prev_tag_marker_ids = current
        if arr.markers:
            self.marker_pub.publish(arr)

    ### NEW: TF statico per antenna rispetto a marker_frame (laser)
    def _publish_antenna_tf(self):
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = self.marker_frame
        t.child_frame_id = "antenna"
        t.transform.translation.x = self.antenna_dx
        t.transform.translation.y = self.antenna_dy
        t.transform.translation.z = 0.0
        th = math.radians(self.antenna_yaw_deg)
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = math.sin(th/2.0)
        t.transform.rotation.w = math.cos(th/2.0)
        try:
            self._tf_static.sendTransform(t)
        except Exception as e:
            self.get_logger().warn(f"TF static antenna: {e}")

    # ===================== Servizi Calibrazione Assistita =====================
    def uwb_calib_reset(self, request, response):
        """Resetta i campioni di calibrazione raccolti."""
        self._calib_pairs.clear()
        response.success = True
        response.message = "Calibrazione: buffer campioni svuotato."
        self.get_logger().info(response.message)
        return response

    def uwb_calib_capture(self, request, response):
        """Cattura un campione: media delle ultime 'calib_window' distanze raw (cm) associata a calib_true_m."""
        if len(self._raw_d_buffer) == 0:
            response.success = False
            response.message = "Nessun dato raw D disponibile (attendi arrivo pacchetti)."
            return response
        n = min(self.calib_window, len(self._raw_d_buffer))
        vals = list(self._raw_d_buffer)[-n:]   # media degli ultimi n
        meas_cm = float(np.mean(vals))
        true_cm = float(self.calib_true_m) * 100.0
        self._calib_pairs.append((meas_cm, true_cm))
        response.success = True
        response.message = f"Campione acquisito: meas={meas_cm:.2f} cm  true={true_cm:.2f} cm  (tot={len(self._calib_pairs)})"
        self.get_logger().info(response.message)
        return response

    def uwb_calib_finish(self, request, response):
        """Chiude la calibrazione: regressione lineare (true_cm ≈ a*meas_cm + b) e aggiorna dist_a/dist_b."""
        if len(self._calib_pairs) < 2:
            response.success = False
            response.message = f"Servono almeno 2 campioni, attuali: {len(self._calib_pairs)}"
            return response
        # regressione lineare y = a x + b (formule chiuse)
        X = np.array([p[0] for p in self._calib_pairs], dtype=np.float64)  # meas_cm
        Y = np.array([p[1] for p in self._calib_pairs], dtype=np.float64)  # true_cm
        x_mean, y_mean = X.mean(), Y.mean()
        denom = np.sum((X - x_mean) ** 2)
        if denom <= 1e-12:
            response.success = False
            response.message = "Campioni non sufficientemente vari (varianza zero)."
            return response
        a = float(np.sum((X - x_mean) * (Y - y_mean)) / denom)
        b = float(y_mean - a * x_mean)

        # aggiorna parametri dist_a/dist_b
        self.set_parameters([
            rclpy.parameter.Parameter("dist_a", rclpy.Parameter.Type.DOUBLE, a),
            rclpy.parameter.Parameter("dist_b", rclpy.Parameter.Type.DOUBLE, b),
        ])
        self.dist_a, self.dist_b = a, b

        response.success = True
        response.message = f"Calibrazione OK: dist_a={a:.6f}, dist_b={b:.6f} (cm). Applicata."
        self.get_logger().info(response.message)
        return response

def main():
    rclpy.init()
    node = LidarAreaMonitor()
    node.get_logger().info(f"Spinning {node.get_name()} ...")
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.get_logger().info("Shutting down ...")
    rclpy.shutdown()


if __name__ == "__main__":
    main()
