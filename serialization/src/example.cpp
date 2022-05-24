
#include <moveit_serialization/yaml-cpp/filter.h>

using namespace YAML::Filter;

int main(int argc, char** argv) {
	std::string cmd = "'.[].dataset.software[]|select(.name==\"moveit_ core\") | .version'";
	// std::string cmd = "'.[].dataset.software[].name'";
	// std::string cmd = "a'.'";
	// std::string cmd = "'.data'";

	TokenPairs list = parseCmd(cmd);

	YAML::Node node = YAML::LoadFile("/home/captain-yoshi/.ros/tmp/dataset.yaml");

	State state = State(Start{ .node = node });

	// auto visitor = std::bind(Transitions(), list, event, std::placeholders::_2);
	Transitions visitor;

	visitor.flows = list;

	auto events = buildEvents(list);

	for (const auto& event : events) {
		auto new_state = boost::apply_visitor(visitor, state, event);

		if (new_state)
			state = std::move(*new_state);
		else
			break;
	}
}
