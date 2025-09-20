// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ros2_lam_interfaces:srv/SetExclusionArea.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros2_lam_interfaces/srv/set_exclusion_area.hpp"


#ifndef ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__TRAITS_HPP_
#define ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ros2_lam_interfaces/srv/detail/set_exclusion_area__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'point_list'
#include "geometry_msgs/msg/detail/point__traits.hpp"

namespace ros2_lam_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetExclusionArea_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: point_list
  {
    if (msg.point_list.size() == 0) {
      out << "point_list: []";
    } else {
      out << "point_list: [";
      size_t pending_items = msg.point_list.size();
      for (auto item : msg.point_list) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetExclusionArea_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: point_list
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.point_list.size() == 0) {
      out << "point_list: []\n";
    } else {
      out << "point_list:\n";
      for (auto item : msg.point_list) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetExclusionArea_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace ros2_lam_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use ros2_lam_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ros2_lam_interfaces::srv::SetExclusionArea_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  ros2_lam_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ros2_lam_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ros2_lam_interfaces::srv::SetExclusionArea_Request & msg)
{
  return ros2_lam_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ros2_lam_interfaces::srv::SetExclusionArea_Request>()
{
  return "ros2_lam_interfaces::srv::SetExclusionArea_Request";
}

template<>
inline const char * name<ros2_lam_interfaces::srv::SetExclusionArea_Request>()
{
  return "ros2_lam_interfaces/srv/SetExclusionArea_Request";
}

template<>
struct has_fixed_size<ros2_lam_interfaces::srv::SetExclusionArea_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<ros2_lam_interfaces::srv::SetExclusionArea_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace ros2_lam_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetExclusionArea_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: is_ok
  {
    out << "is_ok: ";
    rosidl_generator_traits::value_to_yaml(msg.is_ok, out);
    out << ", ";
  }

  // member: error_msg
  {
    out << "error_msg: ";
    rosidl_generator_traits::value_to_yaml(msg.error_msg, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetExclusionArea_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: is_ok
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_ok: ";
    rosidl_generator_traits::value_to_yaml(msg.is_ok, out);
    out << "\n";
  }

  // member: error_msg
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "error_msg: ";
    rosidl_generator_traits::value_to_yaml(msg.error_msg, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetExclusionArea_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace ros2_lam_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use ros2_lam_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ros2_lam_interfaces::srv::SetExclusionArea_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  ros2_lam_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ros2_lam_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ros2_lam_interfaces::srv::SetExclusionArea_Response & msg)
{
  return ros2_lam_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ros2_lam_interfaces::srv::SetExclusionArea_Response>()
{
  return "ros2_lam_interfaces::srv::SetExclusionArea_Response";
}

template<>
inline const char * name<ros2_lam_interfaces::srv::SetExclusionArea_Response>()
{
  return "ros2_lam_interfaces/srv/SetExclusionArea_Response";
}

template<>
struct has_fixed_size<ros2_lam_interfaces::srv::SetExclusionArea_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<ros2_lam_interfaces::srv::SetExclusionArea_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace ros2_lam_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetExclusionArea_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetExclusionArea_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetExclusionArea_Event & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace ros2_lam_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use ros2_lam_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ros2_lam_interfaces::srv::SetExclusionArea_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  ros2_lam_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ros2_lam_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ros2_lam_interfaces::srv::SetExclusionArea_Event & msg)
{
  return ros2_lam_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ros2_lam_interfaces::srv::SetExclusionArea_Event>()
{
  return "ros2_lam_interfaces::srv::SetExclusionArea_Event";
}

template<>
inline const char * name<ros2_lam_interfaces::srv::SetExclusionArea_Event>()
{
  return "ros2_lam_interfaces/srv/SetExclusionArea_Event";
}

template<>
struct has_fixed_size<ros2_lam_interfaces::srv::SetExclusionArea_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Event>
  : std::integral_constant<bool, has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Request>::value && has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Response>::value && has_bounded_size<service_msgs::msg::ServiceEventInfo>::value> {};

template<>
struct is_message<ros2_lam_interfaces::srv::SetExclusionArea_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<ros2_lam_interfaces::srv::SetExclusionArea>()
{
  return "ros2_lam_interfaces::srv::SetExclusionArea";
}

template<>
inline const char * name<ros2_lam_interfaces::srv::SetExclusionArea>()
{
  return "ros2_lam_interfaces/srv/SetExclusionArea";
}

template<>
struct has_fixed_size<ros2_lam_interfaces::srv::SetExclusionArea>
  : std::integral_constant<
    bool,
    has_fixed_size<ros2_lam_interfaces::srv::SetExclusionArea_Request>::value &&
    has_fixed_size<ros2_lam_interfaces::srv::SetExclusionArea_Response>::value
  >
{
};

template<>
struct has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea>
  : std::integral_constant<
    bool,
    has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Request>::value &&
    has_bounded_size<ros2_lam_interfaces::srv::SetExclusionArea_Response>::value
  >
{
};

template<>
struct is_service<ros2_lam_interfaces::srv::SetExclusionArea>
  : std::true_type
{
};

template<>
struct is_service_request<ros2_lam_interfaces::srv::SetExclusionArea_Request>
  : std::true_type
{
};

template<>
struct is_service_response<ros2_lam_interfaces::srv::SetExclusionArea_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__TRAITS_HPP_
