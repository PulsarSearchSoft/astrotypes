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
#include <utility>
#include <iterator>

namespace pss {
namespace astrotypes {
    // pre declarations
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
        template<typename D, typename S, bool, int>
        friend class SliceIteratorBase;

        typedef SliceIteratorBase<DerivedType, SliceType, is_const, 1> BaseT;
        typedef SliceIteratorBase<DerivedType, SliceType, is_const, SliceType::rank> SelfType;
        typedef typename std::conditional<is_const, typename SliceType::Parent::const_iterator, typename SliceType::Parent::iterator>::type parent_iterator;
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;

    protected:
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

    protected:
        SliceT& _slice;
        parent_iterator _offset;
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
        typedef SelfType ImplT; // to allow friend decalariont

    public:
        typedef typename std::iterator_traits<parent_iterator>::value_type value_type;
        typedef typename std::iterator_traits<parent_iterator>::reference reference;
        typedef typename std::iterator_traits<parent_iterator>::pointer pointer;
        typedef typename std::iterator_traits<parent_iterator>::difference_type difference_type;
        typedef typename std::conditional<(SliceType::rank!=1), std::forward_iterator_tag, typename std::iterator_traits<parent_iterator>::iterator_category>::type iterator_category;

    public:
        SliceIteratorBase(SliceT&);
        ~SliceIteratorBase();

        DerivedType& operator++();
        DerivedType& operator++(int);

        template<typename D, bool const_val, int rank>
        bool operator==(SliceIteratorBase<D, SliceType, const_val, rank> const&) const;

        template<typename D, bool const_val, int rank>
        bool operator!=(SliceIteratorBase<D, SliceType, const_val, rank> const&) const;

        /// dereference operator
        const reference operator*() const;

        /// create an end iterator for the givn slice
        static DerivedType create_end(SliceT& slice);

        difference_type operator-(SelfType const&) const;

    protected:
        parent_iterator _current;
        parent_iterator _end;
};

template<typename SliceType, bool is_const>
class SliceIterator : public SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, SliceType::rank>
{
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;
        friend SliceType;

    public:
        // made availabel for Fried definitions in Slice class
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, SliceType::rank> BaseT;
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const, 1> ImplT;

    public:
        SliceIterator(SliceT&);
};

} // namespace astrotypes
} // namespace pss
#include "detail/SliceIterator.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H
