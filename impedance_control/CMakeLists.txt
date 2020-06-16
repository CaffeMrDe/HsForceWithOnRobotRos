cmake_minimum_required(VERSION 3.0.2)
project(impedance_control)

## Compile as C++11, supported in ROS Kinetic and newer
 add_compile_options(-std=c++14)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosmsg
  rospy
)

add_definitions(-D_LINUX_)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES impedance_control
#  CATKIN_DEPENDS roscpp rosmsg rospy
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
 include
  ${catkin_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
    ~/catkin_ws/src/HsForceWithOnRobotRos/impedance_control/3rd/lib/HsApi
    ~/catkin_ws/src/HsForceWithOnRobotRos/impedance_control/3rd/lib/boost_linux64)
include_directories(
    include
    ~/catkin_ws/src/HsForceWithOnRobotRos/impedance_control/3rd/include
)

add_executable(impedance_control
        src/impedance_control.cpp
        src/utils/impedance.cpp
        src/utils/timerTask.cpp
        src/utils/Ctimer.cpp
        src/robot/hsRobotUtil.cpp
        )
 target_link_libraries(impedance_control
   ${catkin_LIBRARIES}
   CommApi
   Hsc3Api
   boost_chrono
   boost_system
   boost_thread
   boost_date_time
 )
