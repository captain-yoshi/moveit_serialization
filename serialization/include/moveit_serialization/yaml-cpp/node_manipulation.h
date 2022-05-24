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

/* Author: Captain Yoshi, Carlos O'Ryan (Jaybeams), Adam Nevraumont, Kunal Tyagi
   Desc: Utility functions for manipulating/comparing YAML nodes

   TODO add proper liscence
   Carlos O'Ryan: https://coryan.github.io/jaybeams/merge__yaml_8hpp_source.html
     - merge_node
     - merge_map
     - merge_sequences
     - merge
     - clone
   Adam Nevraumont: https://stackoverflow.com/a/56462505
     - dispatch
   Kunal Tyagi: https://gist.github.com/kunaltyagi/ebe13098cc22a717f793684659644f4e
     - isSubset
*/

#pragma once

#include <moveit_serialization/yaml-cpp/conversion/conversion.h>

/**
 * Store the overrides for each class.
 *
 * Jaybeams configuration objects can be overriden "by-class",
 * meaning, all configs of the same class receive the same overrides.
 * This type is used to (temporarily) store the by-class overrides in
 * a given context.
 */
typedef std::map<std::string, YAML::Node> class_overrides;

namespace YAML {
/**
 * Merge two YAML nodes.
 *
 * Unlike a simple assignment, if @a source does not have a value
 * for a given key, we keep the value from @a target.
 */
void merge_node(Node target, Node const& source);

/**
 * Merge all the values from @a source into @a target
 *
 * Unlike a simple assignment, if @a source does not have a value
 * for a given key, we keep the value from @a target.
 */
void merge_map(Node target, Node const& source);

/**
 * Memberwise merge two sequences, from @a source into @a target.
 *
 * If @a source has more elements than @a target the additional values
 * are appended.  If @a source has less elements than @a target, the
 * extra values in @a target are unmodified.
 */
void merge_sequences(Node target, Node const& source);

/**
 * Merge the class-overrides from @a source into @a by_class.
 *
 * Given a set of by-class overrides apply any additional by-class
 * overrides from @a source source into @a by_class.
 */
void merge(class_overrides& by_class, Node source);

/**
 * Recursively clone all the overrides in @a by_class.
 */
class_overrides clone(class_overrides const& by_class);

/**
 * c++14 equivalent of 'if constexpr' (c++17)
 */
template <class Integral, Integral N>
auto dispatch(std::integral_constant<Integral, N>) {
	return [](auto&&... args) { return std::get<N>(std::forward_as_tuple(decltype(args)(args)...)); };
}
template <std::size_t N>
auto dispatch() {
	return dispatch(std::integral_constant<std::size_t, N>{});
}

/**
 * Comparison between 2 YAML::Node
 *
 * Given some types, the nodes are decoded and compared.
 */
template <typename... Ts>
struct scalar_compare
{
	bool equality(const Node& source, const Node& target) { return _impl_equality<0>(source, target); }

	bool non_equality(const Node& source, const Node& target) { return !_impl_equality<0>(source, target); }

	bool lower_then(const Node& source, const Node& target) { return _impl_lower_then<0>(source, target); }

	bool greater_then(const Node& source, const Node& target) { return _impl_greater_then<0>(source, target); }

	bool lower_equal(const Node& source, const Node& target) { return !_impl_greater_then<0>(source, target); }

	bool greater_equal(const Node& source, const Node& target) { return !_impl_lower_then<0>(source, target); }

private:
	using TypeTuple = std::tuple<Ts...>;
	constexpr static std::size_t TUPLE_SIZE = std::tuple_size<TypeTuple>::value;

	template <std::size_t N>
	bool _impl_equality(const Node& source, const Node& target) {
		return dispatch(std::integral_constant<bool, N == TUPLE_SIZE>{})(
		    // 0, aka false branch:
		    [&](auto Nval) {
			    using SelectedType = std::tuple_element_t<Nval, TypeTuple>;

			    try {
				    return source.as<SelectedType>() == target.as<SelectedType>();
			    } catch (TypedBadConversion<SelectedType>& e) {
				    return _impl_equality<Nval + 1>(source, target);
			    }
		    },
		    // 1, aka true branch:
		    [](auto&&) { return false; })(std::integral_constant<std::size_t, N>{});
	}

	template <std::size_t N>
	bool _impl_greater_then(const Node& source, const Node& target) {
		return dispatch(std::integral_constant<bool, N == TUPLE_SIZE>{})(
		    // 0, aka false branch:
		    [&](auto Nval) {
			    using SelectedType = std::tuple_element_t<Nval, TypeTuple>;

			    try {
				    return source.as<SelectedType>() > target.as<SelectedType>();
			    } catch (TypedBadConversion<SelectedType>& e) {
				    return _impl_greater_then<Nval + 1>(source, target);
			    }
		    },
		    // 1, aka true branch:
		    [](auto&&) { return false; })(std::integral_constant<std::size_t, N>{});
	}

	template <std::size_t N>
	bool _impl_lower_then(const Node& source, const Node& target) {
		return dispatch(std::integral_constant<bool, N == TUPLE_SIZE>{})(
		    // 0, aka false branch:
		    [&](auto Nval) {
			    using SelectedType = std::tuple_element_t<Nval, TypeTuple>;

			    try {
				    return source.as<SelectedType>() < target.as<SelectedType>();
			    } catch (TypedBadConversion<SelectedType>& e) {
				    return _impl_lower_then<Nval + 1>(source, target);
			    }
		    },
		    // 1, aka true branch:
		    [](auto&&) { return false; })(std::integral_constant<std::size_t, N>{});
	}
};

template <class... Args>
bool scalar_compare_eq(const Node& source, const Node& target);

template <class T, class... Args>
bool helper_compare_eq(const Node& source, const Node& target) {
	try {
		return source.as<T>() == target.as<T>();
	} catch (TypedBadConversion<T>& e) {
		return scalar_compare_eq<Args...>(source, target);
	}
}

/**
 * source is subset of target
 *
 * can ommit comparison between scalars
 */
bool isSubset(const Node& source, const Node& target, bool compare_scalar = true);

/**
 * Get target scalar node from source scalar node.
 */
bool getSubsetScalar(const Node& source, const Node& target, Node& scalar);

void removeSeqOnKeyMatch(const std::string& key, Node& node, bool match);

}  // namespace YAML
