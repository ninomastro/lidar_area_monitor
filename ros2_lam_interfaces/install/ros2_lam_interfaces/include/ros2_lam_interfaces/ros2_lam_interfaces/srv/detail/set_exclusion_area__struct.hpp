// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ros2_lam_interfaces:srv/SetExclusionArea.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros2_lam_interfaces/srv/set_exclusion_area.hpp"


#ifndef ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__STRUCT_HPP_
#define ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'point_list'
#include "geometry_msgs/msg/detail/point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Request __attribute__((deprecated))
#else
# define DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Request __declspec(deprecated)
#endif

namespace ros2_lam_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetExclusionArea_Request_
{
  using Type = SetExclusionArea_Request_<ContainerAllocator>;

  explicit SetExclusionArea_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit SetExclusionArea_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _point_list_type =
    std::vector<geometry_msgs::msg::Point_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<geometry_msgs::msg::Point_<ContainerAllocator>>>;
  _point_list_type point_list;

  // setters for named parameter idiom
  Type & set__point_list(
    const std::vector<geometry_msgs::msg::Point_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<geometry_msgs::msg::Point_<ContainerAllocator>>> & _arg)
  {
    this->point_list = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Request
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Request
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetExclusionArea_Request_ & other) const
  {
    if (this->point_list != other.point_list) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetExclusionArea_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetExclusionArea_Request_

// alias to use template instance with default allocator
using SetExclusionArea_Request =
  ros2_lam_interfaces::srv::SetExclusionArea_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ros2_lam_interfaces


#ifndef _WIN32
# define DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Response __attribute__((deprecated))
#else
# define DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Response __declspec(deprecated)
#endif

namespace ros2_lam_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetExclusionArea_Response_
{
  using Type = SetExclusionArea_Response_<ContainerAllocator>;

  explicit SetExclusionArea_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->is_ok = false;
      this->error_msg = "";
    }
  }

  explicit SetExclusionArea_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : error_msg(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->is_ok = false;
      this->error_msg = "";
    }
  }

  // field types and members
  using _is_ok_type =
    bool;
  _is_ok_type is_ok;
  using _error_msg_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _error_msg_type error_msg;

  // setters for named parameter idiom
  Type & set__is_ok(
    const bool & _arg)
  {
    this->is_ok = _arg;
    return *this;
  }
  Type & set__error_msg(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->error_msg = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Response
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Response
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetExclusionArea_Response_ & other) const
  {
    if (this->is_ok != other.is_ok) {
      return false;
    }
    if (this->error_msg != other.error_msg) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetExclusionArea_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetExclusionArea_Response_

// alias to use template instance with default allocator
using SetExclusionArea_Response =
  ros2_lam_interfaces::srv::SetExclusionArea_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ros2_lam_interfaces


// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Event __attribute__((deprecated))
#else
# define DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Event __declspec(deprecated)
#endif

namespace ros2_lam_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetExclusionArea_Event_
{
  using Type = SetExclusionArea_Event_<ContainerAllocator>;

  explicit SetExclusionArea_Event_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_init)
  {
    (void)_init;
  }

  explicit SetExclusionArea_Event_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _info_type =
    service_msgs::msg::ServiceEventInfo_<ContainerAllocator>;
  _info_type info;
  using _request_type =
    rosidl_runtime_cpp::BoundedVector<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>>;
  _request_type request;
  using _response_type =
    rosidl_runtime_cpp::BoundedVector<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>>;
  _response_type response;

  // setters for named parameter idiom
  Type & set__info(
    const service_msgs::msg::ServiceEventInfo_<ContainerAllocator> & _arg)
  {
    this->info = _arg;
    return *this;
  }
  Type & set__request(
    const rosidl_runtime_cpp::BoundedVector<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros2_lam_interfaces::srv::SetExclusionArea_Request_<ContainerAllocator>>> & _arg)
  {
    this->request = _arg;
    return *this;
  }
  Type & set__response(
    const rosidl_runtime_cpp::BoundedVector<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros2_lam_interfaces::srv::SetExclusionArea_Response_<ContainerAllocator>>> & _arg)
  {
    this->response = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Event
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros2_lam_interfaces__srv__SetExclusionArea_Event
    std::shared_ptr<ros2_lam_interfaces::srv::SetExclusionArea_Event_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetExclusionArea_Event_ & other) const
  {
    if (this->info != other.info) {
      return false;
    }
    if (this->request != other.request) {
      return false;
    }
    if (this->response != other.response) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetExclusionArea_Event_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetExclusionArea_Event_

// alias to use template instance with default allocator
using SetExclusionArea_Event =
  ros2_lam_interfaces::srv::SetExclusionArea_Event_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ros2_lam_interfaces

namespace ros2_lam_interfaces
{

namespace srv
{

struct SetExclusionArea
{
  using Request = ros2_lam_interfaces::srv::SetExclusionArea_Request;
  using Response = ros2_lam_interfaces::srv::SetExclusionArea_Response;
  using Event = ros2_lam_interfaces::srv::SetExclusionArea_Event;
};

}  // namespace srv

}  // namespace ros2_lam_interfaces

#endif  // ROS2_LAM_INTERFACES__SRV__DETAIL__SET_EXCLUSION_AREA__STRUCT_HPP_
