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
namespace multiarray {

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
template<typename Dim, typename>
constexpr
DimensionSize<ExcludedDim> ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::dimension()
{
    return DimensionSize<Dim>(1);
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value, DimensionSize<Dim>>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::dimension() const
{
    return BaseT::template dimension<Dim>();
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value
                      , ReducedRankSlice<typename SliceMixinRemover<typename SliceBaseType::template OperatorSliceBaseType<Dim>::type>::type, ExcludedDim>>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::operator[](DimensionIndex<Dim> dimension)
{
    static_assert(RankT == 0, "not yet implemented");
    return static_cast<BaseT&>(*this)[dimension];
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value
                      , ReducedRankSlice<typename SliceMixinRemover<typename SliceBaseType::template ConstOperatorSliceBaseType<Dim>::type>::type, ExcludedDim>>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::operator[](DimensionIndex<Dim> dimension) const
{
    static_assert(RankT == 0, "not yet implemented");
    return static_cast<BaseT const&>(*this)[dimension];
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::iterator ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::begin()
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_begin();
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::begin() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_begin();
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::cbegin() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_cbegin();
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::iterator ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::end()
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_end();
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::end() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_end();
}

template<typename SliceBaseType, typename ExcludedDim, std::size_t RankT>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, RankT>::cend() const
{
    static_assert(RankT == 0, "not yet implemented");
    return BaseT::impl_cend();
}

///// ------------ Rank 1 ----------------------------------------------------------------
template<typename SliceBaseType, typename ExcludedDim>
template<typename Dim>
typename std::enable_if<!has_type<typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::ExcludeTuple, Dim>::value
                      , typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::reference_type>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::operator[](DimensionIndex<Dim> dimension)
{
   return *static_cast<BaseT&>(*this)[dimension].begin();
}

template<typename SliceBaseType, typename ExcludedDim>
template<typename Dim>
typename std::enable_if<!has_type<typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::ExcludeTuple, Dim>::value
                      , typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::const_reference_type>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::operator[](DimensionIndex<Dim> dimension) const
{
   return *static_cast<BaseT const&>(*this)[dimension].begin();
}

template<typename SliceBaseType, typename ExcludedDim>
template<typename Dim>
inline
typename std::enable_if<has_type<typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::ExcludeTuple, Dim>::value
                      , typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::SelfType&>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::operator[](DimensionIndex<Dim>)
{
   return static_cast<SelfType&>(*this);
}

template<typename SliceBaseType, typename ExcludedDim>
template<typename Dim>
inline
typename std::enable_if<has_type<typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::ExcludeTuple, Dim>::value
                      , typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::SelfType const&>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::operator[](DimensionIndex<Dim>) const
{
   return static_cast<SelfType const&>(*this);
}

template<typename SliceBaseType, typename ExcludedDim>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::iterator ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::begin()
{
    return BaseT::impl_begin();
}

template<typename SliceBaseType, typename ExcludedDim>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::begin() const
{
    return BaseT::impl_begin();
}

template<typename SliceBaseType, typename ExcludedDim>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::cbegin() const
{
    return BaseT::impl_cbegin();
}

template<typename SliceBaseType, typename ExcludedDim>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::iterator ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::end()
{
    return BaseT::impl_end();
}

template<typename SliceBaseType, typename ExcludedDim>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::end() const
{
    return BaseT::impl_end();
}

template<typename SliceBaseType, typename ExcludedDim>
typename ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::const_iterator ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::cend() const
{
    return BaseT::impl_cend();
}

template<typename SliceBaseType, typename ExcludedDim>
template<typename Dim, typename>
constexpr
DimensionSize<ExcludedDim> ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::dimension()
{
    return DimensionSize<ExcludedDim>(1);
}

template<typename SliceBaseType, typename ExcludedDim>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value, DimensionSize<Dim>>::type
ReducedRankSlice<SliceBaseType, ExcludedDim, 1>::dimension() const
{
    return BaseT::template dimension<Dim>();
}

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
