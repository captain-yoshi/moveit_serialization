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
   Desc: Wrapper alternatives to format float types
*/

#pragma once

#include <c4/format.hpp>

namespace c4 {
namespace yml {

// Create new float type
template <class T>
struct floattype
{
    T val;
};

// Convert float or double to floattype
template <class T>
inline auto freal(const T& t);

template <>
inline auto freal(const float& f)
{
    return floattype<float>{ f };
}

template <>
inline auto freal(const double& d)
{
    return floattype<double>{ d };
}

// Wrapper for formating float and doubles
template <class T>
inline std::size_t to_chars(substr buf, floattype<T> fmt)
{
    // https://en.cppreference.com/w/cpp/types/numeric_limits/max_digits10

    // The value of std::numeric_limits<T>::max_digits10 is the number of base-10 digits that are necessary to uniquely
    // represent all distinct values of the type T, such as necessary for serialization/deserialization to text. This
    // constant is meaningful for all floating-point types.
    //
    // Unlike most mathematical operations, the conversion of a floating-point value to text and back is exact as long
    // as at least max_digits10 were used (9 for float, 17 for double): it is guaranteed to produce the same
    // floating-point value, even though the intermediate text representation is not exact. It may take over a hundred
    // decimal digits to represent the precise value of a float in decimal notation.

    return c4::ftoa(buf, fmt.val, std::numeric_limits<T>::max_digits10, FTOA_FLEX /*pick the proper format*/);
}

}  // namespace yml
}  // namespace c4
