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
#ifndef PSS_ASTROTYPES_MULTIARRAY_REDUCEDRANKSLICE_H
#define PSS_ASTROTYPES_MULTIARRAY_REDUCEDRANKSLICE_H
#include <tuple>

namespace pss {
namespace astrotypes {
namespace multiarray {
/*
 * @brief Helper Class for Slice. Provides an interface for slices manipulated with operator[] that is consistent
 *        whichever dimension order they are called
 * @details not intended for public use
 */

template<typename SliceType, typename ExcludedDim, std::size_t RankT=SliceType::rank - 1>
class ReducedRankSlice : public SliceType
{
        typedef SliceType BaseT;

    protected:
        typedef typename join_tuples<typename BaseT::ExcludeTuple, std::tuple<ExcludedDim>>::type ExcludeTuple;
        typedef typename tuple_diff<typename SliceType::DimensionTuple, ExcludeTuple>::type DimensionTuple;
        typedef typename SliceIteratorHelper<ReducedRankSlice, typename SliceType::iterator>::type iterator;
        typedef typename SliceIteratorHelper<ReducedRankSlice, typename SliceType::const_iterator>::type const_iterator;
        friend typename iterator::BaseT;
        friend typename iterator::ImplT;
        friend typename const_iterator::BaseT;
        friend typename const_iterator::ImplT;

    public:
        static constexpr std::size_t rank = SliceType::rank - 1;

    public:
        ReducedRankSlice(SliceType const& s) : SliceType(s) {}
        ReducedRankSlice(SliceType&& s) : SliceType(std::forward<SliceType>(s)) {}

        /**
         * @brief iterator pointing to the first element in the slice
         */
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value
                               , ReducedRankSlice<typename SliceMixinRemover<typename SliceType::template OperatorSliceType<Dim>::type>::type, ExcludedDim>>::type
        operator[](DimensionIndex<Dim>);

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, ExcludedDim>::value
                               , ReducedRankSlice<typename SliceMixinRemover<typename SliceType::template ConstOperatorSliceType<Dim>::type>::type, ExcludedDim>>::type
        operator[](DimensionIndex<Dim>) const;

        /**
         * @brief the width of the slice in this dimesnion has been reduced to 1
         */
        template<typename Dim>
        static
        typename std::enable_if<std::is_same<Dim, ExcludedDim>::value, std::size_t>::type
        dimension();
};

template<typename SliceType, typename ExcludedDim>
class ReducedRankSlice<SliceType, ExcludedDim, 1> : public SliceType
{
        typedef SliceType BaseT;
        typedef typename join_tuples<typename BaseT::ExcludeTuple, std::tuple<ExcludedDim>>::type ExcludeTuple;

    public:
        typedef typename astrotypes::tuple_diff<typename SliceType::DimensionTuple, ExcludeTuple>::type DimensionTuple;
        typedef typename std::iterator_traits<typename SliceType::iterator>::reference reference_type;
        typedef typename std::iterator_traits<typename SliceType::Parent::const_iterator>::reference const_reference_type;
        typedef typename SliceIteratorHelper<ReducedRankSlice, typename SliceType::iterator>::type iterator;
        typedef typename SliceIteratorHelper<ReducedRankSlice, typename SliceType::const_iterator>::type const_iterator;

    private:
        friend typename iterator::BaseT;
        friend typename iterator::ImplT;
        friend typename const_iterator::BaseT;
        friend typename const_iterator::ImplT;

    public:
        ReducedRankSlice(SliceType const& s) : BaseT(s) {}
        ReducedRankSlice(SliceType&& s) : BaseT(std::move(s)) {}

        /**
         * @brief iterator pointing to the first element in the slice
         */
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        /**
         * @brief return the values rather than an iterator type
         */
        template<typename Dim>
        typename std::enable_if<!has_type<ExcludeTuple, Dim>::value
                               , reference_type>::type
        operator[](DimensionIndex<Dim>);

        template<typename Dim>
        typename std::enable_if<!has_type<ExcludeTuple, Dim>::value
                               , const_reference_type>::type
        operator[](DimensionIndex<Dim>) const;

        /**
         * @brief the width of the slice in this dimesnion has been reduced to 1
         */
        template<typename Dim>
        static
        typename std::enable_if<std::is_same<Dim, ExcludedDim>::value, std::size_t>::type
        dimension();
};

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
#include "ReducedRankSlice.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_REDUCEDRANKSLICE_H
