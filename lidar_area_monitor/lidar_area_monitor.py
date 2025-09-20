import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan, PointCloud2, PointField
from visualization_msgs.msg import Marker, MarkerArray
from std_msgs.msg import Header

from shapely import Polygon, Point
import math
import struct

from ros2_lam_interfaces.srv import SetExclusionArea

class LidarAreaMonitor(Node):
    def __init__(self, node_name = "lidar_area_monitor_node" ):
        super().__init__(node_name)
        
        self.lidar_sub = self.create_subscription(LaserScan, "/scan", self.scan_callback, 10)
        self.points_pub = self.create_publisher(PointCloud2, "/points_inside", 10)
        exclusion_area_points = [
            [0.5, 0.5],
            [0.5, -0.5],
            [0.1, -0.5],
            [0.1, 0.5]
        ]
        self.poly_area = Polygon(exclusion_area_points)
        
        self.area_srv = self.create_service(SetExclusionArea, "set_exclusion_area", self.set_exclusion_area)
        
    def set_exclusion_area(self, request, response):
        #TODO sanity check
        try:
            exclusion_area_points = []
            for point in request.point_list:
                exclusion_area_points.append([point.x, point.y])

            self.poly_area = Polygon(exclusion_area_points)
            response.is_ok = True
            return response
        except Exception as ex:
            self.get_logger().error(f"{ex=}")
            response.is_ok = False
            response.error_msg = f"{ex=}"
    
    def create_exclusion_marker(self):
        marker = Marker()
        marker.header.frame_id = "base_link"
        marker.header.stamp = self.get_clock().now().to_msg()
        marker.ns = "exclusion_area"
        marker_array = MarkerArray()
        marker.id = 0
        marker.type = Marker.LINE_STRIP
        marker.action = Marker.ADD
        marker.pose.orientation.w = 1.0
        marker.scale.x = 0.1  # Spessore della linea
        marker.color.r = 1.0
        marker.color.g = 0.0
        marker.color.b = 0.0
        marker.color.a = 1.0
    
        # Aggiungi i punti del poligono
        for point in self.poly_area.exterior.coords:
            marker.points.append(Point(x=point[0], y=point[1], z=0.0))

        marker.points.append(marker.points[0])  # Chiudi il poligono
        marker_array.markers.append(marker)
        self.marker_pub = self.create_publisher(MarkerArray, "/visualization_marker_array", 10)

        # Esegui la pubblicazione
        self.marker_pub.publish(self.create_exclusion_marker())

        return marker_array

    
    def scan_callback(self, msg : LaserScan):
        #self.latest_msg = msg
        points_inside = []
        for r, i in zip(msg.ranges, range(len(msg.ranges))):
            x = r * math.cos(msg.angle_min + i*msg.angle_increment)
            y = r * math.sin(msg.angle_min + i*msg.angle_increment)
            if self.poly_area.contains(Point([x,y])):
                points_inside.append([x, y, 0.0])
        print(points_inside)
        if len(points_inside) > 0:
            header = Header()
            header.frame_id = msg.header.frame_id
            header.stamp = msg.header.stamp
            pc2_msg = self.create_point_cloud2_message(header=header, points=points_inside)
            self.points_pub.publish(pc2_msg)
        
        
    def create_point_cloud2_message(self, header, points):
        """Create the PointCloud2 message from a list of 3D points."""
        # Create a list of points with x, y, z coordinates
        cloud_data = []
        for point in points:
            # Each point is a tuple (x, y, z)
            cloud_data.append(struct.pack('fff', *point))

        # Flatten the list of packed points
        cloud_data = b''.join(cloud_data)

        # Create the PointCloud2 message
        pc2_msg = PointCloud2()
        pc2_msg.header = header
        pc2_msg.height = 1  # For 1D point clouds (could be different for 2D grids)
        pc2_msg.width = len(points)
        pc2_msg.is_dense = True
        pc2_msg.is_bigendian = False
        pc2_msg.fields = [
            # Define the fields (x, y, z) of the point cloud
            PointField(name="x", offset=0, datatype=PointField.FLOAT32, count=1),
            PointField(name="y", offset=4, datatype=PointField.FLOAT32, count=1),
            PointField(name="z", offset=8, datatype=PointField.FLOAT32, count=1),
        ]
        pc2_msg.point_step = 12  # Number of bytes per point (3 floats, each 4 bytes)
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