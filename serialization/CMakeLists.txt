cmake_minimum_required(VERSION 3.1.3)
project(moveit_serialization)

find_package(Boost REQUIRED iostreams)
find_package(Eigen3 REQUIRED)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	moveit_core
	moveit_msgs
	moveit_serialization_yamlcpp
)

catkin_package(
	LIBRARIES
    	${PROJECT_NAME}
	INCLUDE_DIRS
		include
	CATKIN_DEPENDS
	    roscpp
	    moveit_core
		moveit_msgs
	    moveit_serialization_yamlcpp
	DEPENDS
	    Boost
		EIGEN3
)

set(CMAKE_CXX_STANDARD 14)

include_directories(include
	${Boost_INCLUDE_DIRS}
	${EIGEN3_INCLUDE_DIRS}
	${catkin_INCLUDE_DIRS})

add_library(${PROJECT_NAME}
	src/yaml-cpp/node_manipulation.cpp
	src/yaml-cpp/filter.cpp
	src/yaml-cpp/conversion/utilities.cpp
	src/yaml-cpp/conversion/ros.cpp
	src/yaml-cpp/conversion/std_msgs.cpp
	src/yaml-cpp/conversion/geometry_msgs.cpp
	src/yaml-cpp/conversion/shape_msgs.cpp
	src/yaml-cpp/conversion/sensor_msgs.cpp
	src/yaml-cpp/conversion/octomap_msgs.cpp
	src/yaml-cpp/conversion/trajectory_msgs.cpp
	src/yaml-cpp/conversion/collision_detection.cpp
	src/yaml-cpp/conversion/object_recognition_msgs.cpp
	src/yaml-cpp/conversion/moveit_msgs.cpp)

add_executable(run_example src/example.cpp)
target_link_libraries(run_example ${PROJECT_NAME} ${catkin_LIBRARIES} ${Boost_LIBRARIES})

install(DIRECTORY include/
	DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION})

install(TARGETS run_example ${PROJECT_NAME}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
