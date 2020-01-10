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
#include "pss/astrotypes/multiarray/ExtendedMultiArray.h"


namespace pss {
namespace astrotypes {
namespace multiarray {


template<typename DerivedType, typename MultiArrayType, typename Enable>
ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::ExtendedMultiArray(MultiArrayType const& ma)
    : _ma(ma)
{
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::~ExtendedMultiArray()
{
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::Type& ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::get()
{
    return IsWrapped::extract(_ma);
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::Type const& ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::get() const
{
    return IsWrapped::extract(_ma);
}

template<typename DerivedType, typename MultiArrayType>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type>::iterator
ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type >::begin()
{
    return this->get().begin();
}

template<typename DerivedType, typename MultiArrayType>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type>::iterator
ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type >::end()
{
    return this->get().end();
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::const_iterator ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::begin() const
{
    return get().begin();
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::const_iterator ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::end() const
{
    return get().end();
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::const_iterator ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::cbegin() const
{
    return get().cbegin();
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::const_iterator ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::cend() const
{
    return get().cend();
}

template<typename DerivedType, typename MultiArrayType>
template<typename Dimension>
inline typename ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type>::ReducedDimensionSliceType
ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type>::operator[](DimensionIndex<Dimension> index)
{
    return this->get()[index];
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
template<typename Dimension>
typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::ConstReducedDimensionSliceType ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::operator[](DimensionIndex<Dimension> index) const
{
    return get()[index];
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
template<typename Dim>
inline DimensionSize<Dim> ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::dimension() const
{
    return get().template dimension<Dim>();
}

template<typename DerivedType, typename MultiArrayType>
template<typename... Dims>
typename ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type>::SliceType ExtendedMultiArray<DerivedType, MultiArrayType, typename std::enable_if<ExtendedMultiArray<DerivedType, MultiArrayType, void*>::is_const>::type>::slice(DimensionSpan<Dims> const&... ranges)
{
    return this->get().slice(ranges...);
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
template<typename... Dims>
typename ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::ConstSliceType ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::slice(DimensionSpan<Dims> const&... ranges) const
{
    return get().slice(ranges...);
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
std::size_t ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::data_size() const
{
    return get().data_size();
}

template<typename DerivedType, typename MultiArrayType, typename Enable>
bool ExtendedMultiArray<DerivedType, MultiArrayType, Enable>::operator==(Type const& other) const
{
    return get() == other;
}

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
