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
#ifndef PSS_ASTROTYPES_MULTIARRAY_OVERLAYSLICEITERATOR_H
#define PSS_ASTROTYPES_MULTIARRAY_OVERLAYSLICEITERATOR_H
#include <type_traits>
#include <utility>
#include <iterator>

namespace pss {
namespace astrotypes {

/**
 * @brief Overlays an iterator from one data structure onto that of another
 * @details allows a single iterator to be used to iterate through multiple objects
 *          saving computational costs
 */
template<typename IterableType, typename SliceIteratorType>
class OverlaySliceIterator
{
        typedef typename std::conditional<std::is_const<IterableType>::value
                                         , typename IterableType::const_iterator
                                         , typename IterableType::iterator>::type parent_iterator;
    public:
        typedef typename std::iterator_traits<parent_iterator>::value_type value_type;
        typedef typename std::iterator_traits<parent_iterator>::reference reference;
        typedef typename std::iterator_traits<parent_iterator>::pointer pointer;

    public:
        OverlaySliceIterator(IterableType& data, SliceIteratorType const& slice_it);

        inline reference operator*() const;

    private:
        parent_iterator _parent_iterator;
};

} // namespace astrotypes
} // namespace pss
#include "detail/OverlaySliceIterator.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_OVERLAYSLICEITERATOR_H
