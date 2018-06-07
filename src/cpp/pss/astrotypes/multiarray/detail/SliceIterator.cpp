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

template<typename DerivedType, typename SliceType, bool is_const, int rank>
SliceIteratorBase<DerivedType, SliceType, is_const, rank>::SliceIteratorBase(SliceT& slice)
    : BaseT(slice)
    , _slice(slice)
    , _offset(BaseT::_current)
{
}

template<typename DerivedType, typename SliceType, bool is_const, int rank>
SliceIteratorBase<DerivedType, SliceType, is_const, rank>::~SliceIteratorBase()
{
}

template<typename DerivedType, typename SliceType, bool is_const, int rank>
DerivedType& SliceIteratorBase<DerivedType, SliceType, is_const, rank>::operator++()
{
    _slice.increment_it(this->_current, this->_end, _offset);
    return static_cast<DerivedType&>(*this);
}

template<typename DerivedType, typename SliceType, bool is_const, int rank>
typename SliceIteratorBase<DerivedType, SliceType, is_const, rank>::difference_type SliceIteratorBase<DerivedType, SliceType, is_const, rank>::operator-(SelfType const& f) const
{
    return  _slice.diff_it(this->_current - f._current);
}

template<typename DerivedType, typename SliceType, bool is_const>
DerivedType SliceIteratorBase<DerivedType, SliceType, is_const, 1>::create_end(SliceT& slice)
{
    DerivedType it(slice);
    it._current += static_cast<std::size_t>(slice.base_span());
    it._end = it._current;
    return it;
}

template<typename DerivedType, typename SliceType, bool is_const>
SliceIteratorBase<DerivedType, SliceType, is_const, 1>::SliceIteratorBase(SliceT& slice)
    : _current(slice.base_ptr())
    , _end(_current + slice.contiguous_span())
{
}

template<typename DerivedType, typename SliceType, bool is_const>
SliceIteratorBase<DerivedType, SliceType, is_const, 1>::~SliceIteratorBase()
{
}

template<typename DerivedType, typename SliceType, bool is_const>
typename SliceIteratorBase<DerivedType, SliceType, is_const, 1>::difference_type SliceIteratorBase<DerivedType, SliceType, is_const, 1>::operator-(SelfType const& f) const
{
    return _current - f._current;
}

template<typename DerivedType, typename SliceType, bool is_const>
DerivedType& SliceIteratorBase<DerivedType, SliceType, is_const, 1>::operator++()
{
    ++_current;
    return static_cast<DerivedType&>(*this);
}

template<typename DerivedType, typename SliceType, bool is_const>
DerivedType& SliceIteratorBase<DerivedType, SliceType, is_const, 1>::operator++(int)
{
    DerivedType copy(static_cast<DerivedType&>(*this));
    ++(*this);
    return copy;
}

template<typename DerivedType, typename SliceType, bool is_const>
const typename SliceIteratorBase<DerivedType, SliceType, is_const, 1>::reference SliceIteratorBase<DerivedType, SliceType, is_const, 1>::operator*() const
{
    return *_current;
}

template<typename DerivedType, typename SliceType, bool is_const>
template<typename D, bool const_val, int rank>
bool SliceIteratorBase<DerivedType, SliceType, is_const, 1>::operator==(SliceIteratorBase<D, SliceType, const_val, rank> const& o) const
{
    return _current == o._current;
}

template<typename DerivedType, typename SliceType, bool is_const>
template<typename D, bool const_val, int rank>
bool SliceIteratorBase<DerivedType, SliceType, is_const, 1>::operator!=(SliceIteratorBase<D, SliceType, const_val, rank> const& o) const
{
    return _current != o._current;
}

template<typename SliceType, bool is_const>
SliceIterator<SliceType, is_const>::SliceIterator(SliceT& slice)
    : BaseT(slice)
{
}

} // namespace astrotypes
} // namespace pss
