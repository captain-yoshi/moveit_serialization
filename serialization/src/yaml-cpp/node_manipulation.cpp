#include <moveit_serialization/yaml-cpp/node_manipulation.h>

void YAML::merge_node(YAML::Node target, YAML::Node const& source) {
	switch (source.Type()) {
		case YAML::NodeType::Scalar:
			target = source.Scalar();
			break;
		case YAML::NodeType::Map:
			merge_map(target, source);
			break;
		case YAML::NodeType::Sequence:
			merge_sequences(target, source);
			break;
		case YAML::NodeType::Null:
			throw std::runtime_error("merge_node: Null source nodes not supported");
		case YAML::NodeType::Undefined:
			throw std::runtime_error("merge_node: Undefined source nodes not supported");
	}
}

void YAML::merge_map(YAML::Node target, YAML::Node const& source) {
	for (auto const& j : source) {
		merge_node(target[j.first.Scalar()], j.second);
	}
}

void YAML::merge_sequences(YAML::Node target, YAML::Node const& source) {
	for (std::size_t i = 0; i != source.size(); ++i) {
		if (i < target.size()) {
			merge_node(target[i], source[i]);
		} else {
			target.push_back(YAML::Clone(source[i]));
		}
	}
}

void YAML::merge(class_overrides& by_class, YAML::Node node) {
	// Only Map nodes can override by-class values ...
	if (not node.IsMap()) {
		return;
	}
	// ... iterate over the node, searching for nodes with a key starting
	// with ':' ...
	for (auto i : node) {
		// ... the node is a map, there should be keys for all sub nodes ...
		// std::runtime_error(i.first.as<std::string>("error on key"));
		// ... found a key, check the format ...
		std::string key = i.first.as<std::string>();
		if (key[0] != ':') {
			continue;
		}
		// ... try to insert into the map ...
		auto ins = by_class.emplace(key, i.second);
		if (ins.second == true) {
			// ... good insert, nothing left to do ...
			continue;
		}
		// ... okay there was a node for the class in the map already,
		// need to merge the values ...
		merge_node(ins.first->second, i.second);
	}
}

class_overrides YAML::clone(class_overrides const& by_class) {
	class_overrides tmp;
	for (auto const& i : by_class) {
		tmp.emplace(i.first, YAML::Clone(i.second));
	}
	return tmp;
}
template <class... Args>
bool YAML::scalar_compare_eq(const YAML::Node& source, const YAML::Node& target) {
	return helper_compare_eq<Args...>(source, target);
}
template <>
bool YAML::scalar_compare_eq<>(const YAML::Node&, const YAML::Node&) {
	return true;
}

bool YAML::isSubset(const YAML::Node& source, const YAML::Node& target, bool compare_scalar) {
	// case where scalar is disabled when compared
	if (!compare_scalar && source.Type() == YAML::NodeType::Scalar)
		return true;

	// Validation
	// TODO validate Tag ? Not supported in yaml-cpp ...
	if (source.Type() != target.Type())
		return false;

	switch (source.Type()) {
		case YAML::NodeType::Scalar:
			return scalar_compare_eq<bool, int, double, std::string>(source, target);
		case YAML::NodeType::Null:
			return true;
		case YAML::NodeType::Undefined:
			return false;
		case YAML::NodeType::Map:
		case YAML::NodeType::Sequence:
			if (source.size() > target.size())
				return false;
	}

	// Loop through map
	if (source.IsMap()) {
		bool result = true;
		for (YAML::const_iterator it1 = source.begin(), it2; it1 != source.end() && result == true; ++it1) {
			const auto& key = it1->first.as<std::string>();

			if (!target[key])
				return false;

			result = isSubset(it1->second, target[key], compare_scalar);
		}
		return result;
	}

	// Loop through sequence
	if (source.IsSequence()) {
		bool result = true;
		for (YAML::const_iterator it1 = source.begin(), it2 = target.begin(); it1 != source.end() && result == true;
		     ++it1, ++it2) {
			result = isSubset(*it1, *it2, compare_scalar);
		}
		return result;
	}

	// should not end up here
	return true;
}

bool YAML::getSubsetScalar(const YAML::Node& source, const YAML::Node& target, YAML::Node& scalar) {
	if (source.Type() != YAML::NodeType::Scalar)
		if (source.Type() != target.Type())
			return false;

	switch (source.Type()) {
		case YAML::NodeType::Scalar:
			scalar = YAML::Clone(target);
			return true;
		case YAML::NodeType::Null:
			return true;
		case YAML::NodeType::Undefined:
			return false;
		case YAML::NodeType::Map:
		case YAML::NodeType::Sequence:
			if (source.size() > target.size())
				return false;
	}

	// Loop through map
	if (source.IsMap()) {
		bool result = true;
		for (YAML::const_iterator it1 = source.begin(), it2; it1 != source.end() && result == true; ++it1) {
			const auto& key = it1->first.as<std::string>();

			if (target[key])
				result = getSubsetScalar(it1->second, target[key], scalar);
			else
				return false;
		}
		return result;
	}

	// Loop through sequence
	if (source.IsSequence()) {
		bool result = true;
		for (YAML::const_iterator it1 = source.begin(), it2 = target.begin(); it1 != source.end() && result == true;
		     ++it1, ++it2) {
			result = getSubsetScalar(*it1, *it2, scalar);
		}
		return result;
	}

	return true;
}

void YAML::removeSeqOnKeyMatch(const std::string& key, Node& node, bool match) {
	for (std::size_t i = 0; i < node.size(); ++i) {
		if (match) {
			if (node[i][key])
				node.remove(i);
		} else {
			if (!node[i][key])
				node.remove(i);
		}
	}
}
