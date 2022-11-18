cmake_minimum_required(VERSION 3.1.3)
project(moveit_serialization)

find_package(Boost REQUIRED iostreams)
find_package(Eigen3 REQUIRED)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	moveit_core
	moveit_msgs
)

# ryml internal library includes
set(ryml_INCLUDE_DIRS ext/ryml/src ext/ryml/ext/c4core/src)

catkin_package(
	LIBRARIES
    	${PROJECT_NAME}
        ryml
	INCLUDE_DIRS
		include
        ${ryml_INCLUDE_DIRS}
	CATKIN_DEPENDS
	    roscpp
	    moveit_core
		moveit_msgs
	DEPENDS
	    Boost
		EIGEN3
)

# add ryml as an internal library
# https://github.com/captain-yoshi/rapidyaml/commit/e9ec9fcf5e6f34e5f44e48f1c1038cbb7e38b285
add_subdirectory(ext/ryml ryml)

include_directories(include
	${Boost_INCLUDE_DIRS}
	${EIGEN3_INCLUDE_DIRS}
    ${ryml_INCLUDE_DIRS}
	${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
	# ros
	src/ryml/ros/duration.cpp
	src/ryml/ros/time.cpp

    # xmlrpc
    src/ryml/xmlrpc/xmlrpc.cpp

	# std_msgs
	src/ryml/std_msgs/color_rgba.cpp
	src/ryml/std_msgs/header.cpp

	# geometry_msgs
	src/ryml/geometry_msgs/accel.cpp
	src/ryml/geometry_msgs/vector3.cpp
	src/ryml/geometry_msgs/point.cpp
	src/ryml/geometry_msgs/quaternion.cpp
	src/ryml/geometry_msgs/pose.cpp
	src/ryml/geometry_msgs/transform.cpp
	src/ryml/geometry_msgs/twist.cpp
	src/ryml/geometry_msgs/wrench.cpp
	src/ryml/geometry_msgs/pose_stamped.cpp
	src/ryml/geometry_msgs/transform_stamped.cpp

	# sensor_msgs
	src/ryml/sensor_msgs/joint_state.cpp
	src/ryml/sensor_msgs/multidof_joint_state.cpp

	# shape_msgs
	src/ryml/shape_msgs/mesh.cpp
	src/ryml/shape_msgs/mesh_triangle.cpp
	src/ryml/shape_msgs/plane.cpp
	src/ryml/shape_msgs/solid_primitive.cpp

	# object_recognition_msgs
	src/ryml/object_recognition_msgs/object_type.cpp

	# octomap_msgs
	src/ryml/octomap_msgs/octomap.cpp
	src/ryml/octomap_msgs/octomap_with_pose.cpp

	# trajectory_msgs
	src/ryml/trajectory_msgs/joint_trajectory.cpp
	src/ryml/trajectory_msgs/joint_trajectory_point.cpp
	src/ryml/trajectory_msgs/multidof_joint_trajectory.cpp
	src/ryml/trajectory_msgs/multidof_joint_trajectory_point.cpp

	# moveit_msgs
    src/ryml/moveit_msgs/allowed_collision_entry.cpp
    src/ryml/moveit_msgs/allowed_collision_matrix.cpp
    src/ryml/moveit_msgs/attached_collision_object.cpp
    src/ryml/moveit_msgs/bounding_volume.cpp
    src/ryml/moveit_msgs/cartesian_point.cpp
    src/ryml/moveit_msgs/cartesian_trajectory.cpp
    src/ryml/moveit_msgs/cartesian_trajectory_point.cpp
    src/ryml/moveit_msgs/collision_object.cpp
    src/ryml/moveit_msgs/constraints.cpp
    src/ryml/moveit_msgs/generic_trajectory.cpp
    src/ryml/moveit_msgs/joint_constraint.cpp
    src/ryml/moveit_msgs/link_padding.cpp
    src/ryml/moveit_msgs/link_scale.cpp
    src/ryml/moveit_msgs/motion_plan_request.cpp
    src/ryml/moveit_msgs/object_color.cpp
    src/ryml/moveit_msgs/orientation_constraint.cpp
    src/ryml/moveit_msgs/planning_scene.cpp
    src/ryml/moveit_msgs/planning_scene_world.cpp
    src/ryml/moveit_msgs/position_constraint.cpp
    src/ryml/moveit_msgs/robot_state.cpp
    src/ryml/moveit_msgs/robot_trajectory.cpp
    src/ryml/moveit_msgs/trajectory_constraints.cpp
    src/ryml/moveit_msgs/visibility_constraint.cpp
    src/ryml/moveit_msgs/workspace_parameters.cpp

	# moveit
	src/ryml/moveit/collision_detection/collision_request.cpp

    src/ryml/utils.cpp
    src/ryml/error_handler.cpp
)

add_executable(ryml-quickstart ext/ryml/samples/quickstart.cpp)
target_link_libraries(ryml-quickstart ${catkin_LIBRARIES} ryml::ryml)

install(TARGETS
    ${PROJECT_NAME}
    ryml-quickstart
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
