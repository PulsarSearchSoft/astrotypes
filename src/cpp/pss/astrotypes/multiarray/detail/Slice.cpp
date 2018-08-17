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

// type traits helper structs
template<bool is_const, typename ParentT, typename Dimension>
struct has_dimension<Slice<is_const, ParentT, Dimension>, Dimension> : public std::true_type
{};

template<bool is_const, typename ParentT, typename Dimension1, typename Dimension>
struct has_dimension<Slice<is_const, ParentT, Dimension1>, Dimension> : public has_dimension<ParentT, Dimension>
{};

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
struct has_dimension<Slice<is_const, ParentT, Dimension, Dimensions...>, Dimension> : public std::true_type
{
};

template<bool is_const, typename ParentT, typename Dimension1, typename Dimension, typename... Dimensions>
struct has_dimension<Slice<is_const, ParentT, Dimension1, Dimensions...>, Dimension>
    : public has_dimension<Slice<is_const, ParentT, Dimensions...>, Dimension>
{
};

template<bool is_const, typename ParentT, typename... Dimensions>
struct has_exact_dimensions<Slice<is_const, ParentT, Dimensions...>, Dimensions...> : public std::true_type //has_exact_dimensions<ParentT>
{
};

/*
template<bool is_const, typename ParentT, typename... SliceDimensions, typename... Dimensions>
struct has_exact_dimensions<Slice<is_const, ParentT, SliceDimensions...>, Dimensions...> : public has_exact_dimensions<ParentT, Dimensions...>
{
};
*/

// ------------------- multi dimension ----------------------- -------------
template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice(
        typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
     ,  DimensionSpan<Dims> const&... spans)
    : BaseT(internal_construct_tag(), parent, spans... )
    , _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...))
    , _base_span(0U) // not used (yet) so don't bother calculating it
    , _ptr(parent.begin() + static_cast<std::size_t>(_span.start()) * BaseT::_base_span)
{
    BaseT::offset(_ptr);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice(
        typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
     ,  DimensionSpan<Dims> const&... spans)
    : BaseT(internal_construct_tag(), parent, spans... )
    , _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(0), DimensionSize<Dimension>(parent.template size<Dimension>())))
    , _base_span(0U) // not used (yet) so don't bother calculating it
    , _ptr(parent.begin())
{
    BaseT::offset(_ptr);
}

// SLice changed dim constructor - case Dim not provided bu constructor args
template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice( copy_resize_construct_tag const&
                                                        , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims>&&... spans
                                                        )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), std::forward<DimensionSpan<Dims>>(spans)...)
    , _span(copy._span)
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
    , _ptr(copy._ptr)
{
    _ptr = BaseT::offset(_ptr);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice( copy_resize_construct_base_tag const&
                                                        , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims>&&... spans
                                                        )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), std::forward<DimensionSpan<Dims>>(spans)...)
    , _span(copy._span)
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
{
}

// SLice changed dim constructor - case Dim provided bu constructor args
template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice(copy_resize_construct_tag const&
                                                        , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims>&&... spans
                                              )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), std::forward<DimensionSpan<Dims>>(spans)...)
    , _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(copy._span.start()
                           + arg_helper<DimensionSpan<Dimension>, DimensionSpan<Dims>...>::arg(std::forward<DimensionSpan<Dims>>(spans)...).start())
                           , arg_helper<DimensionSpan<Dimension>, DimensionSpan<Dims>...>::arg(std::forward<DimensionSpan<Dims>>(spans)...).span()))
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
    , _ptr(copy._ptr + static_cast<std::size_t>(
                           arg_helper<DimensionSpan<Dimension>, DimensionSpan<Dims>...>::arg(std::forward<DimensionSpan<Dims>>(spans)...).start())
                     * BaseT::_base_span)
{
    BaseT::offset(_ptr);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice( copy_resize_construct_base_tag const&
                                                        , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims>&&... spans
                                                        )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), std::forward<DimensionSpan<Dims>>(spans)...)
    , _span(arg_helper<DimensionSpan<Dimension>, DimensionSpan<Dims>...>::arg(std::forward<DimensionSpan<Dims>>(spans)...))
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
{
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice( internal_construct_tag const&
       , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ... spans)
    : BaseT(internal_construct_tag(), parent, spans...)
    , _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...))
    , _base_span(parent.template size<Dimension>() * BaseT::_base_span)
{
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<is_const, Parent, Dimension, Dimensions...>::Slice( internal_construct_tag const&
       , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ... spans)
    : BaseT(internal_construct_tag(), parent, spans...)
    , _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(0), DimensionSize<Dimension>(parent->template size<Dimension>())))
    , _base_span(parent.template size<Dimension>() * BaseT::_base_span)
{
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::parent_iterator const& Slice<is_const, Parent, Dimension, Dimensions...>::offset(parent_iterator const& it)
{
    _ptr=it + static_cast<std::size_t>(_span.start()) * BaseT::_base_span;
    return BaseT::offset(_ptr);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<is_const, Parent, Dimension, Dimensions...>::base_span() const
{
    return static_cast<std::size_t>(BaseT::_base_span) * _span.span();
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<is_const, Parent, Dimension, Dimensions...>::diff_base_span() const
{
    return static_cast<std::size_t>(BaseT::_base_span) * (_span.span() - 1);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<is_const, Parent, Dimension, Dimensions...>::contiguous_span() const
{
    return BaseT::contiguous_span();
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::parent_iterator& Slice<is_const, Parent, Dimension, Dimensions...>::base_ptr()
{
    return BaseT::base_ptr();
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::parent_iterator const& Slice<is_const, Parent, Dimension, Dimensions...>::base_ptr() const
{
    return BaseT::base_ptr();
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
std::size_t Slice<is_const, Parent, Dimension, Dimensions...>::data_size() const
{
    return _span.span() * BaseT::data_size();;
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
Slice<is_const, Parent, Dimension, Dimensions...>::size() const
{
    return _span.span();
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type
Slice<is_const, Parent, Dimension, Dimensions...>::size() const
{
    return BaseT::template size<Dim>();
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
Slice<is_const, Parent, Dimensions...>
Slice<is_const, Parent, Dimension, Dimensions...>::operator[](DimensionIndex<Dimension> offset) const
{
    return BaseT(*this) += static_cast<std::size_t>(offset);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<arg_helper<Dim, Dimensions...>::value
&& !std::is_same<Dim, Dimension>::value, typename Slice<is_const, Parent, Dimension, Dimensions...>::SliceType>::type
Slice<is_const, Parent, Dimension, Dimensions...>::operator[](DimensionIndex<Dim> const& index)
{
    return SliceType(copy_resize_construct_tag(), *this, DimensionSpan<Dim>(index, DimensionSize<Dim>(1)));
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<arg_helper<Dim, Dimensions...>::value
&& !std::is_same<Dim, Dimension>::value, typename Slice<is_const, Parent, Dimension, Dimensions...>::ConstSliceType>::type
Slice<is_const, Parent, Dimension, Dimensions...>::operator[](DimensionIndex<Dim> const& index) const
{
    return ConstSliceType(copy_resize_construct_tag(), reinterpret_cast<ConstSliceType const&>(*this), DimensionSpan<Dim>(index, DimensionSize<Dim>(1)));
}

// n.b offset refers to the dimension in the level above (i.e a full _base_span)
template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
Slice<is_const, Parent, Dimension, Dimensions...>& Slice<is_const, Parent, Dimension, Dimensions...>::operator+=(std::size_t offset)
{
    _ptr += (offset * _base_span);
    BaseT::offset(_ptr);
    return *this;
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
Slice<is_const, Parent, Dimension, Dimensions...>& Slice<is_const, Parent, Dimension, Dimensions...>::operator+=(DimensionSize<Dimension> offset)
{
    _ptr += (offset * BaseT::_base_span);
    BaseT::offset(_ptr);
    return *this;
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<arg_helper<Dimension, Dims...>::value, typename Slice<is_const, Parent, Dimension, Dimensions...>::SliceType>::type
Slice<is_const, Parent, Dimension, Dimensions...>::slice(DimensionSpan<Dims>&&... spans)
{
    return SliceType(copy_resize_construct_tag(), *this, std::forward<DimensionSpan<Dims>>(spans)...);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<!arg_helper<Dimension, Dims...>::value, typename Slice<is_const, Parent, Dimension, Dimensions...>::SliceType>::type
Slice<is_const, Parent, Dimension, Dimensions...>::slice(DimensionSpan<Dims>&&... spans)
{
    return SliceType(copy_resize_construct_tag(), *this, std::forward<DimensionSpan<Dims>>(spans)...);
}


template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice<true, Parent, Dimension, Dimensions...>>::type
Slice<is_const, Parent, Dimension, Dimensions...>::slice(DimensionSpan<Dims>&&... spans) const
{
    return ConstSliceType(copy_resize_construct_tag(), reinterpret_cast<ConstSliceType const&>(*this), std::forward<DimensionSpan<Dims>>(spans)...);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice<true, Parent, Dimension, Dimensions...>>::type
Slice<is_const, Parent, Dimension, Dimensions...>::slice(DimensionSpan<Dims>&&... spans) const
{
    return ConstSliceType(copy_resize_construct_tag(), reinterpret_cast<ConstSliceType const&>(*this), std::forward<DimensionSpan<Dims>>(spans)...);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::iterator Slice<is_const, Parent, Dimension, Dimensions...>::begin()
{
    return SliceIterator<SelfType, is_const>(*this);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::const_iterator Slice<is_const, Parent, Dimension, Dimensions...>::begin() const
{
    return SliceIterator<SelfType, true>(*this);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::const_iterator Slice<is_const, Parent, Dimension, Dimensions...>::cbegin() const
{
    return SliceIterator<SelfType, true>(*this);
}

template<bool is_const, typename Parent, typename Dimension, typename... Dimensions>
typename Slice<is_const, Parent, Dimension, Dimensions...>::iterator Slice<is_const, Parent, Dimension, Dimensions...>::end()
{
    return SliceIterator<SelfType, is_const>::create_end(*this);
}

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
typename Slice<is_const, ParentT, Dimension, Dimensions...>::const_iterator Slice<is_const, ParentT, Dimension, Dimensions...>::end() const
{
    return SliceIterator<SelfType, true>::create_end(*this);
}

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
typename Slice<is_const, ParentT, Dimension, Dimensions...>::const_iterator Slice<is_const, ParentT, Dimension, Dimensions...>::cend() const
{
    return SliceIterator<SelfType, true>::create_end(*this);
}

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
template<typename IteratorT>
bool Slice<is_const, ParentT, Dimension, Dimensions...>::increment_it(IteratorT& current, SlicePosition<rank>& pos) const
{
    if(!BaseT::increment_it(current, pos)) {
        current -= BaseT::diff_base_span(); // return pointer to beginning of BaseT block
        if(++pos.index < _span.span())
        {
            current += BaseT::_base_span; // move up to next block
            return true;
        }
        // reset end to the end of a first chunk
        pos.index=0;
        return false;
    }
    return true;
}

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
template<typename IteratorDifferenceT>
IteratorDifferenceT Slice<is_const, ParentT, Dimension, Dimensions...>::diff_it(IteratorDifferenceT const& diff) const
{
    if(diff < (IteratorDifferenceT)BaseT::_base_span) {
        return BaseT::diff_it(diff);
    }
    else {
        return IteratorDifferenceT(diff/BaseT::_base_span) * BaseT::data_size() + BaseT::diff_it(diff%BaseT::_base_span);
    }
}

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
template<bool const_type>
bool Slice<is_const, ParentT, Dimension, Dimensions...>::operator==(Slice<const_type, ParentT, Dimension, Dimensions...> const& s) const
{
    return s.data_size() == data_size()
           && std::equal(s.cbegin(), s.cend(), cbegin());
}

// -------------------- single dimension specialisation -------------------
template<bool is_const, typename Parent, typename Dimension>
Slice<is_const, Parent, Dimension>::Slice(Parent& parent, DimensionSpan<Dimension> const& d)
    : _span(d)
    , _base_span(parent.template size<Dimension>())
    , _ptr(parent.begin() + static_cast<std::size_t>(_span.start()))
{
}


template<bool is_const, typename Parent, typename Dimension>
template<typename... Dims>
Slice<is_const, Parent, Dimension>::Slice( internal_construct_tag const&
       , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ... spans)
    : _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...))
    , _base_span(parent.template size<Dimension>())
{
}


template<bool is_const, typename Parent, typename Dimension>
template<typename... Dims>
Slice<is_const, Parent, Dimension>::Slice( internal_construct_tag const&
       , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ...)
    : _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(0), DimensionSize<Dimension>(parent.template size<Dimension>())))
    , _base_span(parent.template size<Dimension>())
{
}

template<bool is_const, typename Parent, typename Dimension>
template<typename... Dims>
Slice<is_const, Parent, Dimension>::Slice( typename std::enable_if<!arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
     , Slice const& copy
     , DimensionSpan<Dims>&&...)
    : _span(copy._span)
    , _base_span(copy._base_span)
{
}

template<bool is_const, typename Parent, typename Dimension>
template<typename... Dims>
Slice<is_const, Parent, Dimension>::Slice( typename std::enable_if<arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
     , Slice const& copy
     , DimensionSpan<Dims>&&... spans)
    : _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(copy._span.start()
                           + arg_helper<DimensionSpan<Dimension>, DimensionSpan<Dims>...>::arg(std::forward<DimensionSpan<Dims>>(spans)...).start())
                           , arg_helper<DimensionSpan<Dimension>, DimensionSpan<Dims>...>::arg(std::forward<DimensionSpan<Dims>>(spans)...).span()))
    , _base_span(copy._base_span)
{
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_iterator const& Slice<is_const, Parent, Dimension>::offset(parent_iterator const& it)
{
    _ptr = it + static_cast<std::size_t>(_span.start());
    return _ptr;
}

template<bool is_const, typename Parent, typename Dimension>
Slice<is_const, Parent, Dimension>::~Slice()
{
}

template<bool is_const, typename Parent, typename Dimension>
std::size_t Slice<is_const, Parent, Dimension>::data_size() const
{
    return _span.span();
}

template<bool is_const, typename Parent, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type Slice<is_const, Parent, Dimension>::size() const
{
    return _span.span();
}

template<bool is_const, typename Parent, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<((!std::is_same<Dim, Dimension>::value) && (!has_dimension<Parent, Dim>::value))
                       , DimensionSize<Dim>>::type Slice<is_const, Parent, Dimension>::size()
{
    return DimensionSize<Dim>(0);
}

template<bool is_const, typename Parent, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<(!std::is_same<Dim, Dimension>::value)
                        && has_dimension<Parent, Dim>::value
                       , DimensionSize<Dim>>::type Slice<is_const, Parent, Dimension>::size()
{
    return DimensionSize<Dim>(1);
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::reference_type Slice<is_const, Parent, Dimension>::operator[](std::size_t p) const
{
    parent_iterator ptr = (_ptr + p);
    return *ptr;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::reference_type Slice<is_const, Parent, Dimension>::operator[](DimensionIndex<Dimension> const& p) const
{
    return *(_ptr + static_cast<std::size_t>(p));
}

template<bool is_const, typename Parent, typename Dimension>
Slice<is_const, Parent, Dimension> Slice<is_const, Parent, Dimension>::slice(DimensionSpan<Dimension> const& span)
{
    Slice r(*this);
    _ptr += static_cast<std::size_t>(_span.start());
    r._span.start(_span.start() + DimensionSize<Dimension>(span.start()));
    r._span.span(span.span());
    return r;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::ConstSliceType Slice<is_const, Parent, Dimension>::slice(DimensionSpan<Dimension> const& span) const
{
    ConstSliceType r(*this);
    _ptr += static_cast<std::size_t>(_span.start());
    r._span.start(_span.start() + DimensionSize<Dimension>(span.start()));
    r._span.span(span.span());
    return r;
}

template<bool is_const, typename Parent, typename Dimension>
std::size_t Slice<is_const, Parent, Dimension>::base_span() const
{
    return static_cast<std::size_t>(_span.span());
}

template<bool is_const, typename Parent, typename Dimension>
std::size_t Slice<is_const, Parent, Dimension>::diff_base_span() const
{
    return static_cast<std::size_t>(_span.span());
}

template<bool is_const, typename Parent, typename Dimension>
std::size_t Slice<is_const, Parent, Dimension>::contiguous_span() const
{
    return static_cast<std::size_t>(_span.span());
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_iterator const& Slice<is_const, Parent, Dimension>::base_ptr() const
{
    return _ptr;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_iterator& Slice<is_const, Parent, Dimension>::base_ptr()
{
    return _ptr;
}

template<bool is_const, typename Parent, typename Dimension>
Slice<is_const, Parent, Dimension>& Slice<is_const, Parent, Dimension>::operator+=(DimensionSize<Dimension> const& offset)
{
    _ptr += static_cast<std::size_t>(offset) * _base_span;
    return *this;
}

template<bool is_const, typename Parent, typename Dimension>
Slice<is_const, Parent, Dimension>& Slice<is_const, Parent, Dimension>::operator+=(std::size_t offset)
{
    _ptr += offset * _base_span;
    return *this;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_iterator Slice<is_const, Parent, Dimension>::begin()
{
    return _ptr;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_const_iterator Slice<is_const, Parent, Dimension>::begin() const
{
    return _ptr;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_const_iterator Slice<is_const, Parent, Dimension>::cbegin() const
{
    return _ptr;
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_iterator Slice<is_const, Parent, Dimension>::end()
{
    return _ptr + static_cast<std::size_t>(_span.span());
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_const_iterator Slice<is_const, Parent, Dimension>::end() const
{
    return _ptr + static_cast<std::size_t>(_span.span());
}

template<bool is_const, typename Parent, typename Dimension>
typename Slice<is_const, Parent, Dimension>::parent_const_iterator Slice<is_const, Parent, Dimension>::cend() const
{
    return _ptr + static_cast<std::size_t>(_span.span());
}

template<bool is_const, typename Parent, typename Dimension>
template<typename IteratorT>
bool Slice<is_const, Parent, Dimension>::increment_it(IteratorT& current, SlicePosition<rank>& pos) const
{
    ++current;
    if(++pos.index < _span.span())
    {
        return true;
    }
    pos.index=0;
    return false;
}

template<bool is_const, typename ParentT, typename Dimension>
template<typename IteratorDifferenceT>
IteratorDifferenceT Slice<is_const, ParentT, Dimension>::diff_it(IteratorDifferenceT const& diff)
{
    return diff;
}

template<bool is_const, typename ParentT, typename Dimension>
template<bool is_const_>
bool Slice<is_const, ParentT, Dimension>::operator==(Slice<is_const_, ParentT, Dimension> const& s) const
{
    return s.data_size() == data_size()
        && std::equal(s.begin(), s.end(), begin());
}

} // namespace astrotypes
} // namespace pss
