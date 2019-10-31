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
#ifndef PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSPAN_H
#define PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSPAN_H

#include "DimensionSize.h"
#include "DimensionIndex.h"

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      Defines a contiguous range over dimension in index
 *
 * @details
 */

template<typename Dimension>
class DimensionSpan
{
    public:
        DimensionSpan(DimensionIndex<Dimension> start_index, DimensionIndex<Dimension> end_index);
        DimensionSpan(DimensionIndex<Dimension> start_index, DimensionSize<Dimension> size);
        /// convenience operator, start_index will be set to 0
        DimensionSpan(DimensionSize<Dimension> size);

        inline DimensionIndex<Dimension>&         start() { return _start_index; }
        inline DimensionIndex<Dimension> const&   start() const { return _start_index; };
        inline void                               start(DimensionIndex<Dimension> s) { _start_index = s; }
        inline DimensionSize<Dimension>&          span() { return _span; };
        inline DimensionSize<Dimension> const&    span() const { return _span; };
        inline void                               span(DimensionSize<Dimension> s) { _span = s; }

        /// ensure the span fits within the size provided
        inline DimensionSpan<Dimension>&          trim(DimensionSize<Dimension> bounds);

        bool operator==(DimensionSpan<Dimension> const&) const;
        bool operator!=(DimensionSpan<Dimension> const&) const;

    private:
        DimensionIndex<Dimension> _start_index;
        DimensionSize<Dimension>  _span;
};


} // namespace astrotypes
} // namespace pss
#include "detail/DimensionSpan.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSPAN_H
