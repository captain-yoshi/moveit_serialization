/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, Captain Yoshi
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Captain Yoshi
   Desc: Contains all conversion headers
*/

#pragma once

// std (map, vector, string)
#include <moveit_serialization/ryml/std/std.h>

// ros
#include <moveit_serialization/ryml/ros/duration.h>
#include <moveit_serialization/ryml/ros/time.h>

// xmlrpc
#include <moveit_serialization/ryml/xmlrpc/xmlrpc.h>

// std_msgs
#include <moveit_serialization/ryml/std_msgs/color_rgba.h>
#include <moveit_serialization/ryml/std_msgs/header.h>

// geometry_msgs
#include <moveit_serialization/ryml/geometry_msgs/accel.h>
#include <moveit_serialization/ryml/geometry_msgs/point.h>
#include <moveit_serialization/ryml/geometry_msgs/quaternion.h>
#include <moveit_serialization/ryml/geometry_msgs/vector3.h>
#include <moveit_serialization/ryml/geometry_msgs/pose.h>
#include <moveit_serialization/ryml/geometry_msgs/pose_stamped.h>
#include <moveit_serialization/ryml/geometry_msgs/transform.h>
#include <moveit_serialization/ryml/geometry_msgs/transform_stamped.h>
#include <moveit_serialization/ryml/geometry_msgs/twist.h>
#include <moveit_serialization/ryml/geometry_msgs/wrench.h>

// sensor_msgs
#include <moveit_serialization/ryml/sensor_msgs/joint_state.h>
#include <moveit_serialization/ryml/sensor_msgs/multidof_joint_state.h>

// shape_msgs
#include <moveit_serialization/ryml/shape_msgs/mesh.h>
#include <moveit_serialization/ryml/shape_msgs/mesh_triangle.h>
#include <moveit_serialization/ryml/shape_msgs/plane.h>
#include <moveit_serialization/ryml/shape_msgs/solid_primitive.h>

// object_recognition_msgs
#include <moveit_serialization/ryml/object_recognition_msgs/object_type.h>

// octomap_msgs
#include <moveit_serialization/ryml/octomap_msgs/octomap.h>
#include <moveit_serialization/ryml/octomap_msgs/octomap_with_pose.h>

// trajectory_msgs
#include <moveit_serialization/ryml/trajectory_msgs/joint_trajectory_point.h>
#include <moveit_serialization/ryml/trajectory_msgs/joint_trajectory.h>
#include <moveit_serialization/ryml/trajectory_msgs/multidof_joint_trajectory_point.h>
#include <moveit_serialization/ryml/trajectory_msgs/multidof_joint_trajectory.h>

// control_msgs
#include <moveit_serialization/ryml/control_msgs/gripper_command.h>

// moveit_msgs
#include <moveit_serialization/ryml/moveit_msgs/allowed_collision_entry.h>
#include <moveit_serialization/ryml/moveit_msgs/allowed_collision_matrix.h>
#include <moveit_serialization/ryml/moveit_msgs/attached_collision_object.h>
#include <moveit_serialization/ryml/moveit_msgs/bounding_volume.h>
#include <moveit_serialization/ryml/moveit_msgs/cartesian_point.h>
#include <moveit_serialization/ryml/moveit_msgs/cartesian_trajectory.h>
#include <moveit_serialization/ryml/moveit_msgs/cartesian_trajectory_point.h>
#include <moveit_serialization/ryml/moveit_msgs/collision_object.h>
#include <moveit_serialization/ryml/moveit_msgs/constraints.h>
#include <moveit_serialization/ryml/moveit_msgs/generic_trajectory.h>
#include <moveit_serialization/ryml/moveit_msgs/joint_constraint.h>
#include <moveit_serialization/ryml/moveit_msgs/link_padding.h>
#include <moveit_serialization/ryml/moveit_msgs/link_scale.h>
#include <moveit_serialization/ryml/moveit_msgs/motion_plan_request.h>
#include <moveit_serialization/ryml/moveit_msgs/object_color.h>
#include <moveit_serialization/ryml/moveit_msgs/orientation_constraint.h>
#include <moveit_serialization/ryml/moveit_msgs/planning_scene.h>
#include <moveit_serialization/ryml/moveit_msgs/planning_scene_world.h>
#include <moveit_serialization/ryml/moveit_msgs/position_constraint.h>
#include <moveit_serialization/ryml/moveit_msgs/robot_state.h>
#include <moveit_serialization/ryml/moveit_msgs/robot_trajectory.h>
#include <moveit_serialization/ryml/moveit_msgs/trajectory_constraints.h>
#include <moveit_serialization/ryml/moveit_msgs/visibility_constraint.h>
#include <moveit_serialization/ryml/moveit_msgs/workspace_parameters.h>

// moveit
#include <moveit_serialization/ryml/moveit/collision_detection/collision_request.h>

// WARNING: Do NOT include the `ryml.hpp` header here or the error callback.
