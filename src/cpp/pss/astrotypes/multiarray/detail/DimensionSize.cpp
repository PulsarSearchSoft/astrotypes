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
DimensionSize<Dimension>::DimensionSize(std::size_t size)
    : _size(size)
{
}

template<typename Dimension>
DimensionSize<Dimension>::~DimensionSize()
{
}

template<typename Dimension>
DimensionSize<Dimension>::operator std::size_t()
{
    return _size;
}

template<typename Dimension>
DimensionSize<Dimension>::operator std::size_t() const
{
    return _size;
}

template<typename Dimension>
DimensionSize<Dimension>& DimensionSize<Dimension>::operator=(std::size_t s)
{
    _size = s;
    return *this;
}

template<typename Dimension>
bool DimensionSize<Dimension>::operator<(DimensionSize<Dimension> const& b) const
{
    return _size < b._size;
}

template<typename Dimension>
template<typename OtherDimension>
bool DimensionSize<Dimension>::operator==(DimensionSize<OtherDimension> const& b) const
{
    return _size == static_cast<std::size_t>(b);
}

template<typename Dimension>
template<typename OtherDimension>
bool DimensionSize<Dimension>::operator!=(DimensionSize<OtherDimension> const& b) const
{
    return _size != static_cast<std::size_t>(b);
}

template<typename Dimension>
DimensionSize<Dimension>& DimensionSize<Dimension>::operator+=(DimensionSize<Dimension> const& b)
{
    _size += b._size;
    return *this;
}

template<typename Dimension>
template<typename OtherDimension>
std::size_t DimensionSize<Dimension>::operator*(DimensionSize<OtherDimension> const& b) const
{
    return _size * static_cast<std::size_t>(b);
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator*(std::size_t b) const
{
    return DimensionSize<Dimension>(_size * b);
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator/(std::size_t b) const
{
    return DimensionSize<Dimension>(_size / b);
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator+(std::size_t b) const
{
    return DimensionSize<Dimension>(_size + b);
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator-(std::size_t b) const
{
    return DimensionSize<Dimension>(_size - b);
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator+(DimensionSize<Dimension> const& b) const
{
    return DimensionSize<Dimension>(_size + b._size);
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator-(DimensionSize<Dimension> const& b) const
{
    return DimensionSize<Dimension>(_size - b._size);
}

template<typename Dimension>
DimensionSize<Dimension>& DimensionSize<Dimension>::operator++()
{
    ++_size;
    return *this;
}

template<typename Dimension>
DimensionSize<Dimension> DimensionSize<Dimension>::operator++(int)
{
    DimensionSize tmp(_size++);
    return tmp;
}
} // namespace astrotypes
} // namespace pss
