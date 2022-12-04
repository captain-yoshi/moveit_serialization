#include <moveit_serialization/ryml/format.h>
#include <moveit_serialization/ryml/control_msgs/gripper_command.h>

namespace c4 {
namespace yml {

void write(c4::yml::NodeRef* n, control_msgs::GripperCommand const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("position") << freal(rhs.position);
    n->append_child() << yml::key("max_effort") << freal(rhs.max_effort);
}

bool read(c4::yml::ConstNodeRef const& n, control_msgs::GripperCommand* rhs)
{
    if (n.has_child("position"))
        n["position"] >> rhs->position;
    if (n.has_child("max_effort"))
        n["max_effort"] >> rhs->max_effort;

    return true;
}

}  // namespace yml
}  // namespace c4
