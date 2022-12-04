#include <moveit_serialization/ryml/std/std.h>
#include <moveit_serialization/ryml/geometry_msgs/transform_stamped.h>
#include <moveit_serialization/ryml/moveit_msgs/robot_state.h>
#include <moveit_serialization/ryml/moveit_msgs/allowed_collision_matrix.h>
#include <moveit_serialization/ryml/moveit_msgs/link_padding.h>
#include <moveit_serialization/ryml/moveit_msgs/link_scale.h>
#include <moveit_serialization/ryml/moveit_msgs/object_color.h>
#include <moveit_serialization/ryml/moveit_msgs/planning_scene_world.h>

#include <moveit_serialization/ryml/moveit_msgs/planning_scene.h>

namespace c4 {
namespace yml {

void write(c4::yml::NodeRef* n, moveit_msgs::PlanningScene const& rhs)
{
    *n |= c4::yml::MAP;

    n->append_child() << yml::key("name") << rhs.name;
    n->append_child() << yml::key("robot_state") << rhs.robot_state;
    n->append_child() << yml::key("robot_model_name") << rhs.robot_model_name;
    n->append_child() << yml::key("fixed_frame_transforms") << rhs.fixed_frame_transforms;
    n->append_child() << yml::key("allowed_collision_matrix") << rhs.allowed_collision_matrix;
    n->append_child() << yml::key("link_padding") << rhs.link_padding;
    n->append_child() << yml::key("link_scale") << rhs.link_scale;
    n->append_child() << yml::key("object_colors") << rhs.object_colors;
    n->append_child() << yml::key("world") << rhs.world;
    n->append_child() << yml::key("is_diff") << fmt::boolalpha(rhs.is_diff);
}

bool read(c4::yml::ConstNodeRef const& n, moveit_msgs::PlanningScene* rhs)
{
    if (n.has_child("name"))
        n["name"] >> rhs->name;
    if (n.has_child("robot_state"))
        n["robot_state"] >> rhs->robot_state;
    if (n.has_child("robot_model_name"))
        n["robot_model_name"] >> rhs->robot_model_name;
    if (n.has_child("fixed_frame_transforms"))
        n["fixed_frame_transforms"] >> rhs->fixed_frame_transforms;
    if (n.has_child("allowed_collision_matrix"))
        n["allowed_collision_matrix"] >> rhs->allowed_collision_matrix;
    if (n.has_child("link_padding"))
        n["link_padding"] >> rhs->link_padding;
    if (n.has_child("link_scale"))
        n["link_scale"] >> rhs->link_scale;
    if (n.has_child("object_colors"))
        n["object_colors"] >> rhs->object_colors;
    if (n.has_child("world"))
        n["world"] >> rhs->world;
    if (n.has_child("is_diff")) {
        bool b;
        n["is_diff"] >> b;
        rhs->is_diff = b;
    }

    return true;
}

}  // namespace yml
}  // namespace c4
