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
#include "pss/astrotypes/multiarray/Slice.h"
#include <iostream>


namespace pss {
namespace astrotypes {

// ------------------- multi dimension ----------------------- -------------
template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimension, Dimensions...>::Slice(Parent& parent
                                              , DimensionSpan<Dimension> const& d
                                              , DimensionSpan<Dimensions> const&... spans
                                              )
    : BaseT(spans..., parent)
    , _span(d)
    , _base_span(0U) // not used (yet) so don't bother calculating it
    , _ptr(parent.begin() + static_cast<const std::size_t>(_span.start()) * BaseT::_base_span)
{
    BaseT::offset(_ptr);
}

template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimension, Dimensions...>::Slice( DimensionSpan<Dimension> const& d
                                              , DimensionSpan<Dimensions> const&... spans
                                              , Parent const& parent
                                              )
    : BaseT(spans..., parent)
    , _span(d)
    , _base_span(parent.template size<Dimension>() * BaseT::_base_span)
{
}

template<typename Parent, typename Dimension, typename... Dimensions>
void Slice<Parent, Dimension, Dimensions...>::offset(parent_iterator const& it) 
{
    _ptr=it + static_cast<const std::size_t>(_span.start()) * BaseT::_base_span;
    BaseT::offset(_ptr);
}

template<typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<Parent, Dimension, Dimensions...>::base_span() const
{
    return static_cast<const std::size_t>(BaseT::_base_span);
}

template<typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<Parent, Dimension, Dimensions...>::contiguous_span() const
{
    return BaseT::contiguous_span();
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::parent_iterator& Slice<Parent, Dimension, Dimensions...>::base_ptr()
{
    return BaseT::base_ptr();
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::parent_iterator const& Slice<Parent, Dimension, Dimensions...>::base_ptr() const
{
    return BaseT::base_ptr();
}

template<typename Parent, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type 
Slice<Parent, Dimension, Dimensions...>::size() const
{
    return _span.span();
}

template<typename Parent, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type 
Slice<Parent, Dimension, Dimensions...>::size() const
{
    return BaseT::template size<Dim>();
}

template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimensions...>
Slice<Parent, Dimension, Dimensions...>::operator[](DimensionIndex<Dimension> offset) const
{
    return BaseT(*this) += static_cast<std::size_t>(offset);
}

template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimensions...>
Slice<Parent, Dimension, Dimensions...>::operator[](std::size_t offset) const
{
    return BaseT(*this) += offset;
}


// n.b offset refers to the dimension in the level above (i.e a full _base_span)
template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimension, Dimensions...>& Slice<Parent, Dimension, Dimensions...>::operator+=(std::size_t offset)
{
    _ptr += (offset * _base_span);
    BaseT::offset(_ptr);
    return *this;
}

template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimension, Dimensions...>& Slice<Parent, Dimension, Dimensions...>::operator+=(DimensionSize<Dimension> offset)
{
    _ptr += (offset * BaseT::_base_span);
    BaseT::offset(_ptr);
    return *this;
}

template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimension, Dimensions...>
Slice<Parent, Dimension, Dimensions...>::slice(DimensionSpan<Dimension> const& span) const
{
    assert(span.span() <= _span.span());
    SelfType s(*this);
    s += DimensionSize<Dimension>(span.start());
    s._span.start(_span.start() + DimensionSize<Dimension>(span.start()));
    s._span.span(span.span());
    return s;
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::iterator Slice<Parent, Dimension, Dimensions...>::begin()
{
    return make_slice_iterator(*this);
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::const_iterator Slice<Parent, Dimension, Dimensions...>::begin() const
{
    return make_slice_iterator(*this);
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::const_iterator Slice<Parent, Dimension, Dimensions...>::cbegin() const
{
    return make_slice_iterator(*this);
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::iterator Slice<Parent, Dimension, Dimensions...>::end()
{
    return make_end_slice_iterator(*this);
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::const_iterator Slice<Parent, Dimension, Dimensions...>::end() const
{
    return make_end_slice_iterator(*this);
}

template<typename Parent, typename Dimension, typename... Dimensions>
typename Slice<Parent, Dimension, Dimensions...>::const_iterator Slice<Parent, Dimension, Dimensions...>::cend() const
{
    return make_end_slice_iterator(*this);
}

template<typename Parent, typename Dimension, typename... Dimensions>
template<typename IteratorT>
bool Slice<Parent, Dimension, Dimensions...>::increment_it(IteratorT& current, IteratorT& end, IteratorT& offset)
{
    if(!BaseT::increment_it(current, end, offset)) {
        if(current < offset + (_span.span()  - 1 ) * BaseT::_base_span)
        {
            end += BaseT::_base_span;
            current = end - contiguous_span();
            return true;
        }
        // reset end to the end of a first chunk
        offset += _base_span;
        end -= (_span.span() -1) * BaseT::_base_span;
        return false;
    }
    return true;
}
// -------------------- single dimension specialisation -------------------
template<typename Parent, typename Dimension>
Slice<Parent, Dimension>::Slice(Parent& parent, DimensionSpan<Dimension> const& d)
    : _span(d)
    , _base_span(parent.template size<Dimension>())
    , _ptr(parent.begin() + static_cast<const std::size_t>(_span.start()))
{
}

template<typename Parent, typename Dimension>
Slice<Parent, Dimension>::Slice(DimensionSpan<Dimension> const& d, Parent const& p)
    : _span(d)
    , _base_span(p.template size<Dimension>())
{
}

template<typename Parent, typename Dimension>
void Slice<Parent, Dimension>::offset(parent_iterator const& it)
{
    _ptr = it + static_cast<const std::size_t>(_span.start());
}

template<typename Parent, typename Dimension>
Slice<Parent, Dimension>::~Slice()
{
}

template<typename Parent, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type Slice<Parent, Dimension>::size() const
{
    return _span.span();
}

template<typename Parent, typename Dimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type Slice<Parent, Dimension>::size() const
{
    return DimensionSize<Dim>(0);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::reference_type Slice<Parent, Dimension>::operator[](std::size_t p) const
{
    return *(_ptr + p);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::reference_type Slice<Parent, Dimension>::operator[](DimensionIndex<Dimension> const& p) const
{
    return *(_ptr + static_cast<std::size_t const&>(p));
}

template<typename Parent, typename Dimension>
std::size_t Slice<Parent, Dimension>::base_span() const
{
    return static_cast<const std::size_t>(_span.span());
}

template<typename Parent, typename Dimension>
std::size_t Slice<Parent, Dimension>::contiguous_span() const
{
    return static_cast<const std::size_t>(_span.span());
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::parent_iterator const& Slice<Parent, Dimension>::base_ptr() const
{
    return _ptr;
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::parent_iterator& Slice<Parent, Dimension>::base_ptr()
{
    return _ptr;
}

template<typename Parent, typename Dimension>
Slice<Parent, Dimension>& Slice<Parent, Dimension>::operator+=(DimensionSize<Dimension> const& offset)
{
    _ptr += static_cast<const std::size_t&>(offset) * _base_span;
    return *this;
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::iterator Slice<Parent, Dimension>::begin()
{
    return make_slice_iterator(*this);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::const_iterator Slice<Parent, Dimension>::begin() const
{
    return make_slice_iterator(*this);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::const_iterator Slice<Parent, Dimension>::cbegin() const
{
    return make_slice_iterator(*this);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::iterator Slice<Parent, Dimension>::end()
{
    return make_end_slice_iterator(*this);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::const_iterator Slice<Parent, Dimension>::end() const
{
    return make_end_slice_iterator(*this);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::const_iterator Slice<Parent, Dimension>::cend() const
{
    return make_end_slice_iterator(*this);
}

template<typename Parent, typename Dimension>
template<typename IteratorT>
bool Slice<Parent, Dimension>::increment_it(IteratorT& current, IteratorT& end, IteratorT&)
{
    ++current;
    if(current < end)
    {
        return true;
    }
    return false;
}

} // namespace astrotypes
} // namespace pss
