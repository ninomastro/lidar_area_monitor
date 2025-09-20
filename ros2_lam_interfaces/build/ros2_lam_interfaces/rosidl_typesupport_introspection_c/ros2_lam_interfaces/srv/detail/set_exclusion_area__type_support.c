// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ros2_lam_interfaces:srv/SetExclusionArea.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ros2_lam_interfaces/srv/detail/set_exclusion_area__rosidl_typesupport_introspection_c.h"
#include "ros2_lam_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ros2_lam_interfaces/srv/detail/set_exclusion_area__functions.h"
#include "ros2_lam_interfaces/srv/detail/set_exclusion_area__struct.h"


// Include directives for member types
// Member `point_list`
#include "geometry_msgs/msg/point.h"
// Member `point_list`
#include "geometry_msgs/msg/detail/point__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_lam_interfaces__srv__SetExclusionArea_Request__init(message_memory);
}

void ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_fini_function(void * message_memory)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Request__fini(message_memory);
}

size_t ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__size_function__SetExclusionArea_Request__point_list(
  const void * untyped_member)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Request__point_list(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Request__point_list(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__fetch_function__SetExclusionArea_Request__point_list(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Point * item =
    ((const geometry_msgs__msg__Point *)
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Request__point_list(untyped_member, index));
  geometry_msgs__msg__Point * value =
    (geometry_msgs__msg__Point *)(untyped_value);
  *value = *item;
}

void ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__assign_function__SetExclusionArea_Request__point_list(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Point * item =
    ((geometry_msgs__msg__Point *)
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Request__point_list(untyped_member, index));
  const geometry_msgs__msg__Point * value =
    (const geometry_msgs__msg__Point *)(untyped_value);
  *item = *value;
}

bool ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__resize_function__SetExclusionArea_Request__point_list(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  geometry_msgs__msg__Point__Sequence__fini(member);
  return geometry_msgs__msg__Point__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_member_array[1] = {
  {
    "point_list",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_lam_interfaces__srv__SetExclusionArea_Request, point_list),  // bytes offset in struct
    NULL,  // default value
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__size_function__SetExclusionArea_Request__point_list,  // size() function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Request__point_list,  // get_const(index) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Request__point_list,  // get(index) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__fetch_function__SetExclusionArea_Request__point_list,  // fetch(index, &value) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__assign_function__SetExclusionArea_Request__point_list,  // assign(index, value) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__resize_function__SetExclusionArea_Request__point_list  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_members = {
  "ros2_lam_interfaces__srv",  // message namespace
  "SetExclusionArea_Request",  // message name
  1,  // number of fields
  sizeof(ros2_lam_interfaces__srv__SetExclusionArea_Request),
  false,  // has_any_key_member_
  ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_member_array,  // message members
  ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_type_support_handle = {
  0,
  &ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_members,
  get_message_typesupport_handle_function,
  &ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_hash,
  &ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_description,
  &ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_lam_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Request)() {
  ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  if (!ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_type_support_handle.typesupport_identifier) {
    ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_lam_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__functions.h"
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__struct.h"


// Include directives for member types
// Member `error_msg`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_lam_interfaces__srv__SetExclusionArea_Response__init(message_memory);
}

void ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_fini_function(void * message_memory)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_member_array[2] = {
  {
    "is_ok",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_lam_interfaces__srv__SetExclusionArea_Response, is_ok),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "error_msg",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_lam_interfaces__srv__SetExclusionArea_Response, error_msg),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_members = {
  "ros2_lam_interfaces__srv",  // message namespace
  "SetExclusionArea_Response",  // message name
  2,  // number of fields
  sizeof(ros2_lam_interfaces__srv__SetExclusionArea_Response),
  false,  // has_any_key_member_
  ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_member_array,  // message members
  ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle = {
  0,
  &ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_members,
  get_message_typesupport_handle_function,
  &ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_hash,
  &ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_description,
  &ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_lam_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Response)() {
  if (!ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle.typesupport_identifier) {
    ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_lam_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__functions.h"
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
#include "ros2_lam_interfaces/srv/set_exclusion_area.h"
// Member `request`
// Member `response`
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_lam_interfaces__srv__SetExclusionArea_Event__init(message_memory);
}

void ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_fini_function(void * message_memory)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Event__fini(message_memory);
}

size_t ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__size_function__SetExclusionArea_Event__request(
  const void * untyped_member)
{
  const ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence * member =
    (const ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Event__request(
  const void * untyped_member, size_t index)
{
  const ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence * member =
    (const ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Event__request(
  void * untyped_member, size_t index)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence * member =
    (ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__fetch_function__SetExclusionArea_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_lam_interfaces__srv__SetExclusionArea_Request * item =
    ((const ros2_lam_interfaces__srv__SetExclusionArea_Request *)
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Event__request(untyped_member, index));
  ros2_lam_interfaces__srv__SetExclusionArea_Request * value =
    (ros2_lam_interfaces__srv__SetExclusionArea_Request *)(untyped_value);
  *value = *item;
}

void ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__assign_function__SetExclusionArea_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Request * item =
    ((ros2_lam_interfaces__srv__SetExclusionArea_Request *)
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Event__request(untyped_member, index));
  const ros2_lam_interfaces__srv__SetExclusionArea_Request * value =
    (const ros2_lam_interfaces__srv__SetExclusionArea_Request *)(untyped_value);
  *item = *value;
}

bool ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__resize_function__SetExclusionArea_Event__request(
  void * untyped_member, size_t size)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence * member =
    (ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence *)(untyped_member);
  ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence__fini(member);
  return ros2_lam_interfaces__srv__SetExclusionArea_Request__Sequence__init(member, size);
}

size_t ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__size_function__SetExclusionArea_Event__response(
  const void * untyped_member)
{
  const ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence * member =
    (const ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Event__response(
  const void * untyped_member, size_t index)
{
  const ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence * member =
    (const ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Event__response(
  void * untyped_member, size_t index)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence * member =
    (ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__fetch_function__SetExclusionArea_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_lam_interfaces__srv__SetExclusionArea_Response * item =
    ((const ros2_lam_interfaces__srv__SetExclusionArea_Response *)
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Event__response(untyped_member, index));
  ros2_lam_interfaces__srv__SetExclusionArea_Response * value =
    (ros2_lam_interfaces__srv__SetExclusionArea_Response *)(untyped_value);
  *value = *item;
}

void ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__assign_function__SetExclusionArea_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Response * item =
    ((ros2_lam_interfaces__srv__SetExclusionArea_Response *)
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Event__response(untyped_member, index));
  const ros2_lam_interfaces__srv__SetExclusionArea_Response * value =
    (const ros2_lam_interfaces__srv__SetExclusionArea_Response *)(untyped_value);
  *item = *value;
}

bool ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__resize_function__SetExclusionArea_Event__response(
  void * untyped_member, size_t size)
{
  ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence * member =
    (ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence *)(untyped_member);
  ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence__fini(member);
  return ros2_lam_interfaces__srv__SetExclusionArea_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_lam_interfaces__srv__SetExclusionArea_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros2_lam_interfaces__srv__SetExclusionArea_Event, request),  // bytes offset in struct
    NULL,  // default value
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__size_function__SetExclusionArea_Event__request,  // size() function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Event__request,  // get_const(index) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Event__request,  // get(index) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__fetch_function__SetExclusionArea_Event__request,  // fetch(index, &value) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__assign_function__SetExclusionArea_Event__request,  // assign(index, value) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__resize_function__SetExclusionArea_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros2_lam_interfaces__srv__SetExclusionArea_Event, response),  // bytes offset in struct
    NULL,  // default value
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__size_function__SetExclusionArea_Event__response,  // size() function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_const_function__SetExclusionArea_Event__response,  // get_const(index) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__get_function__SetExclusionArea_Event__response,  // get(index) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__fetch_function__SetExclusionArea_Event__response,  // fetch(index, &value) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__assign_function__SetExclusionArea_Event__response,  // assign(index, value) function pointer
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__resize_function__SetExclusionArea_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_members = {
  "ros2_lam_interfaces__srv",  // message namespace
  "SetExclusionArea_Event",  // message name
  3,  // number of fields
  sizeof(ros2_lam_interfaces__srv__SetExclusionArea_Event),
  false,  // has_any_key_member_
  ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_member_array,  // message members
  ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_type_support_handle = {
  0,
  &ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_members,
  get_message_typesupport_handle_function,
  &ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_hash,
  &ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_description,
  &ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_lam_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Event)() {
  ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Request)();
  ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Response)();
  if (!ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_type_support_handle.typesupport_identifier) {
    ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ros2_lam_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "ros2_lam_interfaces/srv/detail/set_exclusion_area__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_members = {
  "ros2_lam_interfaces__srv",  // service namespace
  "SetExclusionArea",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_type_support_handle,
  NULL,  // response message
  // ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle
  NULL  // event_message
  // ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle
};


static rosidl_service_type_support_t ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_type_support_handle = {
  0,
  &ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_members,
  get_service_typesupport_handle_function,
  &ros2_lam_interfaces__srv__SetExclusionArea_Request__rosidl_typesupport_introspection_c__SetExclusionArea_Request_message_type_support_handle,
  &ros2_lam_interfaces__srv__SetExclusionArea_Response__rosidl_typesupport_introspection_c__SetExclusionArea_Response_message_type_support_handle,
  &ros2_lam_interfaces__srv__SetExclusionArea_Event__rosidl_typesupport_introspection_c__SetExclusionArea_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros2_lam_interfaces,
    srv,
    SetExclusionArea
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros2_lam_interfaces,
    srv,
    SetExclusionArea
  ),
  &ros2_lam_interfaces__srv__SetExclusionArea__get_type_hash,
  &ros2_lam_interfaces__srv__SetExclusionArea__get_type_description,
  &ros2_lam_interfaces__srv__SetExclusionArea__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_lam_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea)(void) {
  if (!ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_type_support_handle.typesupport_identifier) {
    ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_lam_interfaces, srv, SetExclusionArea_Event)()->data;
  }

  return &ros2_lam_interfaces__srv__detail__set_exclusion_area__rosidl_typesupport_introspection_c__SetExclusionArea_service_type_support_handle;
}
