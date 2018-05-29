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
    template<typename SliceType> class SliceIterator;

} // namespace pss
} // namespace astrotypes

namespace std {
    // spceilisations for iterator_traits
template<typename SliceType>
struct iterator_traits<pss::astrotypes::SliceIterator<SliceType>>
{
    typedef typename SliceType::Parent::iterator iterator;
    typedef typename iterator::reference reference;
    typedef typename iterator::value_type value_type;
    typedef typename iterator::pointer pointer;
    typedef typename iterator::difference_type difference_type;
};

template<typename SliceType>
struct iterator_traits<pss::astrotypes::SliceIterator<const SliceType>>
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
template<typename ParentT, typename Dimension, typename... Dimensions>
class Slice;

/**
 * @brief Class to Iterate over a Slice
 * @details
 */

template<typename DerivedType, typename SliceType>
class SliceIteratorBase
{
        typedef SliceIteratorBase<DerivedType, SliceType> SelfType;
        typedef typename std::iterator_traits<DerivedType>::reference reference;
        typedef typename std::iterator_traits<DerivedType>::iterator parent_iterator;

    public:
        SliceIteratorBase(SliceType&);
        ~SliceIteratorBase();

        DerivedType& operator++();
        DerivedType& operator++(int);

        template<typename D>
        bool operator==(SliceIteratorBase<D, SliceType> const&) const;

        template<typename D>
        bool operator!=(SliceIteratorBase<D, SliceType> const&) const;

        /// dereference operator
        const reference operator*() const;

        /// create an end iterator for the givn slice
        static DerivedType create_end(SliceType& slice);

    protected:
        SliceType& _slice;
        parent_iterator _current;
        parent_iterator _end;
};

template<typename SliceType>
class SliceIterator : public SliceIteratorBase<SliceIterator<SliceType>, SliceType>
{
        typedef SliceIteratorBase<SliceIterator<SliceType>, SliceType> BaseT;
        friend SliceType;

    public:
        typedef typename std::iterator_traits<SliceIterator<SliceType>>::reference reference;

    public:
        SliceIterator(SliceType&);


        /// dereference operators
        reference operator*();
};

/**
 * @brief SliceIterator specialisation for cont types
 */
template<typename SliceType>
class SliceIterator<const SliceType> : public SliceIteratorBase<SliceIterator<const SliceType>, SliceType>
{
        typedef SliceIteratorBase<SliceIterator<const SliceType>, SliceType> BaseT;
        friend SliceType;

    public:
        typedef typename std::iterator_traits<SliceIterator<SliceType>>::reference reference;

    public:
        SliceIterator(SliceType&);
};

/**
 * @brief construct a SliceIterator of the correct type for a provided slice object
 */
template<typename ParentT, typename... Dimensions>
SliceIterator<Slice<ParentT, Dimensions...>> make_slice_iterator(Slice<ParentT, Dimensions...>& slice);

/**
 * @brief construct a SliceIterator of the correct type for a provided slice object positioned at the ned of the slice
 */
template<typename ParentT, typename... Dimensions>
SliceIterator<Slice<ParentT, Dimensions...>> make_end_slice_iterator(Slice<ParentT, Dimensions...>& slice);

} // namespace astrotypes
} // namespace pss
#include "detail/SliceIterator.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICEITERATOR_H
