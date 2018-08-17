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
    { return BaseT::arg(std::forward<T>(args)...); }
};

/**
 * @brief
 *      Representation of a Slice through a Data Array
 * @details
 */
struct copy_resize_construct_tag {};
struct copy_resize_construct_base_tag {};

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
class Slice : private Slice<is_const, ParentT, Dimensions...>
{
        typedef Slice<is_const, ParentT, Dimensions...> BaseT;
        typedef Slice<is_const, ParentT, Dimension, Dimensions...> SelfType;
        typedef typename ParentT::value_type value_type;
        typedef Dimension SelfDimension;

        typedef typename ParentT::const_iterator parent_const_iterator;
        typedef typename std::conditional<is_const, parent_const_iterator, typename ParentT::iterator>::type parent_iterator;

    public:
        typedef typename std::conditional<is_const, const ParentT, ParentT>::type Parent;
        typedef SliceIterator<SelfType, is_const> iterator;
        typedef SliceIterator<SelfType, true> const_iterator;
        typedef Slice<is_const, ParentT, Dimension, Dimensions...> SliceType;
        typedef Slice<true, ParentT, Dimension, Dimensions...> ConstSliceType;

    public:
        Slice( Parent& parent
             , DimensionSpan<Dimension> const&
             , DimensionSpan<Dimensions> const&...
        );

        /// constructor that will use the parents dimensions as default if not specified
        template<typename... Dims, typename Enable=std::enable_if<!arg_helper<Dimension, Dims...>::value>>
        Slice( Parent& parent
             , DimensionSpan<Dims> const& ...);

        static constexpr std::size_t rank = 1 + sizeof...(Dimensions);

        /**
         * @brief the total number of data memebers in this slice
         */
        std::size_t data_size() const;

        /**
         * @ brief return the size of the slice in the specified dimension
         * @code
         *      Slice<DimesionA, DimensionB> slice(...some initialisers...);
         *      DimensionSize<DimensionA> size_of_A = slice.size<DimensionA>();
         *      DimensionSize<DimensionB> size_of_B = slice.size<DimensionB>();
         *      // note you can rmeomve the dimension tagging on the DimesionSize
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

        /**
         * @brief Take a slice in the specified dimension one channel thick
         */
        Slice<is_const, ParentT, Dimensions...> operator[](DimensionIndex<Dimension>) const;

        template<typename Dim>
        typename std::enable_if<arg_helper<Dim, Dimensions...>::value
                            && !std::is_same<Dim, Dimension>::value, Slice<is_const, ParentT, Dimension, Dimensions...>>::type
        operator[](DimensionIndex<Dim> const&);

        template<typename Dim>
        typename std::enable_if<arg_helper<Dim, Dimensions...>::value
                            && !std::is_same<Dim, Dimension>::value, ConstSliceType>::type
        operator[](DimensionIndex<Dim> const&) const;

        /**
         * @brief Take a sub-slice from this slice
         * @params ipass a DimensionSpan<Dimension> object for each dimension
         *         you wish to restrict.
         *         Span indexes are relative to Slice boundary, not hee parent
         */
        template<typename... Dims>
        typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type
        slice(DimensionSpan<Dims>&&... spans);

        template<typename... Dims>
        typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type
        slice(DimensionSpan<Dims>&&... spans);

        template<typename... Dims>
        typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice<true, ParentT, Dimension, Dimensions...>>::type
        slice(DimensionSpan<Dims>&&... spans) const;

        template<typename... Dims>
        typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice<true, ParentT, Dimension, Dimensions...>>::type
        slice(DimensionSpan<Dims>&&... spans) const;


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
         * @brief compare two arrays
         */
        template<bool const_type>
        bool operator==(Slice<const_type, ParentT, Dimension, Dimensions...> const&) const;

    protected:
        template<bool, typename P, typename D, typename... Ds> friend class Slice;

        template<typename IteratorT> bool increment_it(IteratorT& current, SlicePosition<rank>& pos) const;
        template<typename IteratorDifferenceT> IteratorDifferenceT diff_it(IteratorDifferenceT const& diff) const;

        // return the size of memory occupied by the lowest dimension
        std::size_t contiguous_span() const;

        // reeturn the span of all lower dimensions than this one (i.e an index of +1 in this dimension)
        std::size_t base_span() const;
        std::size_t diff_base_span() const;

        // ptr to the start of the block
        parent_iterator const& base_ptr() const;
        parent_iterator& base_ptr();

        parent_iterator const& offset(parent_iterator const&); // init the offset relative to the top parent

        // init for inheriting classes only
        Slice( DimensionSpan<Dimension> const&
             , DimensionSpan<Dimensions> const&...
             , Parent const& parent
        );

        // init for constructing slices in other Dimensions
        Slice( DimensionSpan<Dimensions> const&...
             , Parent const& parent
        );

        template<typename... Dims>
        Slice( copy_resize_construct_tag const&
             , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims>&&... spans );

        template<typename... Dims>
        Slice( copy_resize_construct_base_tag const&
             , typename std::enable_if<!arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims>&&... spans );

        template<typename... Dims>
        Slice( copy_resize_construct_tag const&
             , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims>&&... spans );

        template<typename... Dims>
        Slice( copy_resize_construct_base_tag const&
             , typename std::enable_if<arg_helper<Dimension, Dims...>::value, Slice>::type const& copy
             , DimensionSpan<Dims>&&... spans );

        // increment pointer by a n * span length
        SelfType& operator+=(std::size_t n);

        // increment pointer by a n * base span length
        SelfType& operator+=(DimensionSize<Dimension> n);

    private:
        friend typename iterator::BaseT;
        friend typename iterator::ImplT;
        friend typename const_iterator::BaseT;
        friend typename const_iterator::ImplT;

    private:
        DimensionSpan<Dimension> _span;
        DimensionSize<Dimension> _base_span;
        parent_iterator _ptr;  // start of block
};

// specialisation for 0 dimensional slice
// which should return a single element
template<bool is_const, typename ParentT, typename Dimension>
class Slice<is_const, ParentT, Dimension>
{
        typedef Slice<is_const, ParentT, Dimension> SelfType;
        typedef typename ParentT::const_iterator parent_const_iterator;
        typedef typename std::conditional<is_const, parent_const_iterator, typename ParentT::iterator>::type parent_iterator;
        typedef typename std::iterator_traits<parent_iterator>::reference reference_type;
        typedef Dimension SelfDimension;

    public:
        typedef typename std::conditional<is_const, const ParentT, ParentT>::type Parent;
        typedef Slice<is_const, ParentT, Dimension> SliceType;
        typedef Slice<true, ParentT, Dimension> ConstSliceType;
        typedef parent_iterator iterator;
        typedef parent_const_iterator const_iterator;

    public:
        Slice(Parent& parent, DimensionSpan<Dimension> const&);
        ~Slice();

        static constexpr std::size_t rank = 1;

        /**
         * @brief the total number of data memebers in this slice
         */
        std::size_t data_size() const;

        /**
         * @ brief return the size of the slice in the specified dimension
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type
        size() const;

        /**
         * @ brief return the size of the slice in the specified dimension (will always be zero)
         */
        template<typename Dim>
        static constexpr
        typename std::enable_if<((!std::is_same<Dim, Dimension>::value) && (!has_dimension<ParentT, Dim>::value))
                               , DimensionSize<Dim>>::type
        size();

        /**
         * @brief return the size of the slice in the specified dimension (will always be one)
         * @detials case where the Dim is not represented ecxplicitly by the Slice, but by the Parent
         */
        template<typename Dim>
        static constexpr
        typename std::enable_if<(!std::is_same<Dim, Dimension>::value)
                               && has_dimension<ParentT, Dim>::value
                               , DimensionSize<Dim>>::type
        size();

        /**
         * @brief return the value at the position specified in this Dimension
         */
        reference_type operator[](std::size_t index) const;
        reference_type operator[](DimensionIndex<Dimension> const& index) const;

        // specialisations for implicit (i.e Parent has dimension but Slice does not)
        template<typename Dim, typename Enable=typename std::enable_if<has_dimension<Parent, Dim>::value
                                                            && !std::is_same<Dim, Dimension>::value>::type>
        SelfType const& operator[](DimensionIndex<Dim> const&) const { return *this; }

        // specialisations for implicit (i.e Parent has dimension but Slice does not)
        template<typename Dim, typename Enable=typename std::enable_if<has_dimension<Parent, Dim>::value
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
        bool operator==(Slice<is_const_, ParentT, Dimension> const&) const;

    protected:
        template<bool, typename P, typename D, typename... Ds> friend class Slice;

        template<typename IteratorT> bool increment_it(IteratorT& current, SlicePosition<rank>& pos) const;
        template<typename IteratorDifferenceT> static IteratorDifferenceT diff_it(IteratorDifferenceT const& diff);

        // return the size of memory occupied by the lowest dimension
        std::size_t contiguous_span() const;

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

    protected:
        Slice(DimensionSpan<Dimension> const&, Parent const&);

        template<typename... Dims>
        Slice( typename std::enable_if<!arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
             , Slice const& copy
             , DimensionSpan<Dims>&&... spans );

        template<typename... Dims>
        Slice( typename std::enable_if<arg_helper<Dimension, Dims...>::value, copy_resize_construct_base_tag const&>::type
             , Slice const& copy
             , DimensionSpan<Dims>&&... spans );

    private:
        DimensionSpan<Dimension> _span;
        DimensionSize<Dimension> _base_span;
        parent_iterator _ptr;   // start, to extent _ptr + _span
};

} // namespace astrotypes
} // namespace pss

#include "detail/Slice.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICE_H
