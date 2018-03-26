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

////////////////////////////////////////////////
// generic dimension implementation
////////////////////////////////////////////////
// public interface for ensuring all is correctly sized up
template<typename T, typename FirstDimension, typename... Dimensions>
MultiArray<T, FirstDimension, Dimensions...>::MultiArray(DimensionSize<FirstDimension> const& fd, DimensionSize<Dimensions> const& ... sizes)
    : BaseT(false, sizes...)
    , _size(fd)
{
    resize(fd);
}

// private interface for constructing in an inheritance stack
template<typename T, typename FirstDimension, typename... Dimensions>
MultiArray<T, FirstDimension, Dimensions...>::MultiArray(bool, DimensionSize<FirstDimension> const& fd, DimensionSize<Dimensions> const& ... sizes)
    : BaseT(false, sizes...)
    , _size(fd)
{
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::iterator MultiArray<T, FirstDimension, Dimensions...>::begin()
{
    return BaseT::begin();
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::const_iterator MultiArray<T, FirstDimension, Dimensions...>::begin() const
{
    return BaseT::begin();
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::const_iterator MultiArray<T, FirstDimension, Dimensions...>::cbegin() const
{
    return BaseT::cbegin();
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::iterator MultiArray<T, FirstDimension, Dimensions...>::end()
{
    return BaseT::end();
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::const_iterator MultiArray<T, FirstDimension, Dimensions...>::end() const
{
    return BaseT::end();
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::const_iterator MultiArray<T, FirstDimension, Dimensions...>::cend() const
{
    return BaseT::cend();
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::ReducedDimensionSliceType MultiArray<T, FirstDimension, Dimensions...>::operator[](DimensionIndex<FirstDimension> index)
{
    return SliceType(*this, DimensionSpan<FirstDimension>(index, DimensionSize<FirstDimension>(1))
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...)[0];
}

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::SliceType MultiArray<T, FirstDimension, Dimensions...>::slice(DimensionSpan<FirstDimension> const& index_range)
{
    return SliceType(*this, index_range
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...);
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
void MultiArray<T, FirstDimension, Dimensions...>::resize(DimensionSize<Dim> size)
{
    this->do_resize(1,  size);
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type 
MultiArray<T, FirstDimension, Dimensions...>::size() const
{
    return BaseT::template size<Dim>();
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type 
MultiArray<T, FirstDimension, Dimensions...>::size() const
{
    return _size;
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<T, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size), size);
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<T, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size)
{
    _size = size;
    BaseT::do_resize(total * static_cast<std::size_t>(_size), size);
}

/////////////////////////////////////////////////////////////
// Single Dimension specialisation 
/////////////////////////////////////////////////////////////
template<typename T, typename FirstDimension>
MultiArray<T, FirstDimension>::MultiArray(DimensionSize<FirstDimension> const& size)
    : _size(size)
{
    _data.resize(size);
}

template<typename T, typename FirstDimension>
MultiArray<T, FirstDimension>::MultiArray(bool, DimensionSize<FirstDimension> const& size)
    : _size(size)
{
}

template<typename T, typename FirstDimension>
MultiArray<T, FirstDimension>::~MultiArray()
{
}

template<typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type 
MultiArray<T, FirstDimension>::size() const
{
    return DimensionSize<Dim>(0);
}

template<typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type 
MultiArray<T, FirstDimension>::size() const
{
    return _size;
}

template<typename T, typename FirstDimension>
template<typename Dim>
void MultiArray<T, FirstDimension>::resize(DimensionSize<Dim> size)
{
    this->do_resize(1,  size);
}

template<typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<T, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dim>)
{
    _data.resize(total * static_cast<std::size_t>(_size));
}

template<typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<T, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dim> size)
{
    _size = size;
    _data.resize(total * static_cast<std::size_t>(_size));
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::reference_type MultiArray<T, FirstDimension>::operator[](DimensionIndex<FirstDimension> index)
{
    return *(begin() + static_cast<std::size_t>(index));
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::iterator MultiArray<T, FirstDimension>::begin()
{
    return _data.begin();
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::const_iterator MultiArray<T, FirstDimension>::begin() const
{
    return _data.begin();
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::const_iterator MultiArray<T, FirstDimension>::cbegin() const
{
    return _data.cbegin();
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::iterator MultiArray<T, FirstDimension>::end()
{
    return _data.end();
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::const_iterator MultiArray<T, FirstDimension>::end() const
{
    return _data.end();
}

template<typename T, typename FirstDimension>
typename MultiArray<T, FirstDimension>::const_iterator MultiArray<T, FirstDimension>::cend() const
{
    return _data.cend();
}

} // namespace astrotypes
} // namespace pss
