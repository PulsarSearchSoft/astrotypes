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
DimensionIndex<Dimension>::DimensionIndex(std::size_t index)
    : _index(index)
{
}

template<typename Dimension>
DimensionIndex<Dimension>::~DimensionIndex()
{
}

template<typename Dimension>
DimensionIndex<Dimension>::operator std::size_t&()
{
    return _index;
}

template<typename Dimension>
DimensionIndex<Dimension>::operator std::size_t() const
{
    return _index;
}

template<typename Dimension>
DimensionIndex<Dimension>& DimensionIndex<Dimension>::operator++()
{
    ++_index;
    return *this;
}

template<typename Dimension>
DimensionSize<Dimension> DimensionIndex<Dimension>::operator-(DimensionIndex<Dimension> const& b) const
{
    return DimensionSize<Dimension>(_index - b._index);
}

template<typename Dimension>
DimensionIndex<Dimension> DimensionIndex<Dimension>::operator+(DimensionSize<Dimension> const& s) const
{
    return DimensionIndex<Dimension>(_index + static_cast<std::size_t>(s));
}

template<typename Dimension>
bool DimensionIndex<Dimension>::operator<(DimensionIndex<Dimension> const& b) const
{
    return _index < b._index;
}

template<typename Dimension>
bool DimensionIndex<Dimension>::operator<(DimensionSize<Dimension> const& b) const
{
    return _index < static_cast<std::size_t>(b);
}

} // namespace astrotypes
} // namespace pss
