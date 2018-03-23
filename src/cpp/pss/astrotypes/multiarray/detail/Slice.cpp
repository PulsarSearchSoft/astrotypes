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
    , _base_span(0)
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
void Slice<Parent, Dimension, Dimensions...>::offset(iterator const& it) 
{
    _ptr=it + static_cast<const std::size_t>(_span.start()) * BaseT::_base_span;
    BaseT::offset(_ptr);
}

template<typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<Parent, Dimension, Dimensions...>::span() const
{
    return static_cast<const std::size_t>(_span.span()) * BaseT::span();
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
Slice<Parent, Dimension, Dimensions...>::operator[](std::size_t offset) const
{
    return BaseT(*this) += offset;
}

template<typename Parent, typename Dimension, typename... Dimensions>
Slice<Parent, Dimension, Dimensions...>& Slice<Parent, Dimension, Dimensions...>::operator+=(std::size_t offset)
{
    _ptr += (offset * _base_span);
    BaseT::offset(_ptr);
    return *this;
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
void Slice<Parent, Dimension>::offset(iterator const& it)
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
std::size_t Slice<Parent, Dimension>::span() const
{
    return static_cast<const std::size_t>(_span.span());
}

template<typename Parent, typename Dimension>
DimensionSize<Dimension> Slice<Parent, Dimension>::parent_span() const
{
    return _base_span;
}

template<typename Parent, typename Dimension>
Slice<Parent, Dimension>& Slice<Parent, Dimension>::operator+=(DimensionSize<Dimension> const& offset)
{
    _ptr += static_cast<const std::size_t&>(offset) * _base_span;
    return *this;
}

} // namespace astrotypes
} // namespace pss
