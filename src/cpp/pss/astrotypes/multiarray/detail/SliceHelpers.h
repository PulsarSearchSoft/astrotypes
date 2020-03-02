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
#ifndef PSS_ASTROTYPES_MULTIARRAY_SLICEHELPERS_H
#define PSS_ASTROTYPES_MULTIARRAY_SLICEHELPERS_H

/**
 * @brief Implementation details. Helper class defintions.
 */

namespace pss {
namespace astrotypes {
namespace multiarray {

/**
 * @brief const_cast helper between different slice types
 */
template<typename SliceType, typename Enable=void>
struct FlipConstType;

// internal helper class for FlipConstType
template<typename SliceType>
struct FlipConstTypeHelper;

template<typename SliceT>
struct CastToConstSliceType;

template<typename SliceT>
struct CastToNonConstSliceType;

/**
 * @brief removes the Mixin wrapper decorator to get at the Slice type below
 */
template<typename SliceType>
struct RemoveMixinWrapper;

/**
 * @brief restore the Mixin wrapper for a SliceType
 */
template<typename SliceType>
struct RestoreMixinWrapper;

/**
 * @brief flip a booleans value
 */
template<bool Value>
struct Flip
{
    constexpr static bool const value = !Value;
};
static_assert(Flip<true>::value==false, "Flip broken");
static_assert(Flip<false>::value==true, "Flip broken");

/**
 * @brief extract types from an arbitrary ordered argument list
 */
template<typename T, typename... Dims>
struct arg_helper : public std::false_type
{
};

template<typename T, typename... Dims>
struct arg_helper<T, T, Dims...> : public std::true_type
{
    static inline
    T&& arg(T&& arg, Dims&&...)
    { return std::forward<T>(arg); }
};

template<typename T, typename Dim, typename... Dims>
struct arg_helper<T, Dim, Dims...> : public arg_helper<T, Dims...>
{
    typedef arg_helper<T, Dims...> BaseT;

    static inline
    T&& arg(Dim&&, Dims&&...args)
    { return BaseT::arg(std::forward<Dims>(args)...); }
};


template<typename ParentT, typename ExcludeDimension>
struct InternalSliceTraits  {
    typedef ParentT Parent;
    typedef std::tuple<ExcludeDimension> ExcludeTuple;
};

template<typename Traits, typename ExcludeDimension, typename D>
struct InternalSliceTraits<InternalSliceTraits<Traits, D>, ExcludeDimension> : public InternalSliceTraits<Traits, D>
{
    private:
        typedef InternalSliceTraits<Traits, D> BaseT;
    public:
        typedef typename BaseT::Parent Parent;
        typedef typename join_tuples<typename BaseT::ExcludeTuple, std::tuple<ExcludeDimension>>::type ExcludeTuple;
};

template<typename TraitsT>
struct SliceTraitsHelper
{
    typedef std::tuple<> ExcludeTuple;
    typedef TraitsT Parent;
};

template<typename TraitsT, typename D>
struct SliceTraitsHelper<InternalSliceTraits<TraitsT, D>>
{
    typedef typename InternalSliceTraits<TraitsT, D>::ExcludeTuple ExcludeTuple;
    typedef typename InternalSliceTraits<TraitsT, D>::Parent Parent;
};

} // namespace multiarray
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICEHELPERS_H
