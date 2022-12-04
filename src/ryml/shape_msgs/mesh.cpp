#include <Eigen/Core>

#include <geometric_shapes/shape_operations.h>

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
    // optional decoding as a resource URI, e.g. package://my_pkg_name/mesh.stl
    if (n.has_child("resource")) {
        std::string resource;
        n["resource"] >> resource;

        shapes::ShapeMsg shape_msg;
        Eigen::Vector3d scaling{ 1, 1, 1 };

        if (n.has_child("dimensions")) {
            std::vector<double> dimensions;
            n["dimensions"] >> dimensions;

            if (dimensions.size() != 3)
                return false;
            scaling.x() = dimensions[0];
            scaling.y() = dimensions[1];
            scaling.z() = dimensions[2];
        }

        std::unique_ptr<const shapes::Shape> shape =
            std::unique_ptr<const shapes::Shape>(shapes::createMeshFromResource(resource, scaling));
        if (!shape)
            return false;

        shapes::constructMsgFromShape(shape.get(), shape_msg);

        auto& mesh = boost::get<shape_msgs::Mesh>(shape_msg);
        rhs->triangles = mesh.triangles;
        rhs->vertices = mesh.vertices;

    } else {
        if (n.has_child("triangles"))
            n["triangles"] >> rhs->triangles;
        if (n.has_child("vertices"))
            n["vertices"] >> rhs->vertices;
    }
    return true;
}

}  // namespace yml
}  // namespace c4
