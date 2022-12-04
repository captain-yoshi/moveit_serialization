#include <moveit_serialization/ryml/format.h>
#include <moveit_serialization/ryml/std/std.h>
#include <moveit_serialization/ryml/std_msgs/header.h>

#include <moveit_serialization/ryml/octomap_msgs/octomap.h>

namespace c4 {
namespace yml {

void write(c4::yml::NodeRef* n, octomap_msgs::Octomap const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("header") << rhs.header;
    n->append_child() << yml::key("binary") << fmt::boolalpha(rhs.binary);
    n->append_child() << yml::key("id") << rhs.id;
    n->append_child() << yml::key("resolution") << freal(rhs.resolution);
    n->append_child() << yml::key("data") << rhs.data;
}

bool read(c4::yml::ConstNodeRef const& n, octomap_msgs::Octomap* rhs)
{
    if (n.has_child("header"))
        n["header"] >> rhs->header;
    if (n.has_child("binary")) {
        bool b;
        n["binary"] >> b;
        rhs->binary = b;
    }
    if (n.has_child("id"))
        n["id"] >> rhs->id;
    if (n.has_child("resolution"))
        n["resolution"] >> rhs->resolution;
    if (n.has_child("data"))
        n["data"] >> rhs->data;

    return true;
}

}  // namespace yml
}  // namespace c4
