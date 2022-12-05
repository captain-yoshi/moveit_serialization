# MoveIt Serialization

Serializer for multiple ros messages including moveit_msgs. Supports yaml serialization through [rapidyaml](https://github.com/biojppm/rapidyaml).

## Rapidyaml
One of the fastest yaml parser and emiter available.
- Current version: [captain-yoshi/rapidyaml/e9ec9fc](https://github.com/captain-yoshi/rapidyaml/commit/e9ec9fcf5e6f34e5f44e48f1c1038cbb7e38b285).
- Built internally to avoid unwanted changes to the global error handler.
- Custom `Tree::has_all` method for subset node comparaison (not yet merged).
- `C4_DEBUG_BREAK` is disabled at runtime in the error handler.

## ROS Messages
The serialization uses the same ros messages [built-in types](http://wiki.ros.org/msg) convention except for the `bool/bool[]` types. These types are encoded as boolean's `true/false` instead of uint8_t `1/0`.

Encoding a message will give an exact representation of the message (except for boolean's). Decoding a message follows the exact message structure. There are some messages that have optional decoding utilities:

| ROS Msg Type                | Alternate Decoding Option
|:----------------------------|:------------------------------------------------------------------------------------------|
| *geometry_msgs::Point*      | [x, y, z]                                                                                 |
| *geometry_msgs::Vector3*    | [x, y, x]                                                                                 |
| *geometry_msgs::Quaternion* | [x, y, z, w]                                                                              |
| *shape_msgs::Mesh*          | resource: package://my_pkg_name/mesh.stl <br> dimensions: [x, y, z]                       |

## Caveats
Encoding for `bool` MUST be formatted using `fmt::boolalpha`.

Encoding for `floats/double` MUST be formated using `freal`. This is to ensure that the conversion of a floating-point value to text and back is [exact](include/moveit_serialization/ryml/format.h#L73-L84). The drawback is that it needs 9 and 17 digits respectively for floats and doubles, thus increasing the size of memory needed.


## Contribution
The initial serialization using [yaml-cpp](https://github.com/jbeder/yaml-cpp) was done by Zachary Kingston from the [robowflex](https://github.com/KavrakiLab/robowflex) project.
