#include <moveit_serialization/ryml/format.h>
#include <moveit_serialization/ryml/geometry_msgs/quaternion.h>

namespace c4 {
namespace yml {

void write(c4::yml::NodeRef* n, geometry_msgs::Quaternion const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("x") << freal(rhs.x);
    n->append_child() << yml::key("y") << freal(rhs.y);
    n->append_child() << yml::key("z") << freal(rhs.z);
    n->append_child() << yml::key("w") << freal(rhs.w);
}

bool read(c4::yml::NodeRef const& n, geometry_msgs::Quaternion* rhs)
{
    n["x"] >> rhs->x;
    n["y"] >> rhs->y;
    n["z"] >> rhs->z;
    n["w"] >> rhs->w;

    return true;
}

}  // namespace yml
}  // namespace c4
