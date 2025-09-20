// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ros2_lam_interfaces:srv/SetExclusionArea.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros2_lam_interfaces/srv/set_exclusion_area.h"


#ifndef ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__STRUCT_H_
#define ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'point_list'
#include "geometry_msgs/msg/detail/point__struct.h"

/// Struct defined in srv/SetExclusionArea in the package ros2_lam_interfaces.
typedef struct ros2_lam_interfaces__srv__SetExclusionArea_Request
{
  geometry_msgs__msg__Point__Sequence point_list;
} ros2_lam_interfaces__srv__SetExclusionArea_Request;

// Struct for a sequence of ros2_lam_interfaces__srv__SetExclusionArea_Request.
typedef struct ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence
{
  ros2_lam_interfaces__srv__SetExclusionArea_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'error_msg'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetExclusionArea in the package ros2_lam_interfaces.
typedef struct ros2_lam_interfaces__srv__SetExclusionArea_Response
{
  bool is_ok;
  rosidl_runtime_c__String error_msg;
} ros2_lam_interfaces__srv__SetExclusionArea_Response;

// Struct for a sequence of ros2_lam_interfaces__srv__SetExclusionArea_Response.
typedef struct ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence
{
  ros2_lam_interfaces__srv__SetExclusionArea_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  ros2_lam_interfaces__srv__SetExclusionArea_Event__request__MAX_SIZE = 1
};
// response
enum
{
  ros2_lam_interfaces__srv__SetExclusionArea_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/SetExclusionArea in the package ros2_lam_interfaces.
typedef struct ros2_lam_interfaces__srv__SetExclusionArea_Event
{
  service_msgs__msg__ServiceEventInfo info;
  ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence request;
  ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence response;
} ros2_lam_interfaces__srv__SetExclusionArea_Event;

// Struct for a sequence of ros2_lam_interfaces__srv__SetExclusionArea_Event.
typedef struct ros2_lam_interfaces__srv__SetExclusionArea_Event__Sequence
{
  ros2_lam_interfaces__srv__SetExclusionArea_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros2_lam_interfaces__srv__SetExclusionArea_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__STRUCT_H_
