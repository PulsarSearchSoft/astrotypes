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

namespace std {
    // spceilisations for iterator_traits
template<typename SliceType>
struct iterator_traits<pss::astrotypes::SliceIterator<SliceType, false>>
{
    typedef typename SliceType::Parent::iterator iterator;
    typedef typename iterator::reference reference;
    typedef typename iterator::value_type value_type;
    typedef typename iterator::pointer pointer;
    typedef typename iterator::difference_type difference_type;
};

template<typename SliceType>
struct iterator_traits<pss::astrotypes::SliceIterator<SliceType, true>>
{
    typedef typename SliceType::Parent::const_iterator iterator;
    typedef typename iterator::reference reference;
    typedef typename iterator::value_type value_type;
    typedef typename iterator::pointer pointer;
    typedef typename iterator::difference_type difference_type;
};
} // namespace std

namespace pss {
namespace astrotypes {
//template<typename ParentT, typename Dimension, typename... Dimensions>
//class Slice;

/**
 * @brief Class to Iterate over a Slice
 * @details
 */

template<typename DerivedType, typename SliceType, bool is_const>
class SliceIteratorBase : public std::iterator_traits<std::forward_iterator_tag>
{
        typedef SliceIteratorBase<DerivedType, SliceType, is_const> SelfType;
        typedef typename std::iterator_traits<DerivedType>::iterator parent_iterator;
        typedef typename std::iterator_traits<parent_iterator>::reference reference;
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;

        template<typename D, typename S, bool>
        friend class SliceIteratorBase;

    public:
        SliceIteratorBase(SliceT&);
        ~SliceIteratorBase();

        DerivedType& operator++();
        DerivedType& operator++(int);

        template<typename D, bool const_val>
        bool operator==(SliceIteratorBase<D, SliceType, const_val> const&) const;

        template<typename D, bool const_val>
        bool operator!=(SliceIteratorBase<D, SliceType, const_val> const&) const;

        /// dereference operator
        const reference operator*() const;

        /// create an end iterator for the givn slice
        static DerivedType create_end(SliceT& slice);

    protected:
        SliceT& _slice;
        parent_iterator _current;
        parent_iterator _end;
        parent_iterator _offset;
};

template<typename SliceType, bool is_const>
class SliceIterator : public SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const>
{
        typedef SliceIteratorBase<SliceIterator<SliceType, is_const>, SliceType, is_const> BaseT;
        typedef typename std::conditional<is_const, SliceType const, SliceType>::type SliceT;
        friend SliceType;

    public:
        SliceIterator(SliceT&);
};

} // namespace astrotypes
} // namespace pss
#include "detail/SliceIterator.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H
