/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, Captain Yoshi
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Captain Yoshi
   Desc: Filter for yaml

*/

#pragma once

#include <iostream>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <moveit_serialization/yaml-cpp/yaml.h>

namespace YAML {
namespace Filter {

using TokenPairs = std::vector<std::pair<char, std::string>>;

/// Events
struct EventPipe
{};
struct EventOii
{};
struct EventFuncOperator
{};
struct EventEnd
{};

using Event = boost::variant<EventPipe, EventOii, EventFuncOperator, EventEnd>;

/// States
struct Pipe
{
	Node node;
};
struct Oii
{
	Node node;
};
struct FuncOperator
{
	Node node;
	std::string name;
};
struct Start
{
	Node node;
};
struct End
{
	Node node;
};

struct Error
{
	std::string err_msg;
};

using State = boost::variant<Start, End, Pipe, Oii, FuncOperator, Error>;

/// Transitions
struct Transitions : public boost::static_visitor<boost::optional<State>>
{
	boost::optional<State> operator()(Start&, const EventPipe& e);
	boost::optional<State> operator()(Pipe&, const EventOii& e);
	boost::optional<State> operator()(Pipe&, const EventFuncOperator& e);
	boost::optional<State> operator()(Oii&, const EventEnd& e);
	boost::optional<State> operator()(Oii&, const EventFuncOperator& e);

	template <typename State_t, typename Event_t>
	boost::optional<State> operator()(State_t&, const Event_t&) const {
		std::cout << "Unkown" << std::endl;
		return boost::none;
	}

	TokenPairs flows;
};

enum class FunctionOperator : int
{
	INVALID,  //
	SELECT,
};

enum class LogicalOperator : int
{
	INVALID,  //
	EQUALITY,
	NON_EQUALITY,
	GREATER_THEN,
	GREATER_EQUAL,
	LESS_THEN,
	LESS_EQUAL,
};

/// Parse delimiters and tokens from command
TokenPairs parseCmd(const std::string& cmd);

std::vector<Event> buildEvents(const TokenPairs& tok_pairs);

// class FilterParser
// {
// public:
// 	std::set<std::string> relational_operators = {
// 		"==", "!=", ">", ">=", "<", "<=",
// 	};

// 	static constexpr char OII = '.';

// 	// std::set<std::string> logical_operators = {
// 	// 	"|", "(", ".", ">=", "<", "<=",
// 	// };

// 	FilterParser() = default;

// 	/// Expects a single primitive value when applying filter, otherwise an empty string is returned
// 	std::string getValue(const std::string& cmd, const Node& node);
// 	/// Expects a single or multiple primitive values when applying filter, otherwise an empty string is returned
// 	std::string getValues(const std::string& cmd, const Node& node);

// 	Node getNode(const std::string& cmd, const Node& node);

// private:
// };

}  // namespace Filter
}  // namespace YAML
