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
#include "ReducedRankSlice.h"
#include "pss/astrotypes/multiarray/Slice.h"
#include <iostream>

namespace pss {
namespace astrotypes {

// type traits helper structs
template<typename T>
struct is_slice : public std::is_base_of<multiarray::SliceTag, typename std::decay<T>::type> {};

// for any slice type we can ask the object directly or search its parent for implied Dimensions
template<typename T, typename Dimension>
struct enabler2<has_dimension, T, Dimension, typename std::enable_if<is_slice<T>::value>::type>
    : public std::conditional<has_dimension_strict<T,Dimension>::value | has_dimension<typename T::Parent, Dimension>::value
                             , std::true_type
                             , std::false_type>::type
{
};

// for any slice type we can ask the object directly
template<typename T, typename Dimension>
struct enabler2<has_dimension_strict, T, Dimension, typename std::enable_if<is_slice<T>::value>::type>
    : public std::conditional<T::template has_dimension<Dimension>(), std::true_type, std::false_type>::type
{
};

// has_exact_dimensions
template<typename T, typename DimensionTuple>
struct enabler2<has_exact_dimensions, T, DimensionTuple, typename std::enable_if<is_slice<T>::value>::type>
    : public std::is_same<DimensionTuple, typename T::DimensionTuple>
{
};

namespace multiarray {
// -- determine the rwturn types of the various dimension reducing operators
template<typename Dim, typename SliceType>
struct OperatorSliceTypeHelper;

template<typename Dim, typename SliceType>
struct ConstOperatorSliceTypeHelper;

// match SliceMixin
template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
struct OperatorSliceTypeHelper<Dim, SliceMixin<Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>> : public OperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>
{
    typedef typename OperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>::type type;
};

// match SliceMixin for Const types
template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
struct ConstOperatorSliceTypeHelper<Dim, SliceMixin<Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>> : public ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>
{
    typedef typename ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>::type type;
};

template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
struct OperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>> {
    //typedef typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::SliceType type;
    typedef typename std::conditional<IsConst,
                        typename ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>>::type
                      , SliceMixin<multiarray::ReducedRankSlice<Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>, Dim>>
                    >::type type;
};

// ordered in line with Parent
template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename... Dimensions>
struct OperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dim, Dimensions...>> {
    typedef typename std::conditional<IsConst,
                        typename ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dim, Dimensions...>>::type
                      , typename Slice<IsConst, SliceTraitsT, SliceMixin, Dim, Dimensions...>::ReducedSliceType
                    >::type type;
};

template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin>
struct OperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dim>> {
    typedef typename Slice<IsConst, SliceTraitsT, SliceMixin, Dim>::reference_type type;
};

template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
struct ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>> {
    //typedef typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::ConstSliceType type;
    typedef SliceMixin<multiarray::ReducedRankSlice<Slice<true, SliceTraitsT, SliceMixin, Dimension, Dimensions...>, Dim>> type;
};

template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename... Dimensions>
struct ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dim, Dimensions...>> {
    typedef typename Slice<IsConst, SliceTraitsT, SliceMixin, Dim, Dimensions...>::ConstReducedSliceType type;
};

template<typename Dim, bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin>
struct ConstOperatorSliceTypeHelper<Dim, Slice<IsConst, SliceTraitsT, SliceMixin, Dim>> {
    typedef typename Slice<true, SliceTraitsT, SliceMixin, Dim>::const_reference_type type;
};

template<typename TraitsT, typename NewParentT>
struct SliceTraitsParentChangeHelper
{
    typedef NewParentT type;
};

template<typename TraitsT, typename NewParentT, typename D>
struct SliceTraitsParentChangeHelper<InternalSliceTraits<TraitsT, D>, NewParentT>
{
    typedef InternalSliceTraits<NewParentT, D> type;
};

// This helper will transform an incoming slice to the equivalent slice in a different parent
template<bool IsConst, typename SliceTraitsT, typename TargetDimensionTuple, typename T>
struct SliceReturnTypeHelper;

template<bool IsConst, typename SliceTraitsT, typename T, typename TargetDimensionTuple, std::size_t Rank, typename ExcludedDim>
struct SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, ReducedRankSlice<T, ExcludedDim, Rank> >
    : public SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, T>
{
    typedef SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, T> BaseT;
    typedef typename BaseT::slice_type slice_type;
    typedef ReducedRankSlice<typename BaseT::type, ExcludedDim, Rank> type;
};


template<bool IsConst, typename SliceTraitsT, typename T, template<typename> class Mixin, typename TargetDimensionTuple>
struct SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, Mixin<T> >
    : public SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, T>
{
    typedef SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, T> BaseT;
    typedef typename BaseT::slice_type slice_type;
    typedef Mixin<typename BaseT::type> type;
};

template<bool IsConst, typename SliceTraitsT, bool IsConst2, template<typename> class SliceMixin2, typename SliceTraitsT2, typename TargetDimensionTuple,typename Dimension, typename... Dimensions>
struct SliceReturnTypeHelper<IsConst, SliceTraitsT, TargetDimensionTuple, Slice<IsConst2, SliceTraitsT2, SliceMixin2, Dimension, Dimensions...>>
{
    private:
        template<typename Tuple>
        struct UnpackTuple;

        template<typename... TupleArgs>
        struct UnpackTuple<std::tuple<TupleArgs...>>
        {
            typedef Slice<IsConst
                         , typename SliceTraitsParentChangeHelper<SliceTraitsT2, SliceTraitsT>::type
                         , SliceMixin2, TupleArgs...> type;
        };

    public:
        typedef typename minimal_dimension_list<std::tuple<Dimension, Dimensions...>
                                              , TargetDimensionTuple>::type MinimumOrderedTargetDimensionSet;
        typedef typename UnpackTuple<MinimumOrderedTargetDimensionSet>::type slice_type;
        typedef slice_type type;
};


// ------------------- multi dimension ----------------------- -------------
template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
struct Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::OperatorSliceType
{
    typedef typename OperatorSliceTypeHelper<Dim, Slice>::type type;
};

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
struct Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::ConstOperatorSliceType
{
    typedef typename ConstOperatorSliceTypeHelper<Dim, Slice>::type type;
};

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
struct Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::OperatorSliceType
{
    typedef typename OperatorSliceTypeHelper<Dim, Slice>::type type;
};

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
struct Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::ConstOperatorSliceType
{
    typedef typename ConstOperatorSliceTypeHelper<Dim, Slice>::type type;
};

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice()
    : _span(DimensionSize<Dimension>(0u))
    , _base_span(0U)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim, typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice(
          typename std::enable_if<arg_helper<Dimension, Dim, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dim> const& span
       , DimensionSpan<Dims> const&... spans)
    : BaseT(internal_construct_tag(), parent, span, spans... )
    , _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dim> const&, DimensionSpan<Dims> const&...>::arg(span, spans...))
    , _base_span(0U) // not used (yet) so don't bother calculating it
    , _ptr(parent.begin() + static_cast<std::size_t>(_span.start()) * BaseT::_base_span)
{
    BaseT::offset(_ptr);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename SliceT>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( typename std::enable_if<is_slice<SliceT>::value, Parent>::type& parent
                                                                          , SliceT const& slice)
    : BaseT(internal_construct_tag(), parent, static_cast<typename SliceT::SliceType const&>(slice))
    , _span(slice.template parent_span<Dimension>())
    , _base_span(0U)
    , _ptr(parent.begin() + static_cast<std::size_t>(_span.start() * static_cast<std::size_t>(BaseT::_base_span)))
{
    BaseT::offset(_ptr);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim, typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice(
        typename std::enable_if<!arg_helper<Dimension, Dim, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dim> const& span
       , DimensionSpan<Dims> const&... spans)
    : BaseT(internal_construct_tag(), parent, span, spans... )
    , _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(0), parent.template dimension<Dimension>()))
    , _base_span(0U) // not used (yet) so don't bother calculating it
    , _ptr(parent.begin())
{
    BaseT::offset(_ptr);
}

// SLice changed dim constructor - case Dim not provided bu constructor args
template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( copy_resize_construct_tag const&
                                                        , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims> const&... spans
                                                        )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), spans...)
    , _span(copy._span)
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
    , _ptr(copy._ptr)
{
    _ptr = BaseT::offset(_ptr);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( copy_resize_construct_base_tag const&
                                                        , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims> const&... spans
                                                        )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), spans...)
    , _span(copy._span)
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
{
}

// SLice changed dim constructor - case Dim provided bu constructor args
template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice(copy_resize_construct_tag const&
                                                        , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims> const&... spans
                                              )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), spans...)
    , _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(copy._span.start()
                           + arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...).start())
                           , arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...).span()))
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
    , _ptr(copy._ptr + static_cast<std::size_t>(
                           arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...).start())
                     * BaseT::_base_span)
{
    BaseT::offset(_ptr);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( copy_resize_construct_base_tag const&
                                                        , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
                                                        , DimensionSpan<Dims> const&... spans
                                                        )
    : BaseT(copy_resize_construct_base_tag(), static_cast<BaseT const&>(copy), spans...)
    , _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...))
    , _base_span(copy._base_span) // not used (yet) so don't bother calculating it
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( internal_construct_tag const&
       , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ... spans)
    : BaseT(internal_construct_tag(), parent, spans...)
    , _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...))
    , _base_span(parent.template size<Dimension>() * BaseT::_base_span)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( internal_construct_tag const&
       , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ... spans)
    : BaseT(internal_construct_tag(), parent, spans...)
    , _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(0), DimensionSize<Dimension>(parent.template size<Dimension>())))
    , _base_span(parent.template size<Dimension>() * BaseT::_base_span)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename SliceT>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Slice( internal_construct_tag const&
                                                    , typename std::enable_if<is_slice<SliceT>::value, Parent&>::type parent
                                                    , SliceT const& slice)
    : BaseT(internal_construct_tag(), parent, slice)
    , _span(slice.template parent_span<Dimension>())
    , _base_span(parent.template dimension<Dimension>() * BaseT::_base_span)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::parent_iterator const& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::offset(parent_iterator const& it)
{
    _ptr=it + static_cast<std::size_t>(_span.start()) * BaseT::_base_span;
    return BaseT::offset(_ptr);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
std::size_t Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::base_span() const
{
    return static_cast<std::size_t>(BaseT::_base_span * (_span.span() - 1)) + BaseT::base_span();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
std::size_t Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::diff_base_span() const
{
    return static_cast<std::size_t>(BaseT::_base_span) * (_span.span() - 1);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::parent_iterator& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::base_ptr()
{
    return BaseT::base_ptr();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::parent_iterator const& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::base_ptr() const
{
    return BaseT::base_ptr();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
std::size_t Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::data_size() const
{
    return BaseT::data_size() * _span.span();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::size() const
{
    return _span.span();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::size() const
{
    return BaseT::template size<Dim>();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::dimension() const
{
    return _span.span();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::dimension() const
{
    return BaseT::template dimension<Dim>();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dimension>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::span() const
{
    return _span;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSpan<Dim>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::span() const
{
    return BaseT::template span<Dim>();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dimension>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::parent_span() const
{
    return _span;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSpan<Dim>>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::parent_span() const
{
    return BaseT::template parent_span<Dim>();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::template OperatorSliceType<Dimension>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator[](DimensionIndex<Dimension> offset)
{
    typedef typename OperatorSliceType<Dimension>::type ReducedSliceType;
    return ReducedSliceType(static_cast<BaseT const&>(*this)) += static_cast<std::size_t>(offset);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::template ConstOperatorSliceType<Dimension>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator[](DimensionIndex<Dimension> offset) const
{
    typedef typename ConstOperatorSliceType<Dimension>::type ReturnSliceType;
    return ReturnSliceType(reinterpret_cast<ReturnSliceType const&>(*this)) += static_cast<std::size_t>(offset);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<arg_helper<Dim, Dimensions...>::value
&& !std::is_same<Dim, Dimension>::value, typename Slice<IsConst, SliceTraitsT
                , SliceMixin, Dimension, Dimensions...>::template OperatorSliceType<Dim>::type>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator[](DimensionIndex<Dim> const& index)
{
    return Slice(copy_resize_construct_tag(), *this, DimensionSpan<Dim>(index, DimensionSize<Dim>(1)));
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<arg_helper<Dim, Dimensions...>::value
                    && !std::is_same<Dim, Dimension>::value, typename Slice<IsConst, SliceTraitsT
                     , SliceMixin, Dimension, Dimensions...>::template ConstOperatorSliceType<Dim>::type>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator[](DimensionIndex<Dim> const& index) const
{
    typedef Slice<true, SliceTraitsT, SliceMixin, Dimension, Dimensions...> _SliceType;
    typedef typename ConstOperatorSliceType<Dim>::type ReturnSliceType;
    return ReturnSliceType(_SliceType(copy_resize_construct_tag(), reinterpret_cast<_SliceType const&>(*this), DimensionSpan<Dim>(index, DimensionSize<Dim>(1))));
}


// n.b offset refers to the dimension in the level above (i.e a full _base_span)
template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator+=(std::size_t offset)
{
    _ptr += (offset * _base_span);
    BaseT::offset(_ptr);
    return *this;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator+=(DimensionSize<Dimension> offset)
{
    _ptr += (offset * BaseT::_base_span);
    BaseT::offset(_ptr);
    return *this;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<arg_helper<Dimension, Dims...>::value, typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::SliceType>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::slice(DimensionSpan<Dims> const&... spans)
{
    return Slice(copy_resize_construct_tag(), *this, spans...);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<!arg_helper<Dimension, Dims...>::value, typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::SliceType>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::slice(DimensionSpan<Dims> const&... spans)
{
    return Slice(copy_resize_construct_tag(), *this, spans...);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<arg_helper<Dimension, Dims...>::value
                       , typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::ConstSliceType>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::slice(DimensionSpan<Dims> const&... spans) const
{
    return ConstSliceType(Slice<true, Parent, SliceMixin, Dimension, Dimensions...>(copy_resize_construct_tag(), reinterpret_cast<ConstSliceType const&>(*this), spans...));
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename... Dims>
typename std::enable_if<!arg_helper<Dimension, Dims...>::value
                       , typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::ConstSliceType>::type
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::slice(DimensionSpan<Dims> const&... spans) const
{
    return ConstSliceType(Slice<true, Parent, SliceMixin, Dimension, Dimensions...>(copy_resize_construct_tag(), reinterpret_cast<ConstSliceType const&>(*this), spans...));
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::begin()
{
    return iterator(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::begin() const
{
    return const_iterator(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::cbegin() const
{
    return const_iterator(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::end()
{
    return iterator::create_end(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::end() const
{
    return const_iterator::create_end(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::cend() const
{
    return const_iterator::create_end(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
inline typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::Parent& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::parent() const
{
    return BaseT::parent();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_begin()
{
    return impl_iterator(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_begin() const
{
    return impl_const_iterator(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_cbegin() const
{
    return impl_const_iterator(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_end()
{
    return impl_iterator::create_end(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_end() const
{
    return impl_const_iterator::create_end(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::impl_cend() const
{
    return impl_const_iterator::create_end(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename IteratorT>
bool Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::increment_it(IteratorT& current, SlicePosition<rank>& pos) const
{
    if(!BaseT::increment_it(current, pos)) {
        current -= BaseT::diff_base_span(); // return pointer to beginning of BaseT block
        if(++pos.index < _span.span())
        {
            current += static_cast<std::size_t>(BaseT::_base_span); // move up to next block
            return true;
        }
        // reset end to the end of a first chunk
        pos.index=0;
        return false;
    }
    return true;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename IteratorT>
bool Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::add_it(std::size_t increment
                                                                              , IteratorT& current
                                                                              , SlicePosition<rank>& pos) const
{
    std::size_t span = BaseT::data_size();
    std::size_t rank_inc = increment/span;
    if(!BaseT::add_it(increment%span, current, pos)) {
        ++rank_inc;
    }
    std::size_t new_index = pos.index + rank_inc;
    if(new_index < _span.span()) {
        pos.index = new_index;
        current += rank_inc * static_cast<std::size_t>(BaseT::_base_span);
        return true;
    }
    std::size_t delta = new_index - _span.span();
    current += (delta - pos.index) * static_cast<std::size_t>(BaseT::_base_span);
    pos.index = delta;
    return false;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<typename IteratorDifferenceT>
IteratorDifferenceT Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::diff_it(IteratorDifferenceT const& diff) const
{
    if(diff < (IteratorDifferenceT)BaseT::_base_span) {
        return BaseT::diff_it(diff);
    }
    else {
        return IteratorDifferenceT(diff/BaseT::_base_span) * BaseT::data_size() + BaseT::diff_it(diff%BaseT::_base_span);
    }
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
template<bool const_type>
bool Slice<IsConst, SliceTraitsT, SliceMixin, Dimension, Dimensions...>::operator==(Slice<const_type, SliceTraitsT, SliceMixin, Dimension, Dimensions...> const& s) const
{
    return s.data_size() == data_size()
           && std::equal(s.cbegin(), s.cend(), cbegin());
}

// -------------------- single dimension specialisation -------------------
// constructors
template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice()
    : _span(DimensionSize<Dimension>(0U))
    , _base_span(DimensionSize<Dimension>(0U))
    , _parent(nullptr)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice(Parent& parent, DimensionSpan<Dimension> const& d)
    : _span(d)
    , _base_span(parent.template size<Dimension>())
    , _parent(&parent)
    , _ptr(parent.begin() + static_cast<std::size_t>(_span.start()))
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice(Slice const& copy)
    : _span(copy._span)
    , _base_span(copy._base_span)
    , _parent(copy._parent)
    , _ptr(copy._ptr)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename SliceT, typename IteratorType>
struct Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::OffsetJob
{
    OffsetJob(SliceT const& slice, IteratorType& ptr)
        : _slice(slice)
        , _ptr(ptr)
    {}

    template<typename Dim, typename Parent>
    inline
    void exec(Parent const& p) {
        _ptr += _slice.template parent_span<Dim>().start() * p.template block_size_t<Dim>();
    }

    private:
        SliceT const& _slice;
        IteratorType& _ptr;
};

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename SliceT>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice( typename std::enable_if<is_slice<SliceT>::value, Parent&>::type parent
                                                           , SliceT const& slice)
    : _span(slice.template parent_span<Dimension>())
    , _base_span(parent.template dimension<Dimension>())
    , _parent(&parent)
    , _ptr(parent.begin())
{
    typedef typename SliceT::SliceType SliceT_;
    OffsetJob<SliceT_, decltype(_ptr)> job(static_cast<SliceT_ const&>(slice), _ptr);
    parent.for_each_dimension(job);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice( internal_construct_tag const&
       , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ... spans)
    : _span(arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...))
    , _base_span(parent.template size<Dimension>())
    , _parent(&parent)
{
}


template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice( internal_construct_tag const&
       , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
       , DimensionSpan<Dims> const& ...)
    : _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(0), DimensionSize<Dimension>(parent.template size<Dimension>())))
    , _base_span(parent.template size<Dimension>())
    , _parent(&parent)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename SliceT>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice(
               internal_construct_tag const&
             , typename std::enable_if<is_slice<SliceT>::value, Parent&>::type parent
             , SliceT const& slice)
    : _span(slice.template parent_span<Dimension>())
    , _base_span(parent.template dimension<Dimension>())
    , _parent(&parent)
{
}


template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice( typename std::enable_if<!arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
    , Slice const& copy
    , DimensionSpan<Dims> const&...)
    : _span(copy._span)
    , _base_span(copy._base_span)
    , _parent(copy._parent)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename... Dims>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Slice( typename std::enable_if<arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
    , Slice const& copy
    , DimensionSpan<Dims> const&... spans)
    : _span(DimensionSpan<Dimension>(DimensionIndex<Dimension>(copy._span.start()
                           + arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...).start())
                           , arg_helper<DimensionSpan<Dimension> const&, DimensionSpan<Dims> const&...>::arg(spans...).span()))
    , _base_span(copy._base_span)
    , _parent(copy._parent)
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_iterator const& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::offset(parent_iterator const& it)
{
    _ptr = it + static_cast<std::size_t>(_span.start());
    return _ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::~Slice()
{
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::operator=(Slice const& copy)
{
    _span = copy._span;
    _base_span = copy._base_span;
    _parent = copy._parent;
    _ptr = copy._ptr;
    return *this;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
std::size_t Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::data_size() const
{
    return static_cast<std::size_t>(_span.span());
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::size() const
{
    return _span.span();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::dimension() const
{
    return _span.span();
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<((!std::is_same<Dim, Dimension>::value)
                        && (!has_dimension<typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent, Dim>::value))
                       , DimensionSize<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::size() const
{
    return DimensionSize<Dim>(0);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<(!std::is_same<Dim, Dimension>::value)
                        && has_dimension<typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent, Dim>::value
                       , DimensionSize<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::size() const
{
    return DimensionSize<Dim>(1);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<((!std::is_same<Dim, Dimension>::value)
                        && (!has_dimension<typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent, Dim>::value))
                       , DimensionSize<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::dimension() const
{
    return DimensionSize<Dim>(0);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<(!std::is_same<Dim, Dimension>::value)
                        && has_dimension<typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent, Dim>::value
                       , DimensionSize<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::dimension() const
{
    return DimensionSize<Dim>(1);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dimension>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::span() const
{
    return _span;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_span() const
{
    return _span;
}

/*
template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
constexpr
typename std::enable_if<((!std::is_same<Dim, Dimension>::value)
                          && (!has_dimension<typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent, Dim>::value))
                        , DimensionSpan<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::span() const
{
    return DimensionSpan<Dim>(DimensionSize<Dim>(0));;
}
*/

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
typename std::enable_if<((!std::is_same<Dim, Dimension>::value)
                          && (has_dimension<typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent, Dim>::value))
                        , DimensionSpan<Dim>>::type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_span() const
{
    return DimensionSpan<Dim>( _parent->template offset<Dim>(*this), DimensionSize<Dim>(1));;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::reference_type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::operator[](std::size_t p) const
{
    parent_iterator ptr = (_ptr + p);
    return *ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::reference_type Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::operator[](DimensionIndex<Dimension> const& p) const
{
    return *(_ptr + static_cast<std::size_t>(p));
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
Slice<IsConst, SliceTraitsT, SliceMixin, Dimension> Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::slice(DimensionSpan<Dimension> const& span)
{
    Slice r(*this);
    _ptr += static_cast<std::size_t>(_span.start());
    r._span.start(_span.start() + DimensionSize<Dimension>(span.start()));
    r._span.span(span.span());
    return r;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::ConstSliceType Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::slice(DimensionSpan<Dimension> const& span) const
{
    ConstSliceType r(*this);
    _ptr += static_cast<std::size_t>(_span.start());
    r._span.start(_span.start() + DimensionSize<Dimension>(span.start()));
    r._span.span(span.span());
    return r;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
std::size_t Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::base_span() const
{
    return static_cast<std::size_t>(_span.span());
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
std::size_t Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::diff_base_span() const
{
    return static_cast<std::size_t>(_span.span());
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_iterator const& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::base_ptr() const
{
    return _ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_iterator& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::base_ptr()
{
    return _ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::SelfType& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::operator+=(DimensionSize<Dimension> const& offset)
{
    _ptr += static_cast<std::size_t>(offset) * _base_span;
    return static_cast<SelfType&>(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::SelfType& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::operator+=(std::size_t offset)
{
    _ptr += offset * _base_span;
    return static_cast<SelfType&>(*this);
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::begin()
{
    return _ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::begin() const
{
    return _ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::cbegin() const
{
    return _ptr;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::end()
{
    return _ptr + static_cast<std::size_t>(_span.span());
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::end() const
{
    return _ptr + static_cast<std::size_t>(_span.span());
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent_const_iterator Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::cend() const
{
    return _ptr + static_cast<std::size_t>(_span.span());
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
inline typename Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::Parent& Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::parent() const
{
    return *_parent;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename IteratorT>
bool Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::increment_it(IteratorT& current, SlicePosition<rank>& pos) const
{
    ++current;
    if(++pos.index < _span.span())
    {
        return true;
    }
    pos.index=0;
    return false;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename IteratorT>
bool Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::add_it(std::size_t increment, IteratorT& current, SlicePosition<rank>& pos) const
{
    std::size_t new_index = pos.index + increment;
    if(new_index < _span.span())
    {
        pos.index = new_index;
        current += increment;
        return true;
    }
    current -= pos.index;
    pos.index = new_index - _span.span();
    current += pos.index;
    return false;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<typename IteratorDifferenceT>
IteratorDifferenceT Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::diff_it(IteratorDifferenceT const& diff)
{
    return diff;
}

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
template<bool IsConst_>
bool Slice<IsConst, SliceTraitsT, SliceMixin, Dimension>::operator==(Slice<IsConst_, SliceTraitsT, SliceMixin, Dimension> const& s) const
{
    return s.data_size() == data_size()
        && std::equal(s.begin(), s.end(), begin());
}

template<typename SliceType>
struct RemoveMixinWrapper
{
    typedef SliceType type;
};

template<template<typename> class MixinT, bool IsConst, typename SliceTraitsT, typename... Dimensions>
struct RemoveMixinWrapper<MixinT<Slice<IsConst, SliceTraitsT, MixinT, Dimensions...>>>
{
    typedef Slice<IsConst, SliceTraitsT, MixinT, Dimensions...> type;
};

template<typename SliceType, template<typename> class MixinT>
struct RemoveMixinWrapper<MixinT<SliceType>>
{
    typedef typename RemoveMixinWrapper<SliceType>::type type;
};

template<typename SliceType>
struct RestoreMixinWrapper {
    public:
        typedef decltype(SliceType::template slice_mixin_dummy<SliceType>()) type;
};

template<typename SliceType>
struct FlipConstTypeHelper
{
    private:
        typedef typename RemoveMixinWrapper<SliceType>::type InnerSliceType;
        typedef typename RestoreMixinWrapper<typename InnerSliceType::FlipSelfConstType>::type FlippedType;
    public:
        typedef typename std::conditional<std::is_const<SliceType>::value, const FlippedType, FlippedType>::type type;
};

template<bool IsConst, typename SliceTraitsT, template<typename> class SliceMixin, typename... Dimensions>
struct FlipConstTypeHelper<Slice<IsConst, SliceTraitsT, SliceMixin, Dimensions...>>
{
        typedef Slice<IsConst, SliceTraitsT, SliceMixin, Dimensions...> SliceType;
        typedef typename SliceType::FlipSelfConstType FlippedType;

    public:
        typedef typename std::conditional<std::is_const<SliceType>::value, const FlippedType, FlippedType>::type type;
};

template<typename SliceT, typename>
struct FlipConstType {
    typedef typename std::remove_reference<SliceT>::type SliceType;
    typedef typename std::conditional<std::is_const<SliceType>::value
                           , typename std::remove_const<SliceType>::type
                           , SliceType const>::type type;
};

template<typename SliceT>
struct FlipConstType<SliceT, typename std::enable_if<is_slice<SliceT>::value>::type>
    : public FlipConstTypeHelper<typename std::remove_reference<SliceT>::type>
{
};

template<typename T, typename = void>
struct FlipConstHelper
{
    static inline typename FlipConstType<T>::type& exec(T& v) {
        return const_cast<T const&>(v);
    }
};

template<typename T>
struct FlipConstHelper<T, typename std::enable_if<std::is_const<typename std::remove_reference<T>::type>::value && !is_slice<T>::value>::type>
{
    static inline typename FlipConstType<T>::type& exec(T& v) {
        return const_cast<typename FlipConstType<T>::type&>(v);
    }
};

template<typename SliceType>
struct FlipConstHelper<SliceType, typename std::enable_if<is_slice<typename std::remove_reference<SliceType>::type>::value>::type>
{
    static inline typename FlipConstType<SliceType>::type& exec(SliceType& slice) {
        return reinterpret_cast<typename FlipConstType<SliceType>::type&>(slice);
    }
};

template<typename SliceType>
auto flip_const(SliceType&& slice) -> typename FlipConstType<SliceType>::type&
{
    return FlipConstHelper<SliceType>::exec(std::forward<SliceType>(slice));
}

template<typename SliceT>
struct CastToConstSliceType
{
    typedef typename std::remove_reference<SliceT>::type SliceType;
    typedef typename std::conditional<SliceType::is_const(), SliceT, typename FlipConstType<SliceType>::type>::type type;
};

template<typename SliceT>
struct CastToNonConstSliceType
{
    typedef typename std::remove_reference<SliceT>::type SliceType;
    typedef typename std::conditional<!SliceType::is_const(), SliceT, typename FlipConstType<SliceType>::type>::type type;
};

template<typename SliceType, typename=void>
struct CastToConstHelper;

template<typename SliceType>
struct CastToConstHelper<SliceType, typename std::enable_if<is_slice<SliceType>::value && SliceType::is_const()>::type> {
    static inline
    typename CastToConstSliceType<SliceType>::type& exec(SliceType& slice) {
        return slice;
    }
};

template<typename SliceType>
struct CastToConstHelper<SliceType, typename std::enable_if<is_slice<SliceType>::value && !SliceType::is_const()>::type> {
    static inline
    typename CastToConstSliceType<SliceType>::type& exec(SliceType& slice) {
        return flip_const(slice);;
    }
};

template<typename SliceType, typename=void>
struct CastToNonConstHelper;

template<typename SliceType>
struct CastToNonConstHelper<SliceType, typename std::enable_if<is_slice<SliceType>::value && !SliceType::is_const()>::type>
{
    static inline
    typename CastToNonConstSliceType<SliceType>::type& exec(SliceType& slice) {
        return slice;
    }
};

template<typename SliceType>
struct CastToNonConstHelper<SliceType, typename std::enable_if<is_slice<SliceType>::value && SliceType::is_const()>::type> {
    static inline
    typename CastToNonConstSliceType<SliceType>::type& exec(SliceType& slice) {
        return flip_const(slice);;
    }
};

template<typename SliceType>
inline typename CastToConstSliceType<SliceType>::type& cast_to_const_slice(SliceType&& slice)
{
    return CastToConstHelper<typename std::remove_reference<SliceType>::type>::exec(std::forward<SliceType>(slice));
}

template<typename SliceType>
inline typename CastToNonConstSliceType<SliceType>::type& cast_to_non_const_slice(SliceType&& slice)
{
    return CastToNonConstHelper<typename std::remove_reference<SliceType>::type>::exec(std::forward<SliceType>(slice));
}

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
