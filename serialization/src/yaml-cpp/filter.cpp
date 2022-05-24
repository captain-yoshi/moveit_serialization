#include <boost/tokenizer.hpp>
#include <iostream>

#include <moveit_serialization/yaml-cpp/filter.h>
#include <moveit_serialization/yaml-cpp/node_manipulation.h>

namespace YAML {
namespace Filter {

namespace {

const std::map<std::string, FunctionOperator> NameToFuncOpMAP = { { "select", FuncOperator::SELECT } };

FunctionOperator getFuncOpFromStr(const std::string& name) {
	auto it = NameToFuncOpMAP.find(name);
	if (it != NameToFuncOpMAP.end())
		return it->second;
	else
		return FunctionOperator::INVALID;
}

LogicalOperator getLogicalOpFromStr(const std::string& name) {
	if (name.size() == 1) {
		if (name[0] == '>')
			return LogicalOperator::GREATER_THEN;
		if (name[0] == '<')
			return LogicalOperator::LESS_THEN;
	} else if (name.size() == 2) {
		if (name[0] == '=' && name[1] == '=')
			return LogicalOperator::EQUALITY;
		if (name[0] == '!' && name[1] == '=')
			return LogicalOperator::NON_EQUALITY;
		if (name[0] == '>' && name[1] == '=')
			return LogicalOperator::GREATER_EQUAL;
		if (name[0] == '<' && name[1] == '=')
			return LogicalOperator::LESS_EQUAL;
	}

	// no match
	return LogicalOperator::INVALID;
}

void filterNodeFromOii(TokenPairs& flows, YAML::Node& node) {
	while (!flows.empty()) {
		// case: .key[]
		if (flows.size() >= 4 && flows[0].first == '.' && flows[1].first == '\0' && flows[2].first == '[' &&
		    flows[3].first == ']') {
			auto key = flows[1].second;

			if (node.IsSequence()) {
				YAML::removeSeqOnKeyMatch(key, node, false);

				// merge sequences
				Node n;
				for (YAML::iterator it = node.begin(); it != node.end(); ++it) {
					Node seq = *it;
					if (seq[key] && seq[key].IsSequence()) {
						for (YAML::iterator itc = seq[key].begin(); itc != seq[key].end(); ++itc)
							n.push_back(*itc);
					}
				}
				node = n;

			} else {
				if (node[key])
					node = node[key];
				else
					node = Node();
			}
			// std::cout << "===========" << std::endl;
			// std::cout << node << std::endl;
			// std::cout << "===========" << std::endl;
			flows.erase(flows.begin(), flows.begin() + 4);
		}
		// case: .[]
		else if (flows.size() >= 3 && flows[0].first == '.' && flows[1].first == '[' && flows[2].first == ']') {
			if (!node.IsSequence())
				node = YAML::Node();
			// std::cout << "===========" << std::endl;
			// std::cout << node << std::endl;
			// std::cout << "===========" << std::endl;
			flows.erase(flows.begin(), flows.begin() + 3);
		}
		// case: .key
		else if (flows.size() >= 2 && flows[0].first == '.' && flows[1].first == '\0') {
			auto key = flows[1].second;

			if (node.IsSequence()) {
				YAML::removeSeqOnKeyMatch(key, node, false);

				// merge sequences
				Node n;
				for (YAML::iterator it = node.begin(); it != node.end(); ++it) {
					Node seq = *it;

					if (seq[key] && !seq[key].IsSequence())
						n.push_back(seq[key]);
				}
				node = n;
			} else {
				if (node[key])
					node = node[key];
				else
					node = Node();
			}

			// std::cout << "===========" << std::endl;
			// std::cout << node << std::endl;
			// std::cout << "===========" << std::endl;
			flows.erase(flows.begin(), flows.begin() + 2);
		}
		// case: .
		else if (flows.size() >= 1 && flows[0].first == '.') {
			// do nothing
			flows.erase(flows.begin() + 0);
		} else {
			break;
		}
	}
}

boost::optional<State> selectFuncOperator(TokenPairs& flows, std::size_t args, YAML::Node& node) {
	std::size_t flows_size = flows.size();

	// Get Ooi
	filterNodeFromOii(flows, node);

	if (flows.size() < 2)
		return State(Error{ .err_msg = "Missing arguments" });

	std::string predicate_str;
	if (flows[0].second.empty() && !flows[1].second.empty())
		predicate_str = flows[0].first;
	else
		predicate_str = flows[0].first + flows[1].first;

	// Get logical operator
	auto predicate = getLogicalOpFromStr(predicate_str);

	if (predicate == LogicalOperator::INVALID)
		return State(Error{ .err_msg = "Invalid logical operator" });

	predicates
}

}  // namespace

TokenPairs parseCmd(const std::string& cmd) {
	if (cmd.empty())
		return {};

	TokenPairs tok_pairs;

	boost::char_separator<char> sep("", ".,|()[] '!=<>\"");
	boost::tokenizer<boost::char_separator<char> > tok(cmd, sep);

	bool dquote_start = false;
	std::string quote_buf;

	for (auto it = tok.begin(); it != tok.end(); ++it) {
		auto& str = *it;
		if (dquote_start && str[0] != '"') {
			// accumulate tokens in double quotes
			quote_buf += str;
		} else if (it->size() == 1 && str[0] == '"') {
			if (dquote_start) {
				dquote_start = false;
				tok_pairs.push_back({ '\0', quote_buf });
				quote_buf.clear();
			} else
				dquote_start = true;
		} else if (it->size() == 1 && std::ispunct(static_cast<unsigned char>(str[0]))) {
			tok_pairs.push_back({ str[0], "" });

		} else {
			tok_pairs.push_back({ '\0', str });
		}
	}

	if (dquote_start)
		throw std::runtime_error("Command malformed. Missing double quote ending.");

	for (const auto& tok : tok_pairs)
		std::cout << tok.first << "\t::\t" << tok.second << '\n';

	return tok_pairs;
}

std::vector<Event> buildEvents(const TokenPairs& tok_pairs) {
	std::vector<Event> events;
	bool first = true;

	bool reset = false;

	for (const auto& pair : tok_pairs) {
		auto& del = pair.first;
		if (del == '\'') {
			if (first) {
				std::cout << "EventPipe" << std::endl;
				events.push_back(Event(EventPipe{}));
				first = false;
			} else {
				std::cout << "EventEnd" << std::endl;
				events.push_back(Event(EventEnd{}));
			}
		} else if (!reset && del == '.') {
			std::cout << "EventOii" << std::endl;
			events.push_back(Event(EventOii{}));
			reset = true;
		} else if (del == '(' || del == ')') {
			std::cout << "EventFunc" << std::endl;
			events.push_back(Event(EventFuncOperator{}));
		}
	}
	return events;
}

boost::optional<State> Transitions::operator()(Start& s, const EventPipe& e) {
	std::cout << "Start -> EventPipe" << std::endl;
	if (flows.size() < 2)
		return State(Error{ .err_msg = "Command with too few arguments" });

	// move and pop first element
	auto flow = std::move(flows.front());
	flows.erase(flows.begin());

	// command MUST begin and end with single quotes
	if (flow.first != '\'' || flows.back().first != '\'')
		return State(Error{ .err_msg = "Command MUST be enclosed within single quotes" });

	// case: empty quotes
	if (flows.size() == 1)
		return State(End{ .node = std::move(s.node) });

	return State(Pipe{ .node = std::move(s.node) });
}

boost::optional<State> Transitions::operator()(Pipe& s, const EventOii& e) {
	std::cout << "Pipe -> EvenOii" << std::endl;

	// case: empty flow
	if (flows.empty())
		return State(Error{ .err_msg = "Missing arguments" });

	// case: not dot
	if (flows[0].first != '.')
		return State(Error{ .err_msg = "Object identifier-index MUST start with a dot '.'" });

	// filter out YAML node based on object identifier-index
	filterNodeFromOii(flows, s.node);

	return State(Oii{ .node = s.node });
}

boost::optional<State> Transitions::operator()(Pipe& s, const EventFuncOperator& e) {
	std::cout << "Pipe -> EvenOii" << std::endl;

	// case: minimal parameters -> name and '('
	if (flows.size() < 3)
		return State(Error{ .err_msg = "Missing arguments" });

	auto func_name = flows[0].second;
	flows.erase(flows.begin(), flows.begin() + 2);

	// get functi
	if (flows[0].first != '\0' && flows[1].first != '(')
		return State(Error{ .err_msg = "Function operators MUST start with an alphanumerical name followed by '('" });

	// compute end parenthesis index
	std::size_t i;
	for (i = 0; i < flows.size(); ++i) {
		if (flows[i].first == ')') {
			break;
		}
	}

	// end parenthesis not found
	if (i == flows.size())
		return State(Error{ .err_msg = "Function operators MUST be enclosed by a parenthesis ')'" });

	// number of arguments of the function
	std::size_t args = i + 1;

	auto func = getFuncOpFromStr(func_name);

	switch (func) {
		case FunctionOperator::SELECT:
			return selectFuncOperator(flows, args, s.node);
		case FunctionOperator::INVALID:
			// do nothing
			break;
	}

	return State(Error{ .err_msg = "Invalid function operator name" });
}

boost::optional<State> Transitions::operator()(Oii& s, const EventFuncOperator& e) {
	// Get rel node from Oii state
	Node rel_node = s.node;

	// return State({ .node = s.node });
}

boost::optional<State> Transitions::operator()(Oii& s, const EventEnd& e) {
	std::cout << "Oii -> EvenEnd" << std::endl;
	std::cout << s.node;
	return State(End{ .node = s.node });
}

// std::string FilterParser::getValue(const std::string& cmd, const Node& node) {
// 	if (cmd.empty())
// 		return "";

// 	std::string esc_sep("\\");  // escape character
// 	std::string delim(".,|()[] '");  // split on spaces, tabs, new lines, commas
// 	std::string quotes("\"");  // allow double-quoted values with delimiters within

// 	boost::escaped_list_separator<char> separator(esc_sep, delim, quotes);
// 	boost::tokenizer<boost::escaped_list_separator<char>, std::string::const_iterator, std::string> tok(cmd,
// separator);

// 	std::vector<std::size_t> tok_indexes;

// 	std::map<std::size_t, std::string> map;

// 	for (auto it = tok.begin(); it != tok.end(); ++it) {
// 		std::size_t offset = it.base() - cmd.begin() - it->size() - 1;
// 		tok_indexes.push_back(offset);
// 		std::cout << offset << "\t::\t" << *it << '\n';

// 		map.insert({ offset, *it });
// 	}

// 	///
// 	///
// 	State state = State::START;
// 	State prev_state = State::START;

// 	for (auto pair : map) {
// 		if (pair.second.empty()) {
// 			// delimiter found
// 			switch (cmd[pair.first]) {
// 				case '\'':
// 					// do nothing
// 					break;
// 				case '.':
// 					break;
// 				case '[':
// 					break;
// 				case ',':
// 					break;
// 				case '(':
// 					break;
// 				case ';':
// 					break;
// 			}

// 		} else {
// 			// function name, key, val

// 			if (prev_state == State::DOT) {
// 				// Key

// 			} else if (prev_state == State::START || prev_state == State::PIPE) {
// 				// Function operator
// 				if (pair.second.compare("select") == 0) {
// 				} else {
// 					throw std::runtime_error("Invalid function operator");
// 				}
// 			}
// 		}
// 	}

// 	return "";
// }

// std::string FilterParser::getValues(const std::string& cmd, const Node& node) {}

// Node FilterParser::getNode(const std::string& cmd, const Node& node) {}

}  // namespace Filter
}  // namespace YAML
