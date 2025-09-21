#!/usr/bin/env python3
import math
import struct
import numpy as np

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy, HistoryPolicy

from std_msgs.msg import Header
from sensor_msgs.msg import LaserScan, PointCloud2, PointField
from visualization_msgs.msg import Marker, MarkerArray
from rcl_interfaces.msg import SetParametersResult
from builtin_interfaces.msg import Duration

from shapely.geometry import Polygon, Point, box as shapely_box
from shapely.prepared import prep

from ros2_lam_interfaces.srv import SetExclusionArea
from std_srvs.srv import Trigger


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

        # === Servizi aree ===
        # Retro-compatibile: sostituisce TUTTO con un'unica area
        self.area_srv = self.create_service(SetExclusionArea, "set_exclusion_area", self.set_exclusion_area)
        # Aggiunge una nuova area (stesso tipo di request)
        self.add_area_srv = self.create_service(SetExclusionArea, "add_exclusion_area", self.add_exclusion_area)
        # Svuota tutte le aree
        self.clear_areas_srv = self.create_service(Trigger, "clear_exclusion_areas", self.clear_exclusion_areas)

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

        self._reload_style_params()
        self._reload_cluster_params()
        self._reload_bbox_vel_params()
        self._reload_perf_params()
        self.add_on_set_parameters_callback(self._on_params)

        # === Stato tracking ===
        self.next_track_id = 1
        # track: tid -> {'centroid': np.array([x,y]), 'vel': np.array([vx,vy]),
        #                'bbox': (minx,miny,maxx,maxy), 'age': int, 'missed': int, 'last_t': float}
        self.tracks = {}
        self._prev_cluster_marker_ids = set()
        self._frame_count = 0

        # tracking dei marker area per cancellare quelli rimossi
        self._prev_area_ns_ids = set()  # {(ns, id), ...}

        # === DBSCAN backend ===
        try:
            from sklearn.cluster import DBSCAN as SK_DBSCAN  # noqa
            self._has_sklearn = True
        except Exception:
            self._has_sklearn = False
            self.get_logger().warn("scikit-learn non trovato: uso DBSCAN a griglia (veloce su ARM).")

        # Pubblica subito i marker delle aree
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

    def _on_params(self, params):
        changed_areas = False
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
        if changed_areas:
            self.create_area_markers()
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
            from sklearn.cluster import DBSCAN as SK_DBSCAN
            db = SK_DBSCAN(eps=eps, min_samples=min_pts, metric="euclidean").fit(pts_xy)
            return db.labels_.astype(np.int32)
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
            t.text = f"ID {tid}\n({cx:.2f},{cy:.2f})\n|v|={speed:.2f} m/s"
            t.lifetime = lifetime
            arr.markers.append(t); current_ids.add(t.id)
            # bbox
            if bb is not None:
                b = Marker()
                b.header.frame_id = self.marker_frame; b.header.stamp = now
                b.ns = "clusters"; b.id = 3000 + int(tid)
                b.type = Marker.LINE_LIST; b.action = Marker.ADD
                b.scale.x = self.bbox_line_thickness
                b.color.r, b.color.g, b.color.b, b.color.a = self.bbox_color
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

    # -------------------- Helpers --------------------
    def _points_inside_any_area(self, x: np.ndarray, y: np.ndarray):
        """Maschera OR di 'punto dentro' su tutte le aree, con pre-filtro bbox."""
        if not self.areas:
            return np.zeros_like(x, dtype=bool)
        mask_any = np.zeros_like(x, dtype=bool)
        # iterazione su aree; shapely solo sui candidati bbox non già inclusi
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
            return

        x_in = x[mask_inside]; y_in = y[mask_inside]
        pts_inside = np.stack((x_in, y_in), axis=1)

        if self.max_points_inside > 0 and pts_inside.shape[0] > self.max_points_inside:
            sel = np.random.choice(pts_inside.shape[0], self.max_points_inside, replace=False)
            pts_inside = pts_inside[sel]

        header = Header()
        header.frame_id = msg.header.frame_id
        header.stamp = msg.header.stamp
        pc2_msg = self.create_point_cloud2_message(header=header, points_xyz=np.c_[pts_inside, np.zeros((pts_inside.shape[0], 1), dtype=np.float32)])
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
