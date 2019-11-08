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
 * @brief enable_if semantics without exposing them to the top level interface
 * @details this one supports templates with a single parameter
 * inherit from this class in the template declaration.
 * specialisations can be done in the normal way or via the enabler object which
 * allows the use of std::enable_if in the last param position
 */
template<template<typename> class, typename T1, class Enable=void>
struct enabler : public std::false_type
{
};

/**
 * @brief enable_if semantics without exposing them to the top level interface
 * @details this one supports templates with two parameters
 */
template<template<typename...> class, typename T1, typename T2, class Enable=void>
struct enabler2 : public std::false_type
{
};

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
struct has_dimension : public enabler2<has_dimension, T, Dimension>
{
};

template<typename T>
struct has_dimension<T, T> : public std::true_type
{
};

// consider const types to be the same as non const types
template<typename T, typename Dimension>
struct has_dimension<const T, Dimension> : public has_dimension<typename std::decay<T>::type, Dimension> {};

/**
 * @brief return true if the Dimension is explicitly represented in the structure @tparam T
 * @tparam Dimension  : the dimension to find
 * @param T : the structure to search for the Dimension
 * @details :
 *          this differs from has_dimension in that it will return false when the Dimension is implied
 *          by its type rather than being a core part of the implementation
 */
template<typename T, typename Dimension>
struct has_dimension_strict : public enabler2<has_dimension_strict, T, Dimension>
{
};

template<typename T, typename Dimension, template<typename> class Mixin>
struct has_dimension_strict<Mixin<T>, Dimension> : public has_dimension_strict<T, Dimension>
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
 * @ brief helper class to determine if a type is in a parameter list
 */
template<typename T1, typename T2, typename... Ts>
struct list_has_type : public std::conditional<std::is_same<T1, T2>::value || list_has_type<T1, Ts...>::value, std::true_type, std::false_type>::type
{ };

template<typename T>
struct list_has_type<T, T> : public std::true_type
{};

template<typename T1, typename T2>
struct list_has_type<T1, T2> : public std::false_type
{};

/**
 * @ brief helper class to determine if a tuple has a certain type
 */
template<typename Tuple, typename T2>
struct has_type : std::false_type
{ };

template<typename T, typename... Ts>
struct has_type<std::tuple<T, Ts...>, T> : public std::true_type
{};

template<typename T, typename T2, typename... Ts>
struct has_type<std::tuple<T2, Ts...>, T> : public has_type<std::tuple<Ts...>, T>::type
{};

template<typename T1, typename T2, typename... Ts>
struct has_types : public logical_and<typename has_type<T1, T2>::type, has_type<T1, Ts>...>::type
{
};

template<typename T1, typename T2>
struct has_types<T1, T2> : public has_type<T1, T2>::type
{
};

/**
 * @ brief helper class to determine the index of a type T in a tuple
 * @code
 * find_type<std::tuple<A, B>, B>::value; // == 1
 * find_type<std::tuple<A, B>, A>::value; // == 0
 * @endcode
 */
template<typename Tuple, typename T2>
struct find_type;

template<typename T, typename... Ts>
struct find_type<std::tuple<T, Ts...>, T>
{
    static const std::size_t value = 0;
};

template<typename T, typename T2, typename... Ts>
struct find_type<std::tuple<T2, Ts...>, T>
{
    static const std::size_t value = 1 + find_type<std::tuple<Ts...>, T>::value;
};

/**
 * @brief insert values into a tuple
 * @details tuple must not repeat types
 * @code
 * std::tuple<A, B> tuple;
 * B b;
 * tuple_insert_type(tuple, b);
 * @endcode
 */
template<typename Tuple, typename T, typename... Ts>
struct insert_type;

template<typename... TupleTs, typename T1, typename T2, typename... Ts>
struct insert_type<std::tuple<TupleTs...>, T1, T2, Ts...>
{
    inline
    static void exec(std::tuple<TupleTs...>& tuple, T1 const& value, T2 const& value2, Ts const&... values)
    {
        insert_type<std::tuple<TupleTs...>, T1>::exec(tuple, value);
        insert_type<std::tuple<TupleTs...>, T2, Ts...>::exec(tuple, value2, values...);
    }
};

template<typename... TupleTs, typename T>
struct insert_type<std::tuple<TupleTs...>, T>
{
    inline
    static void exec(std::tuple<TupleTs...>& tuple, T const& value)
    {
        static_assert(has_type<std::tuple<TupleTs...>, T>::value, "Type does not exist in tuple");
        std::get<find_type<std::tuple<TupleTs...>, T>::value>(tuple) = value;
    }
};

template<typename... TupleTs, typename... Ts>
void tuple_insert_type(std::tuple<TupleTs...>& tuple, Ts const&...values)
{
    insert_type<std::tuple<TupleTs...>, typename std::remove_reference<Ts>::type...>::exec(tuple, values...);
}

/**
 * @brief copy the matching elements (by type) from one tuple into anothe
 * @code
 * tuple_copy(src_tuple, dst_tuple);
 * @endcode
 */
template<typename Tuple, typename SrcTuple, std::size_t Index=std::tuple_size<SrcTuple>::value>
struct tuple_copy_helper : tuple_copy_helper<Tuple, SrcTuple, Index-1>
{
    tuple_copy_helper(Tuple& tuple, SrcTuple const& src_tuple)
        : tuple_copy_helper<Tuple, SrcTuple, Index-1>(tuple, src_tuple)
    {
        insert_type<Tuple, typename std::tuple_element<Index-1, SrcTuple>::type>::exec(tuple, std::get<Index-1>(src_tuple));
    }
};

template<typename Tuple, typename SrcTuple>
struct tuple_copy_helper<Tuple, SrcTuple, 0>
{
    tuple_copy_helper(Tuple&, SrcTuple const&) {}
};

template<typename... TupleTs, typename... Ts>
void tuple_copy(std::tuple<TupleTs...> const& src_tuple, std::tuple<Ts...>& dst_tuple)
{
    tuple_copy_helper<std::tuple<Ts...>, std::tuple<TupleTs...>>(dst_tuple, src_tuple);
}

/**
 * @brief create a tuple with types from both tuples, but guaranting that any type will
 *        appear only once
 */
template<typename Tuple1, typename Tuple2s>
struct unique_tuple;

template<typename... Tuple1s>
struct unique_tuple<std::tuple<Tuple1s...>, std::tuple<>>
{
    typedef std::tuple<Tuple1s...> type;
};

template<typename... Tuple1s, typename T, typename... Tuple2s>
struct unique_tuple<std::tuple<Tuple1s...>, std::tuple<T, Tuple2s...>>
{
    typedef typename unique_tuple<typename std::conditional<has_type<std::tuple<Tuple1s...>, T>::value
                                         , std::tuple<Tuple1s...>, std::tuple<Tuple1s..., T>>::type
              , std::tuple<Tuple2s...>>::type type;
};

/**
 * @brief produces an extended tuple type with params form all provided tuples
 *        All types must be unique.
 *        Earlier tuples in the parameter list take priority over those later
 *        where there is a type match betwen tuples
 */
template<typename Tuple1, typename Tuple2>
struct merge_tuples_type
{
    typedef typename unique_tuple<Tuple1, Tuple2>::type type;

    inline static
    type exec(Tuple1 const& tuple1, Tuple2 const& tuple2)
    {
        type r;
        tuple_copy(tuple2, r);
        tuple_copy(tuple1, r);
        return r;
    }
};

template<typename Tuple1, typename Tuple2>
typename merge_tuples_type<Tuple1, Tuple2>::type
merge_tuples(Tuple1 const& tuple1, Tuple2 const& tuple2)
{
    return merge_tuples_type<Tuple1, Tuple2>::exec(tuple1, tuple2);
}

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
