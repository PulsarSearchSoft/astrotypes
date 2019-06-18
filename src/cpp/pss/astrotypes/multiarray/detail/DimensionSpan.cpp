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

namespace pss {
namespace astrotypes {

template<typename Dimension>
DimensionSpan<Dimension>::DimensionSpan(DimensionIndex<Dimension> start_index, DimensionIndex<Dimension> end_index)
    : _start_index(start_index)
    , _span(end_index - start_index + 1)
{
}

template<typename Dimension>
DimensionSpan<Dimension>::DimensionSpan(DimensionIndex<Dimension> start_index, DimensionSize<Dimension> size)
    : _start_index(start_index)
    , _span(size)
{
}

template<typename Dimension>
DimensionSpan<Dimension>::DimensionSpan(DimensionSize<Dimension> size)
    : _start_index(0)
    , _span(size)
{
}

template<typename Dimension>
DimensionSpan<Dimension>& DimensionSpan<Dimension>::trim(DimensionSize<Dimension> bounds)
{
    if(_start_index >= bounds) {
        _start_index = DimensionIndex<Dimension>((std::size_t)bounds);
        _span = DimensionSize<Dimension>(0);
        return *this;
    }
    DimensionSize<Dimension> delta = bounds - _start_index;
    if(_span > delta) _span = delta;
    return *this;
}

} // namespace astrotypes
} // namespace pss
