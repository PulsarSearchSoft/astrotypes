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
#include "ReducedRankSlice.h"
#include "../TypeTraits.h"


namespace pss {
namespace astrotypes {

template<typename SliceType, typename ExcludedDim, std::size_t RankT, typename Dimension>
struct has_dimension_strict<multiarray::ReducedRankSlice<SliceType, ExcludedDim, RankT>, Dimension>
    : public has_dimension_strict<SliceType, Dimension>
{
};

template<typename SliceType, typename ExcludedDim, std::size_t RankT, typename Dimension, typename... Dimensions>
struct has_exact_dimensions<multiarray::ReducedRankSlice<SliceType, ExcludedDim, RankT>, Dimension, Dimensions...>
    : public std::is_same<typename multiarray::ReducedRankSlice<SliceType, ExcludedDim, RankT>::DimensionTuple, std::tuple<Dimension, Dimensions...>>::type
{
};

template<typename SliceType, typename ExcludedDim, std::size_t RankT, typename Dimension, typename... Dimensions, template<typename> class SliceMixin>
struct has_exact_dimensions<SliceMixin<multiarray::ReducedRankSlice<SliceType, ExcludedDim, RankT>>, Dimension, Dimensions...>
    : public has_exact_dimensions<multiarray::ReducedRankSlice<SliceType, ExcludedDim, RankT>, Dimension, Dimensions...>::type
{
};

namespace multiarray {

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, ExcludedDim>::value, std::size_t>::type
ReducedRankSlice<SliceType, ExcludedDim, RankT>::dimension()
{
    return 1;
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value
                      , ReducedRankSlice<typename SliceMixinRemover<typename SliceType::template OperatorSliceType<Dim>::type>::type, ExcludedDim>>::type
ReducedRankSlice<SliceType, ExcludedDim, RankT>::operator[](DimensionIndex<Dim> dimension)
{
    static_assert(RankT == 0, "not yet implemented");
    return static_cast<BaseT&>(*this)[dimension];
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value
                      , ReducedRankSlice<typename SliceMixinRemover<typename SliceType::template ConstOperatorSliceType<Dim>::type>::type, ExcludedDim>>::type
ReducedRankSlice<SliceType, ExcludedDim, RankT>::operator[](DimensionIndex<Dim> dimension) const
{
    static_assert(RankT == 0, "not yet implemented");
    return static_cast<BaseT const&>(*this)[dimension];
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceType, ExcludedDim, RankT>::iterator ReducedRankSlice<SliceType, ExcludedDim, RankT>::begin()
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_begin();
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, RankT>::begin() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_begin();
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, RankT>::cbegin() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_cbegin();
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceType, ExcludedDim, RankT>::iterator ReducedRankSlice<SliceType, ExcludedDim, RankT>::end()
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_end();
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, RankT>::end() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_end();
}

template<typename SliceType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, RankT>::cend() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_cend();
}

///// ------------ Rank 1 ----------------------------------------------------------------
template<typename SliceType, typename ExcludedDim>
template<typename Dim>
typename std::enable_if<!has_type<typename ReducedRankSlice<SliceType, ExcludedDim, 1>::ExcludeTuple, Dim>::value
                      , typename ReducedRankSlice<SliceType, ExcludedDim, 1>::reference_type>::type
ReducedRankSlice<SliceType, ExcludedDim, 1>::operator[](DimensionIndex<Dim> dimension)
{
   return *static_cast<BaseT&>(*this)[dimension].begin();
}

template<typename SliceType, typename ExcludedDim>
template<typename Dim>
typename std::enable_if<!has_type<typename ReducedRankSlice<SliceType, ExcludedDim, 1>::ExcludeTuple, Dim>::value
                      , typename ReducedRankSlice<SliceType, ExcludedDim, 1>::const_reference_type>::type
ReducedRankSlice<SliceType, ExcludedDim, 1>::operator[](DimensionIndex<Dim> dimension) const
{
   return *static_cast<BaseT const&>(*this)[dimension].begin();
}

template<typename SliceType, typename ExcludedDim>
typename ReducedRankSlice<SliceType, ExcludedDim, 1>::iterator ReducedRankSlice<SliceType, ExcludedDim, 1>::begin()
{
    return BaseT::impl_begin();
}

template<typename SliceType, typename ExcludedDim>
typename ReducedRankSlice<SliceType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, 1>::begin() const
{
    return BaseT::impl_begin();
}

template<typename SliceType, typename ExcludedDim>
typename ReducedRankSlice<SliceType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, 1>::cbegin() const
{
    return BaseT::impl_cbegin();
}

template<typename SliceType, typename ExcludedDim>
typename ReducedRankSlice<SliceType, ExcludedDim, 1>::iterator ReducedRankSlice<SliceType, ExcludedDim, 1>::end()
{
    return BaseT::impl_end();
}

template<typename SliceType, typename ExcludedDim>
typename ReducedRankSlice<SliceType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, 1>::end() const
{
    return BaseT::impl_end();
}

template<typename SliceType, typename ExcludedDim>
typename ReducedRankSlice<SliceType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceType, ExcludedDim, 1>::cend() const
{
    return BaseT::impl_cend();
}

template<typename SliceType, typename ExcludedDim>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, ExcludedDim>::value, std::size_t>::type
ReducedRankSlice<SliceType, ExcludedDim, 1>::dimension()
{
    return 1;
}

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
