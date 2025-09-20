cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ros2_lam_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Generate service files
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/SetExclusionArea.srv"
  DEPENDENCIES geometry_msgs
  LIBRARY_NAME ${PROJECT_NAME}
)

# Install service files (if needed)
install(
  DIRECTORY srv/
  DESTINATION share/${PROJECT_NAME}/srv
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # Uncomment when copyright and license are added
  # set(ament_cmake_copyright_FOUND TRUE)
  # set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
