cmake_minimum_required(VERSION 2.8.3)
project(radar_camera_fusion)

set(OpenCV_DIR /opt/ros/kinetic/share/OpenCV-3.3.1-dev)
#set(OpenCV_DIR /usr/share/OpenCV)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## is used, also find other catkin packages
find_package(OpenCV REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  conti_radar_msgs
  cv_bridge
  image_transport
)
 
catkin_package(
  #INCLUDE_DIRS include
  #LIBRARIES
  #CATKIN_DEPENDS #roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  src/
  include/
)

## Declare a C++ library
add_library(${PROJECT_NAME}
   src/radar_camera_fusion.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)

## The recommended prefix ensures that target names across packages don't collide
 add_executable(${PROJECT_NAME}_node tool/radar_camera_fusion_node.cpp)

## same as for the library above
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  ${PROJECT_NAME}
#  libboost
)

message(STATUS "opencv version: ${OpenCV_VERSION}")
message(STATUS "opencv include: ${OpenCV_INCLUDE_DIRS}")

#############
## Install ##
#############
## Mark executables and/or libraries for installation
#install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#)

