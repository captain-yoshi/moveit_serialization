#include <moveit_serialization/ryml/std/std.h>
#include <moveit_serialization/ryml/geometry_msgs/point.h>
#include <moveit_serialization/ryml/shape_msgs/mesh_triangle.h>

#include <moveit_serialization/ryml/shape_msgs/mesh.h>

namespace c4 {
namespace yml {

void write(c4::yml::NodeRef* n, shape_msgs::Mesh const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("triangles") << rhs.triangles;
    n->append_child() << yml::key("vertices") << rhs.vertices;
}

bool read(c4::yml::ConstNodeRef const& n, shape_msgs::Mesh* rhs)
{
    if (n.has_child("triangles"))
        n["triangles"] >> rhs->triangles;
    if (n.has_child("vertices"))
        n["vertices"] >> rhs->vertices;

    return true;
}

}  // namespace yml
}  // namespace c4
