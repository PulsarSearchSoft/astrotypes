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
#include <functional>

namespace pss {
namespace astrotypes {

// type traits helper structs
template<typename T>
struct is_multiarray : public std::is_base_of<multiarray::MultiArrayTag, typename std::decay<T>::type> {};

// for any multiarray type we can ask the object directly for its dimension
template<typename T, typename Dimension>
struct enabler2<has_dimension, T, Dimension, typename std::enable_if<is_multiarray<T>::value>::type>
    : public std::conditional< T::template has_dimension<Dimension>()
                             , std::true_type
                             , std::false_type>::type
{
};

template<typename T, typename DimensionTuple>
struct enabler2<has_exact_dimensions, T, DimensionTuple, typename std::enable_if<is_multiarray<T>::value>::type>
    : public std::is_same<DimensionTuple, typename T::DimensionTuple>
{
};

////////////////////////////////////////////////
// type deduction helpers
// /////////////////////////////////////////////
namespace multiarray {
template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
struct MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::OperatorSliceType
{
    typedef typename OperatorSliceTypeHelper<Dim, typename MultiArray::SliceType>::type type;
};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
struct MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::ConstOperatorSliceType
{
    typedef typename ConstOperatorSliceTypeHelper<Dim, typename MultiArray::SliceType>::type type;
};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
struct MultiArray<Alloc, T, SliceMixin, Dimension>::OperatorSliceType
{
    typedef typename OperatorSliceTypeHelper<Dim, typename MultiArray::SliceType>::type type;
};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename Dimension>
template<typename Dim>
struct MultiArray<Alloc, T, SliceMixin, Dimension>::ConstOperatorSliceType
{
    typedef typename ConstOperatorSliceTypeHelper<Dim, typename MultiArray::SliceType>::type type;
};

////////////////////////////////////////////////
// generic dimension implementation
////////////////////////////////////////////////
// public interface for ensuring all is correctly sized up
template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::MultiArray()
    : BaseT()
    , _size(DimensionSize<FirstDimension>(0))
{
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::MultiArray(DimensionSize<Dim> size, DimensionSize<Dims>... sizes)
    : BaseT(false, size, sizes...)
    , _size(arg_helper<DimensionSize<FirstDimension> const&, DimensionSize<Dim> const&, DimensionSize<Dims> const&...>::arg(size, sizes...))
{
    resize(arg_helper<DimensionSize<FirstDimension> const&, DimensionSize<Dim> const&, DimensionSize<Dims> const&...>::arg(size, sizes...));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename DimensionType, typename Enable>
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::MultiArray(DimensionType const& d)
    : BaseT(false, d)
    , _size(d.template dimension<FirstDimension>())
{
    resize(d.template dimension<FirstDimension>());
    do_transpose(*this, d);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename DimensionType>
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::MultiArray(
      typename std::enable_if<has_dimensions<DimensionType, FirstDimension, Dimensions...>::value, bool>::type
    , DimensionType const& d
    )
    : BaseT(false, d)
    , _size(d.template dimension<FirstDimension>())
{
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename MultiArrayType, typename DimensionType>
void MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_transpose(MultiArrayType& ma, DimensionType const& d)
{
    for(DimensionIndex<FirstDimension> i(0); i < _size; ++i) {
        auto slice = ma[i];
        BaseT::do_transpose(slice, d[i]);
    }
}


// private interface for constructing in an inheritance stack
template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename... Dims>
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::MultiArray(
                                  typename std::enable_if<arg_helper<FirstDimension, Dims...>::value, bool>::type
                                , DimensionSize<Dims> const&... sizes)
    : BaseT(false, sizes...)
    , _size(arg_helper<DimensionSize<FirstDimension> const&, DimensionSize<Dims> const&...>::arg(sizes...))
{
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::iterator MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::begin()
{
    return BaseT::begin();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::begin() const
{
    return BaseT::begin();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::cbegin() const
{
    return BaseT::cbegin();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::iterator MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::end()
{
    return BaseT::end();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::end() const
{
    return BaseT::end();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::cend() const
{
    return BaseT::cend();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::ReducedDimensionSliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::operator[](DimensionIndex<FirstDimension> index)
{
    return SliceType(*this, DimensionSpan<FirstDimension>(index, DimensionSize<FirstDimension>(1))
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template dimension<Dimensions>()))...)[DimensionIndex<FirstDimension>(0)];
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::ConstReducedDimensionSliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::operator[](DimensionIndex<FirstDimension> index) const
{
    return ConstSliceType(*this, DimensionSpan<FirstDimension>(index, DimensionSize<FirstDimension>(1))
                          , DimensionSpan<Dimensions>(DimensionIndex<Dimensions>(0), DimensionSize<Dimensions>(this->template dimension<Dimensions>()))...)[DimensionIndex<FirstDimension>(0)];
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<has_dimension<MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>, Dim>::value
&& !std::is_same<Dim, FirstDimension>::value, typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::template ConstOperatorSliceType<Dim>::type>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::operator[](DimensionIndex<Dim> const& index) const
{
   return ConstSliceType(*this, DimensionSpan<Dim>(index, DimensionSize<Dim>(1)));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename SliceType>
struct MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::SliceReturnType
{
    typedef SliceReturnTypeHelper<false, MultiArray, DimensionTuple, SliceType> Helper;
    typedef typename Helper::type type;
    typedef typename Helper::slice_type slice_type;
};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename SliceType>
struct MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::ConstSliceReturnType
{
    typedef SliceReturnTypeHelper<true, MultiArray, DimensionTuple, SliceType> Helper;
    typedef typename Helper::type type;
    typedef typename Helper::slice_type slice_type;
};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<has_dimension<MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>, Dim>::value
&& !std::is_same<Dim, FirstDimension>::value, typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::template OperatorSliceType<Dim>::type>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::operator[](DimensionIndex<Dim> const& index)
{
   return SliceType(*this, DimensionSpan<Dim>(index, DimensionSize<Dim>(1)));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::ConstSliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::slice(DimensionSpan<Dim> const& range, DimensionSpan<Dims> const&... ranges) const
{
    return ConstSliceType(*this, range, ranges...);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::SliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::slice(DimensionSpan<Dim> const& range, DimensionSpan<Dims> const&... ranges)
{
    return SliceType(*this, range, ranges...);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::SliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::overlay(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, FirstDimension, Dimensions...>> const& slice)
{
    return SliceType(*this, slice);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::SliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::overlay(Slice<is_const, SliceTraitsT, SliceMixin2, FirstDimension, Dimensions...> const& slice)
{
    return SliceType(*this, slice);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::ConstSliceType MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::overlay(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, FirstDimension, Dimensions...>> const& slice) const
{
    return ConstSliceType(*this, slice);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... OtherDimensions>
template<typename SliceArgType>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, OtherDimensions...>::template SliceReturnType<SliceArgType>::type MultiArray<Alloc, T, SliceMixin, FirstDimension, OtherDimensions...>::overlay(SliceArgType const& slice)
{
    typedef typename SliceReturnType<SliceArgType>::type ReturnType;
    typedef typename SliceReturnType<SliceArgType>::slice_type IntSliceType;
    return ReturnType(IntSliceType(*this, slice));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... OtherDimensions>
template<typename SliceArgType>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension, OtherDimensions...>::template ConstSliceReturnType<SliceArgType>::type MultiArray<Alloc, T, SliceMixin, FirstDimension, OtherDimensions...>::overlay(SliceArgType const& slice) const
{
    typedef typename ConstSliceReturnType<SliceArgType>::type ReturnType;
    typedef typename ConstSliceReturnType<SliceArgType>::slice_type IntSliceType;
    return ReturnType(IntSliceType(*this, slice));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension, typename SliceArgumentType>
typename std::enable_if<has_dimension_strict<SliceArgumentType, Dimension>::value
                     && is_slice<SliceArgumentType>::value
                      , DimensionIndex<Dimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_offset(SliceArgumentType const& slice) const
{
    return slice.template span<Dimension>().start();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension, typename SliceArgumentType>
typename std::enable_if<!has_dimension_strict<SliceArgumentType, Dimension>::value
                     && is_slice<SliceArgumentType>::value
                      , DimensionIndex<Dimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_offset(SliceArgumentType const& slice) const
{
    auto delta = &*slice.begin() - &*slice.parent().cbegin();
    return this->template calculate_offset<Dimension>(delta);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension, typename SliceArgumentType>
inline typename std::enable_if<is_slice<SliceArgumentType>::value, DimensionIndex<Dimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::offset(SliceArgumentType const& slice) const
{
    return this->do_offset<Dimension>(slice);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Job>
void MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::for_each_dimension(Job& job) const
{
    job.template exec<FirstDimension>(*this);
    BaseT::for_each_dimension(job);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension, bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2, typename SliceDim, typename... SliceDimensions>
inline DimensionIndex<Dimension> MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::offset(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, SliceDim, SliceDimensions...>> const& slice) const
{
    return this->template offset<Dimension>(static_cast<Slice<is_const, SliceTraitsT, SliceMixin2, SliceDim, SliceDimensions...> const&>(slice));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension>
typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, DimensionIndex<Dimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::calculate_offset(std::size_t delta) const
{
    return DimensionIndex<Dimension>(delta / (std::size_t)this->BaseT::block_size());
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension>
typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, DimensionIndex<Dimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::calculate_offset(std::size_t delta) const
{
    return BaseT::template calculate_offset<Dimension>(delta % (std::size_t)this->BaseT::block_size());
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
std::size_t MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::block_size() const
{
    return this->template dimension<FirstDimension>() * BaseT::block_size();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename... Dims>
void MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::resize(DimensionSize<Dims>... size)
{
    this->do_resize(1,  std::forward<DimensionSize<Dims>>(size)...);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
void MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::resize(DimensionSize<Dim> size_1, DimensionSize<Dims>... size, T const& value)
{
    this->do_resize<Dim, Dims...>(1, size_1,  std::forward<DimensionSize<Dims>>(size)..., value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::size() const
{
    return BaseT::template dimension<Dim>();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::size() const
{
    return _size;
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::dimension() const
{
    return BaseT::template dimension<Dim>();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::dimension() const
{
    return _size;
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename std::enable_if<!arg_helper<FirstDimension, Dim, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>... sizes)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size), size, std::forward<DimensionSize<Dims>>(sizes)...);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename std::enable_if<!arg_helper<FirstDimension, Dim, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>... sizes, T const& value)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size), size, std::forward<DimensionSize<Dims>>(sizes)..., value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename std::enable_if<arg_helper<FirstDimension, Dim, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>... sizes)
{
    _size = arg_helper<DimensionSize<FirstDimension>, DimensionSize<Dim>, DimensionSize<Dims>...>::arg(std::forward<DimensionSize<Dim>>(size), std::forward<DimensionSize<Dims>>(sizes)...);
    BaseT::do_resize(total * static_cast<std::size_t>(_size), size, std::forward<DimensionSize<Dims>>(sizes)...);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dim, typename... Dims>
typename std::enable_if<arg_helper<FirstDimension, Dim, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>... sizes, T const& value)
{
    _size = arg_helper<DimensionSize<FirstDimension>, DimensionSize<Dim>, DimensionSize<Dims>...>::arg(std::forward<DimensionSize<Dim>>(size), std::forward<DimensionSize<Dims>>(sizes)...);
    BaseT::template do_resize<Dim, Dims...>(total * static_cast<std::size_t>(_size), size, std::forward<DimensionSize<Dims>>(sizes)..., value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
void MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_resize(std::size_t total)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
void MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::do_resize(std::size_t total, T const& value)
{
    BaseT::do_resize(total * static_cast<std::size_t>(_size), value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
std::size_t MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::data_size() const
{
    return BaseT::data_size();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
bool MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::equal_size(MultiArray const& o) const
{
    return _size == o.dimension<FirstDimension>() && BaseT::equal_size(static_cast<BaseT const&>(o));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
bool MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::operator==(MultiArray const& o) const
{
    return equal_size(o) && std::equal(o.cbegin(), o.cend(), cbegin());
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension>
typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::block_size_t() const
{
    return this->BaseT::block_size();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... Dimensions>
template<typename Dimension>
typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension, Dimensions...>::block_size_t() const
{
    return BaseT::template block_size_t<Dimension>();
}

/////////////////////////////////////////////////////////////
// Single Dimension specialisation
/////////////////////////////////////////////////////////////
template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
MultiArray<Alloc, T, SliceMixin, FirstDimension>::MultiArray()
    : _size(DimensionSize<FirstDimension>(0))
{
    _data.resize(_size);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim, typename... Dims>
MultiArray<Alloc, T, SliceMixin, FirstDimension>::MultiArray(DimensionSize<Dim> const& size, DimensionSize<Dims> const&... sizes)
    : _size(arg_helper<DimensionSize<FirstDimension> const&, DimensionSize<Dim> const&, DimensionSize<Dims> const&...>::arg(size, sizes...))
{
    _data.resize(_size);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename... Dims>
MultiArray<Alloc, T, SliceMixin, FirstDimension>::MultiArray(
        typename std::enable_if<arg_helper<FirstDimension, Dims...>::value, bool>::type
        , DimensionSize<Dims> const&... sizes)
    : _size(arg_helper<DimensionSize<FirstDimension> const&, DimensionSize<Dims> const&...>::arg(sizes...))
{
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename DimensionType>
MultiArray<Alloc, T, SliceMixin, FirstDimension>::MultiArray(
      typename std::enable_if<astrotypes::has_dimension<DimensionType, FirstDimension>::value, bool>::type
    , DimensionType const& d
    )
    : _size(d.template dimension<FirstDimension>())
{
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
MultiArray<Alloc, T, SliceMixin, FirstDimension>::~MultiArray()
{
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::size() const
{
    return DimensionSize<Dim>(0);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::size() const
{
    return _size;
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::dimension() const
{
    return DimensionSize<Dim>(0);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::dimension() const
{
    return _size;
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
std::size_t MultiArray<Alloc, T, SliceMixin, FirstDimension>::data_size() const
{
    return this->_data.size();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim>
void MultiArray<Alloc, T, SliceMixin, FirstDimension>::resize(DimensionSize<Dim> size)
{
    this->do_resize(1,  size);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim>
void MultiArray<Alloc, T, SliceMixin, FirstDimension>::resize(DimensionSize<Dim> size, T const& value)
{
    this->do_resize(1,  size, value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename... Dims>
typename std::enable_if<!arg_helper<FirstDimension, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dims>...)
{
    _data.resize(total * static_cast<std::size_t>(_size));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename... Dims>
typename std::enable_if<!arg_helper<FirstDimension, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dims>..., T const& value)
{
    _data.resize(total * static_cast<std::size_t>(_size), value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim, typename... Dims>
typename std::enable_if<arg_helper<FirstDimension, Dim, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>... sizes)
{
    _size = arg_helper<DimensionSize<FirstDimension>, DimensionSize<Dim>, DimensionSize<Dims>...>::arg(std::forward<DimensionSize<Dim>>(size), std::forward<DimensionSize<Dims>>(sizes)...);
    _data.resize(total * static_cast<std::size_t>(_size));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dim, typename... Dims>
typename std::enable_if<arg_helper<FirstDimension, Dim, Dims...>::value, void>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::do_resize(std::size_t total, DimensionSize<Dim> size, DimensionSize<Dims>... sizes, T const& value)
{
    _size = arg_helper<DimensionSize<FirstDimension>, DimensionSize<Dim>, DimensionSize<Dims>...>::arg(std::forward<DimensionSize<Dim>>(size), std::forward<DimensionSize<Dims>>(sizes)...);
    _data.resize(total * static_cast<std::size_t>(_size), value);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dimension>
DimensionIndex<Dimension> MultiArray<Alloc, T, SliceMixin, FirstDimension>::calculate_offset(std::size_t delta) const
{
    static_assert(std::is_same<Dimension, FirstDimension>::value, "Request for offset for a dimension not supported by this structure");
    return DimensionIndex<Dimension>(delta / (std::size_t)this->template dimension<FirstDimension>());
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::reference_type MultiArray<Alloc, T, SliceMixin, FirstDimension>::operator[](DimensionIndex<FirstDimension> index)
{
    return *(begin() + static_cast<std::size_t>(index));
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::iterator MultiArray<Alloc, T, SliceMixin, FirstDimension>::begin()
{
    return _data.begin();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension>::begin() const
{
    return _data.begin();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension>::cbegin() const
{
    return _data.cbegin();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::iterator MultiArray<Alloc, T, SliceMixin, FirstDimension>::end()
{
    return _data.end();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension>::end() const
{
    return _data.end();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
typename MultiArray<Alloc, T, SliceMixin, FirstDimension>::const_iterator MultiArray<Alloc, T, SliceMixin, FirstDimension>::cend() const
{
    return _data.cend();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
bool MultiArray<Alloc, T, SliceMixin, FirstDimension>::operator==(MultiArray const& o) const
{
    return _size == o.dimension<FirstDimension>()
           && std::equal(o.cbegin(), o.cend(), cbegin());
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
bool MultiArray<Alloc, T, SliceMixin, FirstDimension>::equal_size(MultiArray const& o) const
{
    return _size == o.dimension<FirstDimension>();
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dimension>
constexpr
typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::block_size_t() const
{
    return 1;
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Dimension>
typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
MultiArray<Alloc, T, SliceMixin, FirstDimension>::block_size_t() const
{
    static_assert(!std::is_same<Dimension, FirstDimension>::value, "programming error - you shouldn't be calling this");
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename Job>
void MultiArray<Alloc, T, SliceMixin, FirstDimension>::for_each_dimension(Job& job) const
{
    job.template exec<FirstDimension>(*this);
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
template<typename MultiArrayType, typename DimensionType>
void MultiArray<Alloc, T, SliceMixin, FirstDimension>::do_transpose(MultiArrayType& ma, DimensionType const& d)
{
    for(DimensionIndex<FirstDimension> i(0); i < _size; ++i) {
        ma[i] = d[i];
    }
}

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
std::size_t MultiArray<Alloc, T, SliceMixin, FirstDimension>::block_size() const
{
    return static_cast<std::size_t>(_size);
}

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
