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
    std_msgs
  moveit_core
  moveit_visual_tools
  moveit_ros_planning
  moveit_ros_planning_interface
  pluginlib
  geometric_shapes
  pcl_ros
  pcl_conversions
  rosbag
  tf2_ros
  tf2_eigen
  tf2_geometry_msgs
)

add_definitions(-D_LINUX_)

# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/lib/x86_64-linux-gnu/qt5)

# 设置输出的路径
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin" CACHE PATH "Runtime directory" FORCE)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/libs" CACHE PATH "Library directory" FORCE)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/libs" CACHE PATH "Archive directory" FORCE)
file(
        WRITE
        ${CMAKE_CURRENT_SOURCE_DIR}/build/bin/qt.conf
        "[Paths]
#Prefix=../lib/Qt
#Binaries=bin
#Libraries=lib
Plugins=${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qt/plugins
#Imports=imports
#Qml2Imports=qml"
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Network REQUIRED)

SET(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES}
        ${Qt5Network_LIBRARIES} ${Qt5Xml_LIBRARIES} ${Qt5Qml_LIBRARIES})
include_directories(
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS})

#线程
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lpthread -Wl,--no-as-needed  -g3 -Wall")
# 与位置无关
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


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
#generate_messages(DEPENDENCIES
#        std_msgs
#)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES impedance_control
#  CATKIN_DEPENDS roscpp rosmsg rospy
#  DEPENDS system_lib
    CATKIN_DEPENDS
    message_runtime
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

include_directories(
    include
    3rd/include
)

LINK_DIRECTORIES(
        3rd/lib/HsApi
        3rd/lib/boost_linux64)

add_executable(impedance_control
        src/impedance_control.cpp
        src/utils/impedance.cpp
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

add_executable(ros_move_ur5 src/test_move_ur5.cpp)
target_link_libraries(
        ros_move_ur5
        ${catkin_LIBRARIES}
)

add_executable(ur_test_ui src/ur_test_ui.cpp  src/utils/MainWindow.cpp)
target_link_libraries(
        ur_test_ui
        ${catkin_LIBRARIES}
        ${QT_LIBRARIES}

)
