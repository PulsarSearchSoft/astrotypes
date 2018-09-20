/*
 * MIT License
 *
 * Copyright (c) 2018 PulsarearchSoft, Chris Williams, and the SKA Organisation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "oftware"), to deal
 * in theoftware without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of theoftware, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of theoftware.
 *
 * THEOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIE OF MERCHANTABILITY,
 * FITNES FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWIE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THEOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *OFTWARE.
 */
#ifndef PS_ASTROTYPES_MULTIARRAY_TYPETRAITS_H
#define PS_ASTROTYPES_MULTIARRAY_TYPETRAITS_H
#include <type_traits>
#include <tuple>

#include <type_traits>


namespace pss {
namespace astrotypes {

/**
 * @brief helper class to perform a logical and on all the arguments
 */
template<typename T1, typename T2, typename... Ts>
struct logical_and : public logical_and<typename logical_and<T1, T2>::type, Ts...>::type
{};

template<typename T>
struct logical_and<T, T> : public std::true_type
{};

template<typename T1, typename T2>
struct logical_and<T1, T2> : public std::false_type
{};

static_assert(logical_and<std::true_type, std::true_type>::value, "oh oh");
static_assert(!logical_and<std::false_type, std::true_type>::value, "oh oh");
static_assert(!logical_and<std::true_type, std::false_type>::value, "oh oh");
static_assert(logical_and<std::true_type, std::true_type, std::true_type>::value, "oh oh");
static_assert(!logical_and<std::false_type, std::true_type, std::true_type>::value, "oh oh");
static_assert(!logical_and<std::true_type, std::true_type, std::false_type>::value, "oh oh");

/**
 * @brief return true if the Dimension is represented in the structure @tparam T
 * @tparam Dimension  : the dimension to find 
 * @param T : the structure to search for the Dimension
 * @code
 * /// example of a definition for a function that only supports types that support the Time dimension
 * template<typename T>
 * void do_something(std::enable_if<has_dimension<units::Time>::value, T>::type const&);
 * @endcode
 *
 * @details
 * provide a specialisation of this class to inherit from std::true_type if it has the specified dimension
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
 * @brief return true if the all Dimensions provided are represented in the structure @tparam T
 */
template<typename T, typename Dimension, typename... Dimensions>
struct has_dimensions : public logical_and<typename has_dimension<T, Dimension>::type, typename has_dimensions<T, Dimensions...>::type>
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
 * @brief return true if the Dimensions provided match exactly those of the structure T (including order)
 * @tparam Dimensions  : the dimension to find in the order required
 * @param T : the structure to search for the Dimension
 * members:
 *     bool value; // true or false
 *     type; // std::true_type or std::false_type
 * @code
 * /// example of a definition for a function that only supports types that are Time and Frequency ordered
 * template<typename T>
 * void do_something(std::enable_if<has_exact_dimension<units::Time, units::Frequency>::value, T>::type const&);
 * @endcode
 */
template<typename T, typename... Dimensions>
struct has_exact_dimensions : public std::false_type
{
};

/**
 * @ brief helper class to determine if a tuple has a certain type
 */
template<typename Tuple, typename T>
struct has_type : std::false_type
{ };

template<typename T, typename... Ts>
struct has_type<std::tuple<T, Ts...>, T> : public std::true_type
{};

template<typename T, typename T2, typename... Ts>
struct has_type<std::tuple<T2, Ts...>, T> : public has_type<T, std::tuple<Ts...>>::type
{};

template<typename Tuple, typename T, typename... Ts>
struct has_types : public logical_and<typename has_type<Tuple, T>::type, has_type<Tuple, Ts>...>::type
{
};

/**
 * @brief produces a tuple type with params form all provided Tuples
 */
template<typename Tuple1, typename... Tuples>
struct join_tuples;

template<typename... Tuple1, typename... Tuple2>
struct join_tuples<std::tuple<Tuple1...>, std::tuple<Tuple2...>>
{
    typedef std::tuple<Tuple1..., Tuple2...> type;
};

template<typename... Tuple1, typename... Tuple2, typename... Tuples>
struct join_tuples<std::tuple<Tuple1...>, std::tuple<Tuple2...>, Tuples...>
{
    typedef typename join_tuples<std::tuple<Tuple1...>, typename join_tuples<std::tuple<Tuple2...>, Tuples...>::type>::type type;
};

///--------------------------------------------------------------------
// @brief produces a tuple that contains all elements in Tuple1 that are not in Tuple2
// --------------------------------------------------------------------
template<typename Tuple1, typename Tuple2>
struct tuple_diff;

template<typename T, typename... Tuple2>
struct tuple_diff<std::tuple<T>, std::tuple<Tuple2...>>
{
    typedef typename std::conditional<has_type<std::tuple<Tuple2...>, T>::value,
                             std::tuple<>,
                             std::tuple<T>>::type type;
};

template<typename T, typename... Tuple1, typename... Tuple2>
struct tuple_diff<std::tuple<T, Tuple1...>, std::tuple<Tuple2...>>
{
    private:
        typedef tuple_diff<std::tuple<Tuple1...>, std::tuple<Tuple2...>> Next;

    public:
        typedef typename std::conditional<has_type<std::tuple<Tuple2...>, T>::value,
                            typename Next::type,
                            typename join_tuples<std::tuple<T>, typename Next::type>::type>::type type;
};

} // namespace astrotypes
} // namespace pss

#endif // PS_ASTROTYPES_MULTIARRAY_TYPETRAITS_H
