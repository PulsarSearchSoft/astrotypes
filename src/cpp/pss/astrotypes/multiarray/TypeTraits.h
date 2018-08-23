/*
 * MIT License
 * 
 * Copyright (c) 2018 PulsarSearchSoft
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef PSS_ASTROTYPES_MULTIARRAY_TYPETRAITS_H
#define PSS_ASTROTYPES_MULTIARRAY_TYPETRAITS_H

#include <type_traits>


namespace pss {
namespace astrotypes {

template<typename T1, typename T2>
struct And : public std::false_type
{};

template<>
struct And<std::true_type, std::true_type> : public std::true_type
{};

/**
 * @biref return true if the Dimension is represented in the structure @tparam T
 * @tparam Dimension  : the dimension to find 
 * @param T : sthe structure to search for the Dimension
 * @code
 * /// example of a definition for a function that only supports types that support the Time dimension
 * template<typename T>
 * void do_something(std::enable_if<has_dimension<units::Time>::value, T>::type const&);
 * @endcode
 *
 * @details
 * provide a specialisation of this class to inherit from std::true_type if it hss the specified dimension
 */
template<typename T, typename Dimension>
struct has_dimension : public std::false_type
{
};

template<typename T >
struct has_dimension<T, T> : public std::true_type
{
};

/**
 * @biref return true if the all Dimensions provided are represented in the structure @tparam T
 */
template<typename T, typename Dimension, typename... Dimensions>
struct has_dimensions : public And<typename has_dimension<T, Dimension>::type, typename has_dimensions<T, Dimensions...>::type>
{
};

template<typename T, typename Dimension>
struct has_dimensions<T, Dimension> : public has_dimension<T, Dimension>
{
};

// convenience alias
//template<typename T, typename Dimension, typename... Dimensions>
//using has_dimensions = has_dimension<T, Dimension, Dimensions...>;

/**
 * @biref return true if the Dimensions provided match exaclty those of the structure T (including order)
 * @tparam Dimensions  : the dimension to find in the order required
 * @param T : sthe structure to search for the Dimension
 * members:
 *     bool value; // true or false
 *     type; // std::true_type or std::false_type
 * @code
 * /// example of a definition for a function that only supports types that are Time and Frequency ordererd
 * template<typename T>
 * void do_something(std::enable_if<has_exact_dimension<units::Time, units::Frequency>::value, T>::type const&);
 * @endcode
 */
template<typename T, typename... Dimensions>
struct has_exact_dimensions : public std::false_type 
{
};

} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_MULTIARRAY_TYPETRAITS_H