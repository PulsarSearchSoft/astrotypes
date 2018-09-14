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
#include "detail/SlicePosition.h"
#include <utility>
#include <iterator>

namespace pss {
namespace astrotypes {
    // pre declarations
    template<bool is_const, typename ParentT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
    class Slice;

    template<typename SliceType, bool is_const> class SliceIterator;

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
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;

    protected:
        typedef typename BaseT::ImplT ImplT; // to allow friend declaration

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
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;

    protected:
        typedef SelfType ImplT; // to allow friend declaration

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

        /// create an end iterator for the given slice
        static DerivedType create_end(SliceT& slice);

        difference_type operator-(SelfType const&) const;

    protected:
        parent_iterator _current;
};

template<typename SliceType, bool is_const>
class SliceIterator : public SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, SliceType::rank>
{
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;

    public:
        // made available for Fried definitions in Slice class
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, SliceType::rank> BaseT;
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, 1> ImplT;

    public:
        SliceIterator(SliceT&);
};


// handle mixin types
template<bool is_const, typename ParentT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions, bool is_const2>
class SliceIterator<SliceMixin<Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>>, is_const2> : public SliceIteratorBase<SliceIterator<SliceMixin<Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>>, is_const2>, Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>, is_const2, Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...>::rank>
{
        typedef Slice<is_const, ParentT, SliceMixin, Dimension, Dimensions...> SliceType;
        typedef typename std::conditional<is_const2, SliceType const, SliceType>::type SliceT;

    public:
        typedef SliceIteratorBase<SliceIterator<SliceMixin<SliceType>, is_const2>, SliceType, is_const2, SliceType::rank> BaseT;
        typedef SliceIteratorBase<SliceIterator<SliceMixin<SliceType>, is_const2>, SliceType, is_const2, 1> ImplT;

    public:
        SliceIterator(SliceT& s) : BaseT(s) {}
};

} // namespace astrotypes
} // namespace pss
#include "detail/SliceIterator.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H
