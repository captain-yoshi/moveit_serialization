#include <moveit_serialization/ryml/std/std.h>
#include <moveit_serialization/ryml/sensor_msgs/joint_state.h>
#include <moveit_serialization/ryml/sensor_msgs/multidof_joint_state.h>
#include <moveit_serialization/ryml/moveit_msgs/attached_collision_object.h>

#include <moveit_serialization/ryml/moveit_msgs/robot_state.h>

namespace c4 {
namespace yml {

void write(c4::yml::NodeRef* n, moveit_msgs::RobotState const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("joint_state") << rhs.joint_state;
    n->append_child() << yml::key("multi_dof_joint_state") << rhs.multi_dof_joint_state;
    n->append_child() << yml::key("attached_collision_objects") << rhs.attached_collision_objects;
    n->append_child() << yml::key("is_diff") << fmt::boolalpha(rhs.is_diff);
}

bool read(c4::yml::ConstNodeRef const& n, moveit_msgs::RobotState* rhs)
{
    if (n.has_child("joint_state"))
        n["joint_state"] >> rhs->joint_state;
    if (n.has_child("multi_dof_joint_state"))
        n["multi_dof_joint_state"] >> rhs->multi_dof_joint_state;
    if (n.has_child("attached_collision_objects"))
        n["attached_collision_objects"] >> rhs->attached_collision_objects;
    if (n.has_child("is_diff")) {
        bool b;
        n["is_diff"] >> b;
        rhs->is_diff = b;
    }

    return true;
}

}  // namespace yml
}  // namespace c4
