import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan, PointCloud2, PointField
from visualization_msgs.msg import Marker, MarkerArray
from std_msgs.msg import Header
from rcl_interfaces.msg import SetParametersResult

from shapely.geometry import Polygon, Point  # fix import shapely
import math
import struct

from ros2_lam_interfaces.srv import SetExclusionArea

class LidarAreaMonitor(Node):
    def __init__(self, node_name = "lidar_area_monitor_node" ):
        super().__init__(node_name)
        
        self.lidar_sub = self.create_subscription(LaserScan, "/scan", self.scan_callback, 10)
        self.points_pub = self.create_publisher(PointCloud2, "/points_inside", 10)

        # poligono di default
        exclusion_area_points = [
            [0.5, 0.5],
            [0.5, -0.5],
            [0.1, -0.5],
            [0.1, 0.5]
        ]
        self.poly_area = Polygon(exclusion_area_points)

        # === Parametri stile marker (modificabili a runtime) ===
        self.declare_parameter('line_thickness', 0.02)                 # m
        self.declare_parameter('line_color', [1.0, 0.35, 0.0, 1.0])     # RGBA
        self.declare_parameter('sphere_size', 0.0)                    # m (diametro)
        self.declare_parameter('sphere_color', [1.0, 0.35, 0.0, 1.0])   # RGBA
        self._reload_style_params()
        self.add_on_set_parameters_callback(self._on_params)

        # service per cambiare area
        self.area_srv = self.create_service(SetExclusionArea, "set_exclusion_area", self.set_exclusion_area)

        # pubblica i marker all'avvio
        self.create_exclusion_marker()
        
    def _reload_style_params(self):
        self.line_thickness = float(self.get_parameter('line_thickness').value)
        self.line_color = [float(x) for x in self.get_parameter('line_color').value]          # [r,g,b,a]
        self.sphere_size = float(self.get_parameter('sphere_size').value)
        self.sphere_color = [float(x) for x in self.get_parameter('sphere_color').value]      # [r,g,b,a]

    def _on_params(self, params):
        changed = False
        for p in params:
            if p.name == 'line_thickness':
                self.line_thickness = float(p.value); changed = True
            elif p.name == 'line_color':
                self.line_color = [float(x) for x in p.value]; changed = True
            elif p.name == 'sphere_size':
                self.sphere_size = float(p.value); changed = True
            elif p.name == 'sphere_color':
                self.sphere_color = [float(x) for x in p.value]; changed = True
        if changed:
            self.create_exclusion_marker()  # ripubblica con lo stile aggiornato
        return SetParametersResult(successful=True)

    def set_exclusion_area(self, request, response):
        try:
            exclusion_area_points = []
            for p in request.point_list:
                exclusion_area_points.append([p.x, p.y])

            # aggiorna il poligono e ripubblica i marker
            self.poly_area = Polygon(exclusion_area_points)
            self.create_exclusion_marker()

            response.is_ok = True
            response.error_msg = ''
        except Exception as ex:
            self.get_logger().error(f"{ex=}")
            response.is_ok = False
            response.error_msg = f"{ex=}"
        return response

    def create_exclusion_marker(self):
        # Import locali per evitare conflitti con shapely.Point
        from geometry_msgs.msg import Point as GeoPoint
        from std_msgs.msg import ColorRGBA
        from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy, HistoryPolicy

        # Publisher "latched" (TRANSIENT_LOCAL) così RViz riceve anche se parte dopo
        if not hasattr(self, "marker_pub"):
            qos = QoSProfile(
                depth=1,
                reliability=ReliabilityPolicy.RELIABLE,
                durability=DurabilityPolicy.TRANSIENT_LOCAL,
                history=HistoryPolicy.KEEP_LAST,
            )
            self.marker_pub = self.create_publisher(
                MarkerArray, "/visualization_marker_array", qos
            )

        # Coordinate del poligono (chiuso)
        coords = list(self.poly_area.exterior.coords)[:-1]  # togli duplicato finale
        closed = coords + [coords[0]]

        # --- LINEA DEL POLIGONO ---
        line = Marker()
        line.header.frame_id = "base_link"     # cambia se vuoi
        line.header.stamp = self.get_clock().now().to_msg()
        line.ns = "exclusion_area"
        line.id = 0
        line.type = Marker.LINE_STRIP
        line.action = Marker.ADD
        line.pose.orientation.w = 1.0
        line.scale.x = self.line_thickness
        line.color.r, line.color.g, line.color.b, line.color.a = self.line_color
        for x, y in closed:
            line.points.append(GeoPoint(x=x, y=y, z=0.0))

        # --- VERTICI COME SFERE ---
        spheres = Marker()
        spheres.header = line.header
        spheres.ns = "exclusion_area"
        spheres.id = 1
        spheres.type = Marker.SPHERE_LIST
        spheres.action = Marker.ADD
        spheres.scale.x = self.sphere_size
        spheres.scale.y = self.sphere_size
        spheres.scale.z = self.sphere_size
        # colore di base (alcune versioni di RViz lo considerano comunque)
        spheres.color.r, spheres.color.g, spheres.color.b, spheres.color.a = self.sphere_color
        for x, y in coords:
            spheres.points.append(GeoPoint(x=x, y=y, z=0.0))
            spheres.colors.append(ColorRGBA(
                r=self.sphere_color[0], g=self.sphere_color[1],
                b=self.sphere_color[2], a=self.sphere_color[3]
            ))

        # publish
        arr = MarkerArray()
        arr.markers.append(line)
        arr.markers.append(spheres)
        self.marker_pub.publish(arr)
        return arr

    def scan_callback(self, msg : LaserScan):
        points_inside = []
        for r, i in zip(msg.ranges, range(len(msg.ranges))):
            x = r * math.cos(msg.angle_min + i*msg.angle_increment)
            y = r * math.sin(msg.angle_min + i*msg.angle_increment)
            if self.poly_area.contains(Point([x,y])):
                points_inside.append([x, y, 0.0])
        # print(points_inside)  # verbose

        if len(points_inside) > 0:
            header = Header()
            header.frame_id = msg.header.frame_id
            header.stamp = msg.header.stamp
            pc2_msg = self.create_point_cloud2_message(header=header, points=points_inside)
            self.points_pub.publish(pc2_msg)
        
    def create_point_cloud2_message(self, header, points):
        """Create the PointCloud2 message from a list of 3D points."""
        cloud_data = []
        for point in points:
            cloud_data.append(struct.pack('fff', *point))
        cloud_data = b''.join(cloud_data)

        pc2_msg = PointCloud2()
        pc2_msg.header = header
        pc2_msg.height = 1
        pc2_msg.width = len(points)
        pc2_msg.is_dense = True
        pc2_msg.is_bigendian = False
        pc2_msg.fields = [
            PointField(name="x", offset=0, datatype=PointField.FLOAT32, count=1),
            PointField(name="y", offset=4, datatype=PointField.FLOAT32, count=1),
            PointField(name="z", offset=8, datatype=PointField.FLOAT32, count=1),
        ]
        pc2_msg.point_step = 12
        pc2_msg.row_step = pc2_msg.point_step * len(points)
        pc2_msg.data = cloud_data

        return pc2_msg

def main():
    rclpy.init()
    node = LidarAreaMonitor()
    print(f"Spinning {node.get_name()} !")
    rclpy.spin(node)
    print(f"Shutting down ...")
    rclpy.shutdown()

if __name__=="__main__":
    main()
