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

template<typename DerivedType, typename SliceType, bool is_const>
SliceIteratorBase<DerivedType, SliceType, is_const>::SliceIteratorBase(SliceType& slice)
    : _slice(slice)
    , _current(slice.base_ptr())
    , _end(_current + slice.contiguous_span())
    , _offset(_current)
{
}

template<typename DerivedType, typename SliceType, bool is_const>
SliceIteratorBase<DerivedType, SliceType, is_const>::~SliceIteratorBase()
{
}

template<typename DerivedType, typename SliceType, bool is_const>
DerivedType SliceIteratorBase<DerivedType, SliceType, is_const>::create_end(SliceType& slice)
{
    DerivedType it(slice);
    it._current += static_cast<std::size_t>(slice.base_span() * slice._span.span());
    it._end = it._current;
    return it;
}

template<typename DerivedType, typename SliceType, bool is_const>
const typename SliceIteratorBase<DerivedType, SliceType, is_const>::reference SliceIteratorBase<DerivedType, SliceType, is_const>::operator*() const
{
    return *_current;
}

template<typename DerivedType, typename SliceType, bool is_const>
template<typename D>
bool SliceIteratorBase<DerivedType, SliceType, is_const>::operator==(SliceIteratorBase<D, SliceType, is_const> const& o) const
{
    return _current == o._current;
}

template<typename DerivedType, typename SliceType, bool is_const>
template<typename D>
bool SliceIteratorBase<DerivedType, SliceType, is_const>::operator!=(SliceIteratorBase<D, SliceType, is_const> const& o) const
{
    return _current != o._current;
}

template<typename DerivedType, typename SliceType, bool is_const>
DerivedType& SliceIteratorBase<DerivedType, SliceType, is_const>::operator++()
{
    _slice.increment_it(_current, _end, _offset);
    return static_cast<DerivedType&>(*this);
}

template<typename DerivedType, typename SliceType, bool is_const>
DerivedType& SliceIteratorBase<DerivedType, SliceType, is_const>::operator++(int)
{
    DerivedType copy(static_cast<DerivedType&>(*this));
    ++(*this);
    return copy;
}

template<typename SliceType, bool is_const>
SliceIterator<SliceType, is_const>::SliceIterator(SliceType& slice)
    : BaseT(slice)
{
}

/*
template<typename SliceType, bool is_const>
typename SliceIterator<SliceType, is_const>::reference SliceIterator<SliceType, is_const>::operator*()
{
    return *this->_current;  
}
*/

} // namespace astrotypes
} // namespace pss
