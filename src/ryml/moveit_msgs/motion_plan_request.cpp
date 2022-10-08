#include <moveit_serialization/ryml/std/std.h>
#include <moveit_serialization/ryml/format.h>
#include <moveit_serialization/ryml/moveit_msgs/workspace_parameters.h>
#include <moveit_serialization/ryml/moveit_msgs/robot_state.h>
#include <moveit_serialization/ryml/moveit_msgs/constraints.h>
#include <moveit_serialization/ryml/moveit_msgs/trajectory_constraints.h>
#include <moveit_serialization/ryml/moveit_msgs/generic_trajectory.h>

#include <moveit_serialization/ryml/moveit_msgs/motion_plan_request.h>

namespace c4 {
namespace yml {

namespace {

// SFINAE to extract max_cartesian_speed
template <typename, typename = void> constexpr bool has_max_cartesian_speed = false;
template <typename T> constexpr bool has_max_cartesian_speed<T,
    std::void_t< decltype(std::declval<T>().max_cartesian_speed) >
> = true;

template <typename T>
double& max_cartesian_speed_helper(T& motion_plan_request, std::true_type){
  return motion_plan_request.max_cartesian_speed;
};

template <typename T>
double& max_cartesian_speed_helper(T& motion_plan_request, std::false_type){
    std::runtime_error("moveit_msgs::MotionPlanRequest does not have the 'max_cartesian_speed' parameter");

    // silence warning
    static double dummy = 0.0;
    return dummy;
};

template <typename T>
double& max_cartesian_speed(T&& motion_plan_request){
  return max_cartesian_speed_helper(
    const_cast<std::remove_const_t<std::remove_reference_t<T>>&>(motion_plan_request),
    std::integral_constant<bool, has_max_cartesian_speed<T>>{}
  );
};


// SFINAE to extract either cartesian_speed_limited_link or (previous name) cartesian_speed_end_effector_link
template <typename, typename = void> constexpr bool has_limited_link = false;
template <typename T> constexpr bool has_limited_link<T,
    std::void_t< decltype(std::declval<T>().cartesian_speed_limited_link) >
> = true;

template <typename T>
std::string& cartesian_speed_limited_link_helper(T& motion_plan_request, std::true_type){
  return motion_plan_request.cartesian_speed_limited_link;
};

template <typename T>
std::string& cartesian_speed_limited_link_helper(T& motion_plan_request, std::false_type){
  return motion_plan_request.cartesian_speed_end_effector_link;
};

template <typename T>
std::string& cartesian_speed_limited_link(T&& motion_plan_request){
  return cartesian_speed_limited_link_helper(
    const_cast<std::remove_const_t<std::remove_reference_t<T>>&>(motion_plan_request),
    std::integral_constant<bool, has_limited_link<T>>{}
  );
};

template <typename T>
std::string cartesian_speed_limited_link_key(T& motion_plan_request){
  if(has_limited_link<T>)
      return "cartesian_speed_limited_link";
  else
      return "cartesian_speed_end_effector_link";
};

}

void write(c4::yml::NodeRef* n, moveit_msgs::MotionPlanRequest const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("workspace_parameters") << rhs.workspace_parameters;
    n->append_child() << yml::key("start_state") << rhs.start_state;
    n->append_child() << yml::key("goal_constraints") << rhs.goal_constraints;
    n->append_child() << yml::key("path_constraints") << rhs.path_constraints;
    n->append_child() << yml::key("trajectory_constraints") << rhs.trajectory_constraints;
    n->append_child() << yml::key("reference_trajectories") << rhs.reference_trajectories;
    n->append_child() << yml::key("pipeline_id") << rhs.pipeline_id;
    n->append_child() << yml::key("planner_id") << rhs.planner_id;
    n->append_child() << yml::key("group_name") << rhs.group_name;
    n->append_child() << yml::key("num_planning_attempts") << rhs.num_planning_attempts;
    n->append_child() << yml::key("allowed_planning_time") << freal(rhs.allowed_planning_time);
    n->append_child() << yml::key("max_velocity_scaling_factor") << freal(rhs.max_velocity_scaling_factor);
    n->append_child() << yml::key("max_acceleration_scaling_factor") << freal(rhs.max_acceleration_scaling_factor);

    if(has_max_cartesian_speed<moveit_msgs::MotionPlanRequest>) {
        const std::string cartesian_speed_link_key = cartesian_speed_limited_link_key(rhs);

        n->append_child() << yml::key(cartesian_speed_link_key) << cartesian_speed_limited_link(rhs);
        n->append_child() << yml::key("max_cartesian_speed") << freal(max_cartesian_speed(rhs));
    }
}

bool read(c4::yml::ConstNodeRef const& n, moveit_msgs::MotionPlanRequest* rhs)
{
    if (n.has_child("workspace_parameters"))
        n["workspace_parameters"] >> rhs->workspace_parameters;
    if (n.has_child("start_state"))
        n["start_state"] >> rhs->start_state;
    if (n.has_child("goal_constraints"))
        n["goal_constraints"] >> rhs->goal_constraints;
    if (n.has_child("path_constraints"))
        n["path_constraints"] >> rhs->path_constraints;
    if (n.has_child("trajectory_constraints"))
        n["trajectory_constraints"] >> rhs->trajectory_constraints;
    if (n.has_child("reference_trajectories"))
        n["reference_trajectories"] >> rhs->reference_trajectories;
    if (n.has_child("pipeline_id"))
        n["pipeline_id"] >> rhs->pipeline_id;
    if (n.has_child("planner_id"))
        n["planner_id"] >> rhs->planner_id;
    if (n.has_child("group_name"))
        n["group_name"] >> rhs->group_name;
    if (n.has_child("num_planning_attempts"))
        n["num_planning_attempts"] >> rhs->num_planning_attempts;
    if (n.has_child("allowed_planning_time"))
        n["allowed_planning_time"] >> rhs->allowed_planning_time;
    if (n.has_child("max_velocity_scaling_factor"))
        n["max_velocity_scaling_factor"] >> rhs->max_velocity_scaling_factor;
    if (n.has_child("max_acceleration_scaling_factor"))
        n["max_acceleration_scaling_factor"] >> rhs->max_acceleration_scaling_factor;

    if(has_max_cartesian_speed<moveit_msgs::MotionPlanRequest>) {
        if (n.has_child("max_cartesian_speed"))
            n["max_cartesian_speed"] >> max_cartesian_speed(*rhs);
        // support old message naming
        if (n.has_child("cartesian_speed_end_effector_link"))
            n["cartesian_speed_end_effector_link"] >> cartesian_speed_limited_link(*rhs);
        if (n.has_child("cartesian_speed_limited_link"))
            n["cartesian_speed_limited_link"] >> cartesian_speed_limited_link(*rhs);
    }

    return true;
}

}  // namespace yml
}  // namespace c4
