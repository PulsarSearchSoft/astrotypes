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
#ifndef PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H
#define PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H

#include "Slice.h"
#include "OverlaySliceIterator.h"
#include "detail/SlicePosition.h"
#include <utility>
#include <iterator>

namespace pss {
namespace astrotypes {
    // pre declarations
    template<bool is_const, typename ParentT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
    class Slice;

    template<typename SliceType, bool is_const> class SliceIterator;

    namespace multiarray {
        template<typename T, typename D, std::size_t> class ReducedRankSlice;
    } // namespace multiarray

} // namespace pss
} // namespace astrotypes

namespace pss {
namespace astrotypes {

/**
 * @brief Class to Iterate over a Slice
 * @details
 */

template<typename DerivedType, typename SliceType, bool is_const, int Rank>
class SliceIteratorBase : public SliceIteratorBase<DerivedType, SliceType, is_const, 1>
{
    private:
        template<typename D, typename S, bool, int>
        friend class SliceIteratorBase;

        typedef SliceIteratorBase<DerivedType, SliceType, is_const, 1> BaseT;
        typedef SliceIteratorBase<DerivedType, SliceType, is_const, SliceType::rank> SelfType;
        typedef typename std::conditional<is_const, typename SliceType::Parent::const_iterator, typename SliceType::Parent::iterator>::type parent_iterator;

    protected:
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;
        typedef typename BaseT::ImplT ImplT; // to allow friend decalariont

    public:
        typedef typename std::iterator_traits<parent_iterator>::value_type value_type;
        typedef typename std::iterator_traits<parent_iterator>::reference reference;
        typedef typename std::iterator_traits<parent_iterator>::pointer pointer;
        typedef typename std::iterator_traits<parent_iterator>::difference_type difference_type;
        typedef typename std::forward_iterator_tag iterator_category;

    public:
        SliceIteratorBase(SliceT&);
        ~SliceIteratorBase();

        DerivedType& operator++();

        difference_type operator-(SelfType const&) const;

        SliceT const& slice() const;

    protected:
        SliceT& _slice;
        SlicePosition<SliceT::rank>  _pos;
};

template<typename DerivedType, typename SliceType, bool is_const>
class SliceIteratorBase<DerivedType, SliceType, is_const, 1>
{
        template<typename D, typename S, bool, int>
        friend class SliceIteratorBase;

        typedef SliceIteratorBase<DerivedType, SliceType, is_const, 1> SelfType;
        typedef typename std::conditional<is_const, typename SliceType::Parent::const_iterator, typename SliceType::Parent::iterator>::type parent_iterator;

    protected:
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;
        typedef SelfType ImplT; // to allow friend decalariont

    public:
        typedef typename std::iterator_traits<parent_iterator>::value_type value_type;
        typedef typename std::iterator_traits<parent_iterator>::reference reference;
        typedef typename std::iterator_traits<parent_iterator>::pointer pointer;
        typedef typename std::iterator_traits<parent_iterator>::difference_type difference_type;
        typedef typename std::iterator_traits<parent_iterator>::iterator_category iterator_category;

    public:
        SliceIteratorBase(SliceT&);
        ~SliceIteratorBase();

        DerivedType& operator++();
        DerivedType& operator++(int);

        template<typename D, bool const_val>
        bool operator==(SliceIteratorBase<D, SliceType, const_val, 1> const&) const;

        template<typename D, bool const_val>
        bool operator!=(SliceIteratorBase<D, SliceType, const_val, 1> const&) const;

        /// dereference operator
        const reference operator*() const;

        /** @brief return equivalent location in an alternative object to which the Slice corresponds
         *  @code
         *     DimensionSize<A> a_size(10);
         *     DimensionSize<B> b_size(100);
         *     MultiArray<int, A, B> my_object(a_size, b_size);
         *     MultiArray<bool, A, B> my_other_object(a_size, b_size);
         *
         *     DimensionSpan<A> span(DimensionIndex<A>(8), DimensionSize<A>(10));
         *     auto slice = my_object.slice(span);
         *     *slice.begin() = 255;
         *     *slice.begin()(my_other_object) = true;
         *  @endcode
         */
        template<typename DataT>
        inline OverlaySliceIterator<DataT, DerivedType> operator()(DataT&) const;

        /// create an end iterator for the given slice
        static DerivedType create_end(SliceT& slice);

        difference_type operator-(SelfType const&) const;

    protected:
        parent_iterator _current;
};

template<typename SliceType, bool is_const>
class SliceIterator;

template<bool is_const, typename ParentT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions, bool is_const2>
class SliceIterator<Slice<is_const2, ParentT, SliceMixin, Dimension, Dimensions...>, is_const> 
    : public SliceIteratorBase<SliceIterator<Slice<is_const2, ParentT, SliceMixin, Dimension, Dimensions...>, is_const>
                                           , Slice<is_const2, ParentT, SliceMixin, Dimension, Dimensions...>, is_const
                                           , Slice<is_const2, ParentT, SliceMixin, Dimension, Dimensions...>::rank>
{
    protected:
        typedef Slice<is_const2, ParentT, SliceMixin, Dimension, Dimensions...> SliceType;
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;

    public:
        // made available for Fried definitions in Slice class
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, SliceType::rank> BaseT;
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, 1> ImplT;

    public:
        SliceIterator(SliceT& s) : BaseT(s) {}
        SliceIterator(BaseT const& b) : BaseT(b) {}
        SliceIterator(BaseT&& b) : BaseT(std::forward<BaseT>(b)) {}
};

// handle mixin types
template<bool is_const, typename ParentT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions, bool is_const2>
class SliceIterator<SliceMixin<Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>>, is_const2> : public SliceIteratorBase<SliceIterator<SliceMixin<Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>>, is_const2>, Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>, is_const2, Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>::rank>
{
    protected:
        typedef Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...> SliceType;
        typedef typename std::conditional<is_const2, SliceType const, SliceType>::type SliceT;

    public:
        typedef SliceIteratorBase<SliceIterator<SliceMixin<SliceType>, is_const2>, SliceType, is_const2, SliceType::rank> BaseT;
        typedef SliceIteratorBase<SliceIterator<SliceMixin<SliceType>, is_const2>, SliceType, is_const2, 1> ImplT;

    public:
        SliceIterator(SliceT& s) : BaseT(s) {}
};

template<typename T, std::size_t B, typename SliceType, bool is_const>
class SliceIterator<multiarray::ReducedRankSlice<SliceType, T, B>, is_const> : public SliceIterator<SliceType, is_const>
{
        typedef SliceIterator<SliceType, is_const> ActualBaseT;

    protected:
        typedef typename ActualBaseT::SliceT SliceT;  // Inner level of mixin

    public:
        typedef typename ActualBaseT::BaseT BaseT; // required for friend access
        typedef typename ActualBaseT::ImplT ImplT; // required for friend aceess

    public:
        SliceIterator(SliceT& s);
        SliceIterator(BaseT const& b) : ActualBaseT(b) {}
        SliceIterator(BaseT&& b) : ActualBaseT(std::forward<BaseT>(b)) {}

        SliceIterator& operator++();
        SliceIterator& operator++(int);

        static SliceIterator create_end(SliceT& slice) { return SliceIterator(ActualBaseT::create_end(slice)); }
};

// This is a wrapper class that ensures we return the types we expect to see
template<template<typename, typename...> class Mixin, typename... Ts, typename SliceType, bool is_const>
class SliceIterator<Mixin<SliceType, Ts...>, is_const> : public SliceIterator<SliceType, is_const>
{
        typedef SliceIterator<SliceType, is_const> ActualBaseT;

    protected:
        typedef typename ActualBaseT::SliceT SliceT;  // Inner level of mixin

    public:
        typedef typename ActualBaseT::BaseT BaseT; // required for friend access
        typedef typename ActualBaseT::ImplT ImplT; // required for friend aceess

    public:
        SliceIterator(SliceT& s) : ActualBaseT(s) {};
        SliceIterator(BaseT const& b) : ActualBaseT(b) {}
        SliceIterator(BaseT&& b) : ActualBaseT(std::forward<BaseT>(b)) {}

        SliceIterator& operator++();
        SliceIterator& operator++(int);

        static SliceIterator create_end(SliceT& slice) { return SliceIterator(ActualBaseT::create_end(slice)); }
};

/*
 * @brief remove any mixins from the tyeps list
 */
template<typename SliceType>
struct SliceMixinRemover
{
    typedef SliceType type;
};

template<typename SliceType, typename D, std::size_t Rank>
struct SliceMixinRemover<multiarray::ReducedRankSlice<SliceType, D, Rank>>
{
    typedef multiarray::ReducedRankSlice<typename SliceMixinRemover<SliceType>::type, D, Rank> type;
};

template<template<typename> class Mixin, typename SliceType>
struct SliceMixinRemover<Mixin<SliceType>>
{
    typedef typename SliceMixinRemover<SliceType>::type type;
};

/*
 * @brief add layers in the mixin structure of the SliceType
 */
template<typename ReplacementSliceType, typename SliceIterator>
struct SliceIteratorHelper;

template<typename ReplacementSliceType
        , typename SliceType
        , bool is_const>
struct SliceIteratorHelper<ReplacementSliceType, SliceIterator<SliceType, is_const>>
{
    typedef SliceIterator<ReplacementSliceType, is_const> type;
};


// remove the SliceMixin
template<typename ReplacementSliceType
        , template<typename> class SliceMixin
        , typename SliceType
        , bool is_const>
struct SliceIteratorHelper<ReplacementSliceType, SliceIterator<SliceMixin<SliceType>, is_const>>
{
    typedef SliceIterator<SliceMixin<ReplacementSliceType>, is_const> type;
};

} // namespace astrotypes
} // namespace pss
#include "detail/SliceIterator.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H
