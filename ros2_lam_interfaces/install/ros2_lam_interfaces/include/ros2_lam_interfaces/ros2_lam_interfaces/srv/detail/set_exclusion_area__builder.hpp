// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros2_lam_interfaces:srv/SetExclusionArea.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros2_lam_interfaces/srv/set_exclusion_area.hpp"


#ifndef ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__BUILDER_HPP_
#define ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ros2_lam_interfaces/srv/detail/set_exclusion_area__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ros2_lam_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetExclusionArea_Request_point_list
{
public:
  Init_SetExclusionArea_Request_point_list()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ros2_lam_interfaces::srv::SetExclusionArea_Request point_list(::ros2_lam_interfaces::srv::SetExclusionArea_Request::_point_list_type arg)
  {
    msg_.point_list = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros2_lam_interfaces::srv::SetExclusionArea_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros2_lam_interfaces::srv::SetExclusionArea_Request>()
{
  return ros2_lam_interfaces::srv::builder::Init_SetExclusionArea_Request_point_list();
}

}  // namespace ros2_lam_interfaces


namespace ros2_lam_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetExclusionArea_Response_error_msg
{
public:
  explicit Init_SetExclusionArea_Response_error_msg(::ros2_lam_interfaces::srv::SetExclusionArea_Response & msg)
  : msg_(msg)
  {}
  ::ros2_lam_interfaces::srv::SetExclusionArea_Response error_msg(::ros2_lam_interfaces::srv::SetExclusionArea_Response::_error_msg_type arg)
  {
    msg_.error_msg = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros2_lam_interfaces::srv::SetExclusionArea_Response msg_;
};

class Init_SetExclusionArea_Response_is_ok
{
public:
  Init_SetExclusionArea_Response_is_ok()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetExclusionArea_Response_error_msg is_ok(::ros2_lam_interfaces::srv::SetExclusionArea_Response::_is_ok_type arg)
  {
    msg_.is_ok = std::move(arg);
    return Init_SetExclusionArea_Response_error_msg(msg_);
  }

private:
  ::ros2_lam_interfaces::srv::SetExclusionArea_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros2_lam_interfaces::srv::SetExclusionArea_Response>()
{
  return ros2_lam_interfaces::srv::builder::Init_SetExclusionArea_Response_is_ok();
}

}  // namespace ros2_lam_interfaces


namespace ros2_lam_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetExclusionArea_Event_response
{
public:
  explicit Init_SetExclusionArea_Event_response(::ros2_lam_interfaces::srv::SetExclusionArea_Event & msg)
  : msg_(msg)
  {}
  ::ros2_lam_interfaces::srv::SetExclusionArea_Event response(::ros2_lam_interfaces::srv::SetExclusionArea_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros2_lam_interfaces::srv::SetExclusionArea_Event msg_;
};

class Init_SetExclusionArea_Event_request
{
public:
  explicit Init_SetExclusionArea_Event_request(::ros2_lam_interfaces::srv::SetExclusionArea_Event & msg)
  : msg_(msg)
  {}
  Init_SetExclusionArea_Event_response request(::ros2_lam_interfaces::srv::SetExclusionArea_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_SetExclusionArea_Event_response(msg_);
  }

private:
  ::ros2_lam_interfaces::srv::SetExclusionArea_Event msg_;
};

class Init_SetExclusionArea_Event_info
{
public:
  Init_SetExclusionArea_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetExclusionArea_Event_request info(::ros2_lam_interfaces::srv::SetExclusionArea_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_SetExclusionArea_Event_request(msg_);
  }

private:
  ::ros2_lam_interfaces::srv::SetExclusionArea_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros2_lam_interfaces::srv::SetExclusionArea_Event>()
{
  return ros2_lam_interfaces::srv::builder::Init_SetExclusionArea_Event_info();
}

}  // namespace ros2_lam_interfaces

#endif  // ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__BUILDER_HPP_
