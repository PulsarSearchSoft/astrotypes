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

template<typename ParentT, typename... Dimensions>
SliceIterator<Slice<ParentT, Dimensions...>> make_slice_iterator(Slice<ParentT, Dimensions...>& slice)
{
    return SliceIterator<Slice<ParentT, Dimensions...>>(slice);
}

template<typename ParentT, typename... Dimensions>
SliceIterator<Slice<ParentT, Dimensions...>> make_end_slice_iterator(Slice<ParentT, Dimensions...>& slice)
{
    return SliceIterator<Slice<ParentT, Dimensions...>>::create_end(slice);
}

template<typename DerivedType, typename SliceType>
SliceIteratorBase<DerivedType, SliceType>::SliceIteratorBase(SliceType& slice)
    : _slice(slice)
    , _current(slice.base_ptr())
    , _end(_current + slice.contiguous_span())
    , _offset(_current)
{
}

template<typename DerivedType, typename SliceType>
SliceIteratorBase<DerivedType, SliceType>::~SliceIteratorBase()
{
}

template<typename DerivedType, typename SliceType>
DerivedType SliceIteratorBase<DerivedType, SliceType>::create_end(SliceType& slice)
{
    DerivedType it(slice);
    it._current += static_cast<std::size_t>(slice.base_span() * slice._span.span());
    it._end = it._current;
    return it;
}

template<typename DerivedType, typename SliceType>
const typename SliceIteratorBase<DerivedType, SliceType>::reference SliceIteratorBase<DerivedType, SliceType>::operator*() const
{
    return *_current;
}

template<typename DerivedType, typename SliceType>
template<typename D>
bool SliceIteratorBase<DerivedType, SliceType>::operator==(SliceIteratorBase<D, SliceType> const& o) const
{
    return _current == o._current;
}

template<typename DerivedType, typename SliceType>
template<typename D>
bool SliceIteratorBase<DerivedType, SliceType>::operator!=(SliceIteratorBase<D, SliceType> const& o) const
{
    return _current != o._current;
}

template<typename DerivedType, typename SliceType>
DerivedType& SliceIteratorBase<DerivedType, SliceType>::operator++()
{
    _slice.increment_it(_current, _end, _offset);
    return static_cast<DerivedType&>(*this);
}

template<typename DerivedType, typename SliceType>
DerivedType& SliceIteratorBase<DerivedType, SliceType>::operator++(int)
{
    DerivedType copy(static_cast<DerivedType&>(*this));
    ++(*this);
    return copy;
}

template<typename SliceType>
SliceIterator<SliceType>::SliceIterator(SliceType& slice)
    : BaseT(slice)
{
}

template<typename SliceType>
typename SliceIterator<SliceType>::reference SliceIterator<SliceType>::operator*()
{
    return *this->_current;  
}

} // namespace astrotypes
} // namespace pss
