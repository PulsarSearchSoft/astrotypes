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
#ifndef PSS_ASTROTYPES_MULTIARRAY_SLICE_H
#define PSS_ASTROTYPES_MULTIARRAY_SLICE_H
#include "DimensionSpan.h"
#include "SliceIterator.h"
#include "TypeTraits.h"
#include "detail/SlicePosition.h"
#include <utility>

namespace pss {
namespace astrotypes {

/**
 * @brief struct ot help determine if a specified type is a slice
 * @details
 * @code
 * typename std::enable_if<is_slice<SomeType>::value>::type
 * @endcode
 */
template<typename T>
struct is_slice;

template<typename T, typename... Dims>
struct arg_helper : public std::false_type
{
};

template<typename T, typename... Dims>
struct arg_helper<T, T, Dims...> : public std::true_type
{
    static inline
    T&& arg(T&& arg, Dims&&...)
    { return std::forward<T>(arg); }
};

template<typename T, typename Dim, typename... Dims>
struct arg_helper<T, Dim, Dims...> : public arg_helper<T, Dims...>
{
    typedef arg_helper<T, Dims...> BaseT;

    static inline
    T&& arg(Dim&&, Dims&&...args)
    { return BaseT::arg(std::forward<Dims>(args)...); }
};

namespace multiarray {
/**
 * @class Slice
 * @brief
 *      Representation of a Slice through a Data Array
 * @tparam ParnetT the data structure that the slice will be based on
 * @tparam is_const true if the ParentT is const, false otherwise
 * @tparam SliceMixin A template wrapper class around a slice that allows
 *         you to propagate a custom interface to any slice to be consistent
 *         with your main data structure interface.
 * @details
 */
struct copy_resize_construct_tag {};
struct copy_resize_construct_base_tag {};
struct internal_construct_tag{};

template<typename ParentT, typename ExcludeDimension>
struct InternalSliceTraits  {
    typedef ParentT Parent;
    typedef std::tuple<ExcludeDimension> ExcludeTuple;
};

template<typename Traits, typename ExcludeDimension, typename D>
struct InternalSliceTraits<InternalSliceTraits<Traits, D>, ExcludeDimension> : public InternalSliceTraits<Traits, D>
{
    private:
        typedef InternalSliceTraits<Traits, D> BaseT;
    public:
        typedef typename BaseT::Parent Parent;
        typedef typename join_tuples<typename BaseT::ExcludeTuple, std::tuple<ExcludeDimension>>::type ExcludeTuple;
};

template<typename TraitsT>
struct SliceTraitsHelper
{
    typedef std::tuple<> ExcludeTuple;
    typedef TraitsT Parent;
};

template<typename TraitsT, typename D>
struct SliceTraitsHelper<InternalSliceTraits<TraitsT, D>>
{
    typedef typename InternalSliceTraits<TraitsT, D>::ExcludeTuple ExcludeTuple;
    typedef typename InternalSliceTraits<TraitsT, D>::Parent Parent;
};

template<bool Value>
struct Flip
{
    constexpr static bool const value = !Value;
};
static_assert(Flip<true>::value==false, "Flip broken");
static_assert(Flip<false>::value==true, "Flip broken");

template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension, typename... Dimensions>
//class Slice : private Slice<is_const, SliceTraitsT, SliceMixin, Dimensions...>
class Slice : private Slice<is_const, InternalSliceTraits<SliceTraitsT, Dimension>, SliceMixin, Dimensions...>
{
        typedef typename SliceTraitsHelper<SliceTraitsT>::Parent ParentT;
        typedef Slice<is_const, InternalSliceTraits<SliceTraitsT, Dimension>, SliceMixin, Dimensions...> BaseT;
        typedef Slice<is_const, SliceTraitsT, SliceMixin, Dimension, Dimensions...> SelfType;
        typedef Slice<Flip<is_const>::value, SliceTraitsT, SliceMixin, Dimension, Dimensions...> FlipConstSelfType;
        friend FlipConstSelfType;
        typedef typename ParentT::value_type value_type;
        typedef Dimension SelfDimension;

        typedef typename ParentT::const_iterator parent_const_iterator;
        typedef typename std::conditional<is_const, parent_const_iterator, typename ParentT::iterator>::type parent_iterator;

    protected:
        typedef typename SliceTraitsHelper<SliceTraitsT>::ExcludeTuple ExcludeTuple;

    public:
        typedef std::tuple<Dimension, Dimensions...> DimensionTuple;

        /// provides a template to determine the returned type of an operator[]
        //  @tparam Dim  the dimension that will be called
        //  @param type The type that will we returned by the operator[DimensionIndex<Dim>]
        template<typename Dim>
        struct OperatorSliceType;

        template<typename Dim>
        struct ConstOperatorSliceType;

        typedef typename std::conditional<is_const, const ParentT, ParentT>::type Parent;
        typedef SliceIterator<SliceMixin<SelfType>, is_const> iterator;
        typedef SliceIterator<SliceMixin<SelfType>, true> const_iterator;
        typedef SliceMixin<Slice<is_const, SliceTraitsT, SliceMixin, Dimension, Dimensions...>> SliceType;
        typedef SliceMixin<BaseT> ReducedSliceType;
        typedef SliceMixin<Slice<true, SliceTraitsT, SliceMixin, Dimension, Dimensions...>> ConstSliceType;
        typedef SliceMixin<Slice<true, InternalSliceTraits<SliceTraitsT, Dimension>, SliceMixin, Dimensions...>> ConstReducedSliceType;

    public:
        explicit Slice();

        /// constructor - span matching Dimension provided
        template<typename Dim, typename... Dims>
        Slice( typename std::enable_if<arg_helper<Dimension, Dim, Dims...>::value, Parent&>::type parent
             , DimensionSpan<Dim> const&
             , DimensionSpan<Dims> const& ...);

        /// constructor - default span for Dimension
        template<typename Dim, typename... Dims>
        Slice( typename std::enable_if<!arg_helper<Dimension, Dim, Dims...>::value, Parent&>::type parent
             , DimensionSpan<Dim> const&
             , DimensionSpan<Dims> const& ...);

        /// constructor - spans extracted from provided Slice with exact same dimensions as this slice
        template<bool is_const2, typename SliceTraitsT2, template<typename> class SliceMixin2, typename SliceDim, typename... SliceDimensions>
        Slice( Parent&, SliceMixin2<Slice<is_const2, SliceTraitsT2, SliceMixin2, SliceDim, SliceDimensions...>> const& slice);

/*
        template<bool is_const2, typename SliceTraitsT2, template<typename> class SliceMixin2, typename SliceDim>
        Slice( Parent&, SliceMixin2<Slice<is_const2, SliceTraitsT2, SliceMixin2, SliceDim>> const& slice);
*/

        static constexpr std::size_t rank = 1 + sizeof...(Dimensions);

        /**
         * @brief return true if this slice has dimesion D
         */
        template<typename D>
        static constexpr bool has_dimension() { return has_type<std::tuple<Dimension, Dimensions...>, D>::value; }

        /**
         * @brief the total number of data members in this slice
         */
        std::size_t data_size() const;

        /**
         * @ brief return the size of the slice in the specified dimension
         * @code
         *      Slice<DimesionA, DimensionB> slice(...some initialisers...);
         *      DimensionSize<DimensionA> size_of_A = slice.size<DimensionA>();
         *      DimensionSize<DimensionB> size_of_B = slice.size<DimensionB>();
         *      // note you can remove the dimension tagging on the DimesionSize
         *      // objects for compatibilty with less strongly types libs
         *      // with a static_cast
         *      std::size_t size_A_as_size_t = static_cast<std::size_t>(size_of_A);
         * @endcode
         * @details
         *      If you intend to inherit from this class you may need to call this method with the
         *      template keyword.
         *      e,g,
         *      @code
         *      slice.template size<DimensionA>();
         *      @endcode
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
        size() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type
        size() const;

        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
        dimension() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type
        dimension() const;

        /**
         * @brief the span of the slice in the specified dimension
         * @details the offset and the size of the specified dimension
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dimension>>::type
        span() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSpan<Dim>>::type
        span() const;

        /**
         * @brief Take a slice in the specified dimension with a span of 1
         */
        //Slice<is_const, SliceTraitsT, SliceMixin, Dimensions...> operator[](DimensionIndex<Dimension>) const;
        typename OperatorSliceType<Dimension>::type operator[](DimensionIndex<Dimension>);
        typename ConstOperatorSliceType<Dimension>::type operator[](DimensionIndex<Dimension>) const;

        template<typename Dim>
        typename std::enable_if<arg_helper<Dim, Dimensions...>::value
                            && !std::is_same<Dim, Dimension>::value, typename ConstOperatorSliceType<Dim>::type>::type
        operator[](DimensionIndex<Dim> const&) const;

        template<typename Dim>
        typename std::enable_if<arg_helper<Dim, Dimensions...>::value
                            && !std::is_same<Dim, Dimension>::value, typename OperatorSliceType<Dim>::type>::type
        operator[](DimensionIndex<Dim> const&);

        /**
         * @brief Take a sub-slice from this slice
         * @params pass a DimensionSpan<Dimension> object for each dimension
         *         you wish to restrict.
         *         Span indexes are relative to Slice boundary, not the parent
         */
        template<typename... Dims>
        typename std::enable_if<arg_helper<Dimension, Dims...>::value, SliceType>::type
        slice(DimensionSpan<Dims> const&... spans);

        template<typename... Dims>
        typename std::enable_if<!arg_helper<Dimension, Dims...>::value, SliceType>::type
        slice(DimensionSpan<Dims> const&... spans);

        template<typename... Dims>
        typename std::enable_if<arg_helper<Dimension, Dims...>::value, ConstSliceType>::type
        slice(DimensionSpan<Dims> const&... spans) const;

        template<typename... Dims>
        typename std::enable_if<!arg_helper<Dimension, Dims...>::value, ConstSliceType>::type
        slice(DimensionSpan<Dims> const&... spans) const;

        /**
         * @brief iterator pointing to the first element in the slice
         */
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        /**
         * @brief return refernce to the parent object the Slice is based on
         */
        Parent& parent() const;

        /**
         * @brief compare two arrays
         */
        template<bool const_type>
        bool operator==(Slice<const_type, SliceTraitsT, SliceMixin, Dimension, Dimensions...> const&) const;

    protected:
        typedef SliceIterator<SelfType, is_const> impl_iterator;
        typedef SliceIterator<SelfType, true> impl_const_iterator;
        friend typename impl_iterator::BaseT;
        friend typename impl_iterator::ImplT;
        friend typename impl_const_iterator::BaseT;
        friend typename impl_const_iterator::ImplT;


    protected:
        template<bool, typename P, template<typename> class, typename D, typename... Ds> friend class Slice;

        template<typename IteratorT> bool increment_it(IteratorT& current, SlicePosition<rank>& pos) const;
        template<typename IteratorDifferenceT> IteratorDifferenceT diff_it(IteratorDifferenceT const& diff) const;

        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dimension>>::type
        parent_span() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSpan<Dim>>::type
        parent_span() const;

        // return the span of all lower dimensions than this one (i.e an index of +1 in this dimension)
        std::size_t base_span() const;
        std::size_t diff_base_span() const;

        // ptr to the start of the block
        parent_iterator const& base_ptr() const;
        parent_iterator& base_ptr();

        parent_iterator const& offset(parent_iterator const&); // init the offset relative to the top parent

        // init for inheriting classes only
        template<typename... Dims>
        Slice( internal_construct_tag const&
             , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
             , DimensionSpan<Dims> const& ... spans);

        template<typename... Dims>
        Slice( internal_construct_tag const&
             , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
             , DimensionSpan<Dims> const& ... spans);

        template<bool is_const2, typename SliceTraitsT2, template<typename> class SliceMixin2, typename SliceDim, typename... Dimensions2>
        Slice( internal_construct_tag const&
             , Parent&
             , SliceMixin2<Slice<is_const2, SliceTraitsT2, SliceMixin2, SliceDim, Dimensions2...>> const& slice);

        template<typename... Dims>
        Slice( copy_resize_construct_tag const&
             , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims> const&... spans );

        template<typename... Dims>
        Slice( copy_resize_construct_base_tag const&
             , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims> const&... spans );

        template<typename... Dims>
        Slice( copy_resize_construct_tag const&
             , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims> const&... spans );

        template<typename... Dims>
        Slice( copy_resize_construct_base_tag const&
             , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims> const&... spans );

        // increment pointer by a n * span length
        SelfType& operator+=(std::size_t n);

        // increment pointer by a n * base span length
        SelfType& operator+=(DimensionSize<Dimension> n);

        /**
         * @brief iterator pointing to the first element in the slice
         */
        impl_iterator impl_begin();
        impl_const_iterator impl_begin() const;
        impl_const_iterator impl_cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        impl_iterator impl_end();
        impl_const_iterator impl_end() const;
        impl_const_iterator impl_cend() const;

    private:
        friend typename iterator::BaseT;
        friend typename iterator::ImplT;
        friend typename const_iterator::BaseT;
        friend typename const_iterator::ImplT;

    private:
        DimensionSpan<Dimension> _span; // local span
        DimensionSize<Dimension> _base_span;
        parent_iterator _ptr;  // start of block
};

class SliceTag {
    protected:
        SliceTag() {}
        ~SliceTag() {} // protected to prevent polymorphic use of non-virtual destructor
}; // allows is_slice to work. Has no other function

// specialisation for 0 dimensional slice
// which should return a single element
template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin, typename Dimension>
class Slice<is_const, SliceTraitsT, SliceMixin, Dimension> : public SliceTag
{
        typedef Slice<is_const, SliceTraitsT, SliceMixin, Dimension> SelfType;
        typedef Slice<Flip<is_const>::value, SliceTraitsT, SliceMixin, Dimension> FlipConstSelfType;
        friend FlipConstSelfType;
        friend SliceMixin<FlipConstSelfType>;

        typedef typename SliceTraitsHelper<SliceTraitsT>::Parent ParentT;
        typedef typename ParentT::const_iterator parent_const_iterator;
        typedef typename std::conditional<is_const, parent_const_iterator, typename ParentT::iterator>::type parent_iterator;
        typedef Dimension SelfDimension;

    protected:
        typedef typename SliceTraitsHelper<SliceTraitsT>::ExcludeTuple ExcludeTuple;

    public:
        typedef std::tuple<Dimension> DimensionTuple;

        template<typename Dim>
        struct OperatorSliceType;

        template<typename Dim>
        struct ConstOperatorSliceType;

        typedef typename std::iterator_traits<parent_iterator>::reference reference_type;
        typedef typename std::iterator_traits<parent_const_iterator>::reference const_reference_type;
        typedef typename std::conditional<is_const, const ParentT, ParentT>::type Parent;
        typedef Slice<is_const, SliceTraitsT, SliceMixin, Dimension> SliceType;
        typedef Slice<true, SliceTraitsT, SliceMixin, Dimension> ConstSliceType;
        typedef parent_iterator iterator;
        typedef parent_const_iterator const_iterator;

    public:
        explicit Slice();
        Slice(Parent& parent, DimensionSpan<Dimension> const&);
        Slice(Slice const&);

        template<bool is_const2, typename SliceTraitsT2, template<typename> class SliceMixin2, typename SliceDim, typename... SliceDimensions>
        Slice( Parent&, SliceMixin2<Slice<is_const2, SliceTraitsT2, SliceMixin2, SliceDim, SliceDimensions...>> const& slice);

        ~Slice();

        static constexpr std::size_t rank = 1;

        /**
         * @brief return true if this slice has dimesion D
         */
        template<typename D>
        static constexpr bool has_dimension() { return std::is_same<Dimension, D>::value; }

        /**
         * @brief copy assignment operator
         */
         Slice& operator=(Slice const&);

        /**
         * @brief the total number of data members in this slice
         */
        std::size_t data_size() const;

        /**
         * @brief return the size of the slice in the specified dimension
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
        size() const;

        /**
         * @brief return the size of the slice in the specified dimension (will always be zero)
         */
        template<typename Dim>
        constexpr
        typename std::enable_if<((!std::is_same<Dim, Dimension>::value) && (!astrotypes::has_dimension<Parent, Dim>::value))
                               , DimensionSize<Dim>>::type
        size() const;

        /**
         * @brief return the size of the slice in the specified dimension (will always be one)
         * @details case where the Dim is not represented explicitly by the Slice, but by the Parent
         */
        template<typename Dim>
        constexpr
        typename std::enable_if<(!std::is_same<Dim, Dimension>::value)
                               && astrotypes::has_dimension<Parent, Dim>::value
                               , DimensionSize<Dim>>::type
        size() const;

        /**
         * @ brief return the size of the slice in the specified dimension
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
        dimension() const;

        /**
         * @ brief return the size of the slice in the specified dimension (will always be zero)
         */
        template<typename Dim>
        constexpr
        typename std::enable_if<((!std::is_same<Dim, Dimension>::value) && (!astrotypes::has_dimension<Parent, Dim>::value))
                               , DimensionSize<Dim>>::type
        dimension() const;

        /**
         * @brief return the size of the slice in the specified dimension (will always be one)
         * @detials case where the Dim is not represented explicitly by the Slice, but by the Parent
         */
        template<typename Dim>
        constexpr
        typename std::enable_if<(!std::is_same<Dim, Dimension>::value)
                               && astrotypes::has_dimension<Parent, Dim>::value
                               , DimensionSize<Dim>>::type
        dimension() const;

        /**
         * @brief return the size of the slice in the specified span relative to the parent
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSpan<Dimension>>::type
        span() const;

        /**
         * @brief return the value at the position specified in this Dimension
         */
        reference_type operator[](std::size_t index) const;
        reference_type operator[](DimensionIndex<Dimension> const& index) const;

        // specialisations for implicit (i.e Parent has dimension but Slice does not)
        template<typename Dim, typename Enable=typename std::enable_if<astrotypes::has_dimension<Parent, Dim>::value
                                                            && !std::is_same<Dim, Dimension>::value>::type>
        SelfType const& operator[](DimensionIndex<Dim> const&) const { return *this; }

        // specialisations for implicit (i.e Parent has dimension but Slice does not)
        template<typename Dim, typename Enable=typename std::enable_if<astrotypes::has_dimension<Parent, Dim>::value
                                                            && !std::is_same<Dim, Dimension>::value>::type>
        SelfType& operator[](DimensionIndex<Dim> const&) { return *this; }

        /**
         * @brief return a sub window on the slice
         */
        ConstSliceType slice(DimensionSpan<Dimension> const& span) const;
        SliceType slice(DimensionSpan<Dimension> const& span);

        /**
         * @brief iterator pointing to the first element in the slice
         */
        parent_iterator begin();
        parent_const_iterator begin() const;
        parent_const_iterator cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        parent_iterator end();
        parent_const_iterator end() const;
        parent_const_iterator cend() const;

        /**
         * @brief compare tow arrays
         */
        template<bool is_const_>
        bool operator==(Slice<is_const_, SliceTraitsT, SliceMixin, Dimension> const&) const;

        /**
         * @brief return the parent object of which the slice is based on
         */
        Parent& parent() const;

    protected:
        template<bool, typename P, template<typename> class, typename D, typename... Ds> friend class Slice;

        template<typename IteratorT> bool increment_it(IteratorT& current, SlicePosition<rank>& pos) const;
        template<typename IteratorDifferenceT> static IteratorDifferenceT diff_it(IteratorDifferenceT const& diff);


        // same as size() - to support base_span calls from higher dimensions
        std::size_t base_span() const;

        // number of elements between the blocks end and its beginning
        // 1 Base_ptr less than base_span
        std::size_t diff_base_span() const;

        // ptr to the start of the block
        parent_iterator const& base_ptr() const;
        parent_iterator& base_ptr();

        parent_iterator const& offset(parent_iterator const&); // init the offset relative to the top parent

        SelfType& operator+=(DimensionSize<Dimension> const&);
        SelfType& operator+=(std::size_t n);

        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value , DimensionSpan<Dim>>::type
        parent_span() const;

        /**
         * @ brief return the span of the slice in the specified dimension relative to the parent
         * @details  specialisation for a slice that
         *           does not have this Dimension, but come from a parent class that does
         */
        template<typename Dim>
        typename std::enable_if<((!std::is_same<Dim, Dimension>::value) && (astrotypes::has_dimension<Parent, Dim>::value))
                               , DimensionSpan<Dim>>::type
        parent_span() const;

    protected:
        template<typename... Dims>
        Slice( internal_construct_tag const&
             , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Parent&>::type parent
             , DimensionSpan<Dims> const&... spans);

        template<typename... Dims>
        Slice( internal_construct_tag const&
             , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Parent&>::type parent
             , DimensionSpan<Dims> const&... spans);

        template<bool is_const2, typename SliceTraitsT2, template<typename> class SliceMixin2, typename SliceDim, typename... Dimensions2>
        Slice( internal_construct_tag const&
             , Parent&
             , Slice<is_const2, SliceTraitsT2, SliceMixin2, SliceDim, Dimensions2...> const& slice);

        template<typename... Dims>
        Slice( typename std::enable_if<!arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
             , Slice const& copy
             , DimensionSpan<Dims> const&... spans );

        template<typename... Dims>
        Slice( typename std::enable_if<arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
             , Slice const& copy
             , DimensionSpan<Dims> const&... spans );

    protected:
        template<typename SliceType, typename IteratorType>
        struct OffsetJob;

    private:
        DimensionSpan<Dimension> _span;
        DimensionSize<Dimension> _base_span;
        Parent* _parent;
        parent_iterator _ptr;   // start, to extent _ptr + _span
};

} // namespace multiarray
} // namespace astrotypes
} // namespace pss

#include "detail/Slice.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICE_H
