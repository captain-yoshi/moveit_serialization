# MoveIt Serialization

Serializer for multiple ros messages including moveit_msgs. Supports yaml serialization through rapidyaml.

## Roadmap
- Add binary serealizer for ros_msgs

## rapidyaml
One of the fastest yaml parser and emiter available. Built internally to avoid unwanted changes to the global error handler.<br>
Current version: https://github.com/captain-yoshi/rapidyaml/commit/e9ec9fcf5e6f34e5f44e48f1c1038cbb7e38b285

## Contribution
The conversion for decoding/encoding moveit_msgs in yaml-cpp was done by Zachary Kingston and taken from the [robowflex](https://github.com/KavrakiLab/robowflex) project.
