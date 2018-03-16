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

template<typename T, typename FirstDimension, typename... Dimensions>
typename MultiArray<T, FirstDimension, Dimensions...>::SliceType MultiArray<T, FirstDimension, Dimensions...>::operator[](DimensionIndex<FirstDimension> index)
{
    return SliceType(*this, std::make_pair(index, DimensionSize<FirstDimension>(1))
                          , std::make_pair(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template size<Dimensions>()))...);
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type 
MultiArray<T, FirstDimension, Dimensions...>::size() const
{
    return BaseT::size();
}

template<typename T, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type 
MultiArray<T, FirstDimension, Dimensions...>::size() const
{
    return _size;
}

/////////////////////////////////////////////////////////////
// Single Dimension specialisation 
/////////////////////////////////////////////////////////////
template<typename T, typename FirstDimension>
MultiArray<T, FirstDimension>::MultiArray(DimensionSize<FirstDimension> const&)
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
typename MultiArray<T, FirstDimension>::SliceType MultiArray<T, FirstDimension>::operator[](DimensionIndex<FirstDimension> index)
{
    return SliceType(*this, std::make_pair(index, DimensionSize<FirstDimension>(1)));
}

} // namespace astrotypes
} // namespace pss
