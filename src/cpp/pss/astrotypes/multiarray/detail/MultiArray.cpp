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
template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
MultiArray<Alloc, T, FirstDimension, Dimensions...>::MultiArray(DimensionSize<FirstDimension> const& fd, DimensionSize<Dimensions> const& ... sizes)
    : BaseT(false, sizes...)
    , _size(fd)
{
    resize(fd);
}

// private interface for constructing in an inheritance stack
template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
MultiArray<Alloc, T, FirstDimension, Dimensions...>::MultiArray(bool, DimensionSize<FirstDimension> const& fd, DimensionSize<Dimensions> const& ... sizes)
    : BaseT(false, sizes...)
    , _size(fd)
{
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::iterator MultiArray<Alloc, T, FirstDimension, Dimensions...>::begin()
{
    return BaseT::begin();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, FirstDimension, Dimensions...>::begin() const
{
    return BaseT::begin();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, FirstDimension, Dimensions...>::cbegin() const
{
    return BaseT::cbegin();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::iterator MultiArray<Alloc, T, FirstDimension, Dimensions...>::end()
{
    return BaseT::end();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, FirstDimension, Dimensions...>::end() const
{
    return BaseT::end();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, FirstDimension, Dimensions...>::cend() const
{
    return BaseT::cend();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::ReducedDimensionSliceType MultiArray<Alloc, T, FirstDimension, Dimensions...>::operator[](DimensionIndex<FirstDimension> index)
{
    return SliceType(*this, DimensionSpan<FirstDimension>(index, DimensionSize<FirstDimension>(1))
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...)[0];
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::ConstReducedDimensionSliceType MultiArray<Alloc, T, FirstDimension, Dimensions...>::operator[](DimensionIndex<FirstDimension> index) const
{
    return ConstSliceType(*this, DimensionSpan<FirstDimension>(index, DimensionSize<FirstDimension>(1))
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...)[0];
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::SliceType MultiArray<Alloc, T, FirstDimension, Dimensions...>::slice(DimensionSpan<FirstDimension> const& index_range)
{
    return SliceType(*this, index_range
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...);
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, FirstDimension, Dimensions...>::ConstSliceType MultiArray<Alloc, T, FirstDimension, Dimensions...>::slice(DimensionSpan<FirstDimension> const& index_range) const
{
    return ConstSliceType(*this, index_range
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...);
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
template<typename... Dims>
void MultiArray<Alloc, T, FirstDimension, Dimensions...>::resize(DimensionSize<Dims>... size)
{
    this->do_resize(1,  std::forward<DimensionSize<Dims>>(size)...);
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type 
MultiArray<Alloc, T, FirstDimension, Dimensions...>::size() const
{
    return BaseT::template size<Dim>();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type 
MultiArray<Alloc, T, FirstDimension, Dimensions...>::size() const
{
    return _size;
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<Alloc, T, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>&&... sizes)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size), size, std::forward<DimensionSize<Dims>>(sizes)...);
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<Alloc, T, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>&&... sizes)
{
    _size = size;
    BaseT::do_resize(total * static_cast<std::size_t>(_size), std::forward<DimensionSize<Dims>>(sizes)...);
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
void MultiArray<Alloc, T, FirstDimension, Dimensions...>::do_resize(std::size_t total)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size));
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
std::size_t MultiArray<Alloc, T, FirstDimension, Dimensions...>::data_size() const
{
    return BaseT::data_size();
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
bool MultiArray<Alloc, T, FirstDimension, Dimensions...>::equal_size(MultiArray const& o) const
{
    return _size == o.size<FirstDimension>() && BaseT::equal_size(static_cast<BaseT const&>(o));
}

template<typename Alloc, typename T, typename FirstDimension, typename... Dimensions>
bool MultiArray<Alloc, T, FirstDimension, Dimensions...>::operator==(MultiArray const& o) const
{
    return equal_size(o) && std::equal(o.cbegin(), o.cend(), cbegin());
}

/////////////////////////////////////////////////////////////
// Single Dimension specialisation 
/////////////////////////////////////////////////////////////
template<typename Alloc, typename T, typename FirstDimension>
MultiArray<Alloc, T, FirstDimension>::MultiArray(DimensionSize<FirstDimension> const& size)
    : _size(size)
{
    _data.resize(size);
}

template<typename Alloc, typename T, typename FirstDimension>
MultiArray<Alloc, T, FirstDimension>::MultiArray(bool, DimensionSize<FirstDimension> const& size)
    : _size(size)
{
}

template<typename Alloc, typename T, typename FirstDimension>
MultiArray<Alloc, T, FirstDimension>::~MultiArray()
{
}

template<typename Alloc, typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type 
MultiArray<Alloc, T, FirstDimension>::size() const
{
    return DimensionSize<Dim>(0);
}

template<typename Alloc, typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type 
MultiArray<Alloc, T, FirstDimension>::size() const
{
    return _size;
}

template<typename Alloc, typename T, typename FirstDimension>
std::size_t MultiArray<Alloc, T, FirstDimension>::data_size() const
{
    return this->_data.size();
}

template<typename Alloc, typename T, typename FirstDimension>
template<typename Dim>
void MultiArray<Alloc, T, FirstDimension>::resize(DimensionSize<Dim> size)
{
    this->do_resize(1,  size);
}

template<typename Alloc, typename T, typename FirstDimension>
//template<typename Dim>
//typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, void>::type 
void MultiArray<Alloc, T, FirstDimension>::do_resize(std::size_t total)
{
    _data.resize(total * static_cast<std::size_t>(_size));
}

template<typename Alloc, typename T, typename FirstDimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, void>::type 
MultiArray<Alloc, T, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dim> size)
{
    _size = size;
    _data.resize(total * static_cast<std::size_t>(_size));
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::reference_type MultiArray<Alloc, T, FirstDimension>::operator[](DimensionIndex<FirstDimension> index)
{
    return *(begin() + static_cast<std::size_t>(index));
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::iterator MultiArray<Alloc, T, FirstDimension>::begin()
{
    return _data.begin();
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::const_iterator MultiArray<Alloc, T, FirstDimension>::begin() const
{
    return _data.begin();
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::const_iterator MultiArray<Alloc, T, FirstDimension>::cbegin() const
{
    return _data.cbegin();
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::iterator MultiArray<Alloc, T, FirstDimension>::end()
{
    return _data.end();
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::const_iterator MultiArray<Alloc, T, FirstDimension>::end() const
{
    return _data.end();
}

template<typename Alloc, typename T, typename FirstDimension>
typename MultiArray<Alloc, T, FirstDimension>::const_iterator MultiArray<Alloc, T, FirstDimension>::cend() const
{
    return _data.cend();
}

template<typename Alloc, typename T, typename FirstDimension>
bool MultiArray<Alloc, T, FirstDimension>::operator==(MultiArray const& o) const
{
    return _size == o.size<FirstDimension>() 
           && std::equal(o.cbegin(), o.cend(), cbegin());
}

template<typename Alloc, typename T, typename FirstDimension>
bool MultiArray<Alloc, T, FirstDimension>::equal_size(MultiArray const& o) const
{
    return _size == o.size<FirstDimension>();
}

} // namespace astrotypes
} // namespace pss
