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
#ifndef PSS_ASTROTYPES_MULTIARRAY_SLICE_H
#define PSS_ASTROTYPES_MULTIARRAY_SLICE_H
#include "DimensionIndex.h"
#include <utility>

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      Representation of a Slice through a Data Array
 * @details
 */

template<typename Parent, typename Dimension, typename... Dimensions>
class Slice
{
        typedef Slice<Parent, Dimension, Dimensions...> SelfType;
        typedef typename Parent::value_type value_type;
        typedef typename Parent::reference_type reference_type;

    public:
        Slice( std::pair<DimensionIndex<Dimension>, DimensionIndex<Dimension>> const&
             , std::pair<DimensionIndex<Dimensions>, DimensionIndex<Dimensions>> const&...
        );
        ~Slice();

        /**
         * @brief Take a slice in the specified dimension one channel thick
         */
        Slice<Parent, Dimensions...> operator[](std::size_t) const;

    private:
        Parent& _parent;
};

// specialisation for 0 dimensional slice
// which should return a single element
template<typename Parent, typename Dimension>
class Slice<Parent, Dimension>
{
        typedef typename Parent::reference_type reference_type;

    public:
        Slice(Parent& parent, std::pair<DimensionIndex<Dimension>, DimensionIndex<Dimension>> const&);
        ~Slice();

        /**
         * @ brief return the size of the slice in the specified dimension
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type 
        size() const;

        /**
         * @ brief return the size of the slice in the specified dimension (will always be zero)
         */
        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type 
        size() const;

        /**
         * @ brief return the value pointer to
         */
        reference_type operator[](std::size_t) const;

    private:
        Parent& _parent;
        DimensionSize<Dimension> _span;
        typename Parent::iterator _ptr;   // start, to extent _ptr + _span
};

} // namespace astrotypes
} // namespace pss

#include "detail/Slice.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICE_H
