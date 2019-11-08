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
#ifndef PSS_ASTROTYPES_MULTIARRAY_H
#define PSS_ASTROTYPES_MULTIARRAY_H

#include "Slice.h"
#include "DimensionSize.h"
#include "DimensionIndex.h"
#include <vector>

namespace pss {
namespace astrotypes {

/**
 * @brief struct ot help determine if a specified type is a multiarray
 * @details
 * @code
 * typename std::enable_if<is_multiarray<SomeType>::value>::type
 * @endcode
 */
template<typename T>
struct is_multiarray;

namespace multiarray {

/**
 * @brief
 *      template classes to specify multiple dimension arrays with explicit dimension types
 * @tparam T the type of object to be stored in the array (e.g. float, int, Stokes, etc)
 *
 */

template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension, typename... OtherDimensions>
class MultiArray : MultiArray<Alloc, T, SliceMixin, OtherDimensions...>
{
        typedef MultiArray<Alloc, T, SliceMixin, OtherDimensions...> BaseT;
        typedef MultiArray<Alloc, T, SliceMixin, FirstDimension, OtherDimensions...> SelfType;
        typedef std::vector<T, Alloc> Container;

    public:
        typedef typename BaseT::iterator iterator;
        typedef typename BaseT::const_iterator const_iterator;
        typedef typename BaseT::value_type value_type;

    public:
        /**
          * @brief provides a template to determine the returned type of an operator[]
          *  @tparam Dim  the dimension that will be called
          *  @param type The type that will we returned by the operator[DimensionIndex<Dim>]
          */
        template<typename Dim>
        struct OperatorSliceType;

        /**
          * @brief provides a template to determine the returned type of an operator[] const
          *  @tparam Dim  the dimension that will be called
          *  @param type The type that will we returned by the operator[DimensionIndex<Dim>]
          */
        template<typename Dim>
        struct ConstOperatorSliceType;

        /**
          * @brief provides a template to determine the returned type of overlay slices
          *  @tparam SliceInputType the type of slice used to generate the overlay slice
          *  @param type The type that will we returned by the overlap(T) method.
          */
        template<typename SliceInputType>
        struct SliceReturnType;
        template<typename SliceInputType>
        struct ConstSliceReturnType;

        typedef SliceMixin<Slice<false, SelfType, SliceMixin, FirstDimension, OtherDimensions...>> SliceType;
        typedef SliceMixin<Slice<true, SelfType, SliceMixin, FirstDimension, OtherDimensions...>> ConstSliceType;
        typedef typename SliceType::template OperatorSliceType<FirstDimension>::type ReducedDimensionSliceType;
        typedef typename ConstSliceType::template ConstOperatorSliceType<FirstDimension>::type ConstReducedDimensionSliceType;

        typedef T& reference_type;
        typedef T const& const_reference_type;

    public:
        MultiArray();

        template<typename Dim, typename... Dims>
        MultiArray(DimensionSize<Dim> size, DimensionSize<Dims>... sizes);

        /// copy operator needs to be called explicitly as this is an expensive operation
        explicit MultiArray(MultiArray const&) = default;

        /// @brief initialise and transpose from a group with different memory ordering
        //  @details will copy the memory from the object tranposing it in the process
        //  to the required memory order
        template<typename DimensionType, typename Enable=typename std::enable_if<
                   has_dimensions<DimensionType, FirstDimension, OtherDimensions...>::value
                && !has_exact_dimensions<FirstDimension, OtherDimensions...>::value>::type>
        explicit MultiArray(DimensionType const&);

        static constexpr std::size_t rank = 1 + sizeof...(OtherDimensions);

        /**
         * @brief return true if this slice has dimesion D
         */
        template<typename D>
        static constexpr bool has_dimension() { return has_type<std::tuple<FirstDimension, OtherDimensions...>, D>::value; }

        /**
         * @brief iterators acting over he entire data structure
         * @details useful for e.g std::copy of the entire structure
         *          without inspection
         */
        inline iterator begin();
        inline iterator end();
        inline const_iterator begin() const;
        inline const_iterator end() const;
        inline const_iterator cbegin() const;
        inline const_iterator cend() const;

        /**
         * @brief take a slice of width 1 data at the specified index of the first dimension
         * @details
         *    This can be used to access elements in a similar way to c style arrays.
         *
         * @code
         * MultiArray<int, DimA, DimB> my_multi_array(...);
         * for(DimensionIndex<DimA> i(0); i < my_multi_array.size<DimA>(); ++i)
         * {
         *     for(DimensionIndex<DimB> j(0); j < my_multi_array.size<DimB>(); ++j)
         *     {
         *          my_multi_array[i][j] = 10;
         *     }
         * }
         * @endcode
         */
        ReducedDimensionSliceType operator[](DimensionIndex<FirstDimension> index);
        ConstReducedDimensionSliceType operator[](DimensionIndex<FirstDimension> index) const;

        template<typename Dim>
        typename std::enable_if<astrotypes::has_dimension<MultiArray, Dim>::value
                            && !std::is_same<Dim, FirstDimension>::value
                            , typename OperatorSliceType<Dim>::type>::type
        operator[](DimensionIndex<Dim> const&);

        template<typename Dim>
        typename std::enable_if<astrotypes::has_dimension<MultiArray, Dim>::value
                            && !std::is_same<Dim, FirstDimension>::value
                            , typename ConstOperatorSliceType<Dim>::type>::type
        operator[](DimensionIndex<Dim> const&) const;

        /**
         * @brief return a slice of the specified dimension spanning the index_range provided
         * @details no bonuds checking is performed. Undefined behaviour can result from requests
         *          for a slice outside the limits.
         *          An example to test bounds using the @method trim method:
         *          @code
         *              DimensionSize<A> a_size(10);
         *              DimensionSize<B> b_size(100);
         *              MultiArray<int, A, B> ma(a_size, b_size);
         *
         *              // this span starts inside the array but extends beyond the bounds
         *              DimensionSpan<A> span(DimensionIndex<A>(8), DimensionSize<A>(10));
         *
         *              // adjust the span so that it fits in bounds
         *              span.trim(ma.template dimension<A>());
         *              if(span.span() > DimensionSize<A>(0)) {
         *                  // the span is valid and adjusted to fit.
         *              }
         *          @endocde
         */
        template<typename Dim, typename... Dims>
        SliceType slice(DimensionSpan<Dim> const& range, DimensionSpan<Dims> const&...);

        template<typename Dim, typename... Dims>
        ConstSliceType slice(DimensionSpan<Dim> const& range, DimensionSpan<Dims> const&...) const;

        /**
         * @brief overlay the Slice from another data structure creqting the equivalent slice for this structure
         * @detials specialisation where Dimensions match that of the slice.
         */
        template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2>
        SliceType overlay(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, FirstDimension, OtherDimensions...>> const&);

        template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2>
        SliceType overlay(Slice<is_const, SliceTraitsT, SliceMixin2, FirstDimension, OtherDimensions...> const&);

        template<bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2>
        ConstSliceType overlay(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, FirstDimension, OtherDimensions...>> const&) const;

        template<typename SliceArgType>
        typename SliceReturnType<SliceArgType>::type overlay(SliceArgType const& slice);

        template<typename SliceArgType>
        typename ConstSliceReturnType<SliceArgType>::type overlay(SliceArgType const& slice) const;

        /**
         * @brief The offset position of the beginning of the provided slice
         */
        template<typename Dimension, typename SliceArgumentType>
        typename std::enable_if<is_slice<SliceArgumentType>::value, DimensionIndex<Dimension>>::type
        offset(SliceArgumentType const&) const;

        /**
         * @brief The offset position of the beginning of the provided slice - mixin specialisation
         */
        template<typename Dimension, bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2, typename SliceDim, typename... SliceDimensions>
        DimensionIndex<Dimension> offset(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, SliceDim, SliceDimensions...>> const& slice) const;

        /**
         * @brief resize the array in the specified dimension
         */
        template<typename... Dimensions>
        void resize(DimensionSize<Dimensions>... size);

        /**
         * @brief resize the array in the specified dimension, setting any new elements to the specified value
         * @details automatic deduction of Dimensions can be problematic so ensure you specifiy them explcitly
         *          e.g. my_array.template resize<DimensionA, DimensionB>( size_a, size_b, value);
         */
        template<typename Dim, typename... Dimensions>
        void resize(DimensionSize<Dim>, DimensionSize<Dimensions>... size, T const& value);

        /**
         * @brief resize the array in the specified dimension
         *      @code
         *      multi_array.template size<DimensionA>();
         *      @endcode
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
        size() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
        size() const;

        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
        dimension() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
        dimension() const;

        /**
         * @brief the total size of data in all dimensions
         */
        std::size_t data_size() const;

        /**
         * @brief compare data in the two arrays
         */
        bool operator==(MultiArray const&) const;

        /**
         * @brief return true if the sizes of each dimension are equivalent
         */
        bool equal_size(MultiArray const&) const;

        /**
         * @brief the size of a block (span) at the specified Dimension
         */
        template<typename Dimension>
        typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
        block_size_t() const;

        /**
         * @brief the size of a block (span) at the specified Dimension
         */
        template<typename Dimension>
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
        block_size_t() const;

        /**
         * @brief iterate over each dimension layer of the structure and execute a callback on Job for each layer
         * @details Job should have a templated method on dimension for the callback:
         * @code
         * template<typename Dimension> void exec(MultiArray<...Dimension> const& multi_array_layer);
         * @endcode
         * e.g
         * @code
         *
         * struct Job {
         *    static inline void
         *    template<typename Dimension, typnename DataStruct>
         *    void exec(DataStruct const& array) {
         *        std::cout << array.dimension<Dimension>() << "\n";
         *    }
         * };
         *
         * Job job;
         * my_data.for_each_dimension(jon);
         *
         * @endcode
         */
        template<typename Job>
        void for_each_dimension(Job& job) const;

    protected:

        template<typename... Dims>
        MultiArray(typename std::enable_if<arg_helper<FirstDimension, Dims...>::value, bool>::type disable_resize_tag
                  , DimensionSize<Dims> const&...);

        template<typename DimensionType>
        MultiArray(typename std::enable_if<has_dimensions<DimensionType, FirstDimension, OtherDimensions...>::value, bool>::type disable_transpose_tag
                  , DimensionType const& d);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<!arg_helper<FirstDimension, Dimension, Dims...>::value, void>::type
        do_resize(std::size_t total, DimensionSize<Dimension> size, DimensionSize<Dims>... sizes);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<!arg_helper<FirstDimension, Dimension, Dims...>::value, void>::type
        do_resize(std::size_t total, DimensionSize<Dimension> size, DimensionSize<Dims>... sizes, T const& value);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<arg_helper<FirstDimension, Dimension, Dims...>::value, void>::type
        do_resize(std::size_t total, DimensionSize<Dimension> size, DimensionSize<Dims>... sizes);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<arg_helper<FirstDimension, Dimension, Dims...>::value, void>::type
        do_resize(std::size_t total, DimensionSize<Dimension> size, DimensionSize<Dims>... sizes, T const& value);

        void do_resize(std::size_t total);
        void do_resize(std::size_t total, T const& value);

        template<typename SelfSlice, typename OtherSlice>
        void do_transpose(SelfSlice&, OtherSlice const&);

        template<typename Dimension, typename SliceArgumentType>
        typename std::enable_if<has_dimension_strict<SliceArgumentType, Dimension>::value
                               && is_slice<SliceArgumentType>::value
                               , DimensionIndex<Dimension>>::type
        do_offset(SliceArgumentType const& slice) const;

        template<typename Dimension, typename SliceArgumentType>
        typename std::enable_if<!has_dimension_strict<SliceArgumentType, Dimension>::value
                               && is_slice<SliceArgumentType>::value
                               , DimensionIndex<Dimension>>::type
        do_offset(SliceArgumentType const& slice) const;


        template<typename Dimension>
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, DimensionIndex<Dimension>>::type
        calculate_offset(std::size_t delta) const;

        template<typename Dimension>
        typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, DimensionIndex<Dimension>>::type
        calculate_offset(std::size_t delta) const;

        /**
         * @brief the size of a complete block (span) at dimension FirstDimension
         */
        std::size_t block_size() const;

    private:
        DimensionSize<FirstDimension>     _size;
};

class MultiArrayTag {}; // allows is_multiarray to work. Has no other function

// specialisation for single dimension arrays
template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
class MultiArray<Alloc, T, SliceMixin, FirstDimension> : public MultiArrayTag
{
        typedef MultiArray<Alloc, T, SliceMixin, FirstDimension> SelfType;
        typedef std::vector<T, Alloc> Container;

    public:
        /**
          * @brief provides a template to determine the returned type of an operator[]
          *  @tparam Dim  the dimension that will be called
          *  @param type The type that will we returned by the operator[DimensionIndex<Dim>]
          */
        template<typename Dim>
        struct OperatorSliceType;

        /**
          * @brief provides a template to determine the returned type of an operator[] const
          *  @tparam Dim  the dimension that will be called
          *  @param type The type that will we returned by the operator[DimensionIndex<Dim>]
          */
        template<typename Dim>
        struct ConstOperatorSliceType;

        /**
          * @brief provides a template to determine the returned type of overlay slices
          *  @tparam SliceInputType the type of slice used to generate the overlay slice
          *  @param type The type that will we returned by the overlap(T) method.
          */
        typedef SliceMixin<Slice<false, SelfType, SliceMixin, FirstDimension>> SliceType;
        typedef SliceMixin<Slice<true, SelfType, SliceMixin, FirstDimension>> ConstSliceType;
        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;

        typedef T& reference_type;
        typedef T const& const_reference_type;
        typedef T value_type;

    public:
        MultiArray();

        template<typename Dim, typename... Dims>
        MultiArray(DimensionSize<Dim> const& size, DimensionSize<Dims> const&... sizes);
        explicit MultiArray(MultiArray const&) = default;
        ~MultiArray();

        static constexpr std::size_t rank = 1;

        /**
         * @brief return true if this array has dimesion D
         */
        template<typename D>
        static constexpr bool has_dimension() { return std::is_same<FirstDimension, D>::value; }

        /**
         * @brief
         */
        reference_type operator[](DimensionIndex<FirstDimension> index);
        const_reference_type operator[](DimensionIndex<FirstDimension> index) const;

        /**
         * @brief The offset position of the beginning of the provided slice
         */
        template<typename Dimension, bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2, typename SliceDim, typename... SliceDimensions>
        DimensionIndex<Dimension> offset(Slice<is_const, SliceTraitsT, SliceMixin2, SliceDim, SliceDimensions...> const& slice) const;

        template<typename Dimension, bool is_const, typename SliceTraitsT, template<typename> class SliceMixin2, typename SliceDim, typename... SliceDimensions>
        DimensionIndex<Dimension> offset(SliceMixin2<Slice<is_const, SliceTraitsT, SliceMixin2, SliceDim, SliceDimensions...>> const& slice) const;

        /// size
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
        size() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
        size() const;

        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type
        dimension() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type
        dimension() const;

        /**
         * @brief the total size of data in all dimensions
         */
        std::size_t data_size() const;

        /**
         * @brief resize in the specified dimension
         */
        template<typename Dimension>
        void resize(DimensionSize<Dimension> size);

        /**
         * @brief resize in the specified dimension, setting any new elements to value
         */
        template<typename Dimension>
        void resize(DimensionSize<Dimension> size, T const& value);

        /**
         * @brief compare data in the two arrays
         */
        bool operator==(MultiArray const&) const;

        /**
         * @brief return true if the sizes of each dimension are equivalent
         */
        bool equal_size(MultiArray const&) const;

        /// bulk iterators
        inline iterator begin();
        inline const_iterator begin() const;
        inline const_iterator cbegin() const;
        inline iterator end();
        inline const_iterator end() const;
        inline const_iterator cend() const;

        /**
         * @brief the size of a block (span) at the specified Dimension
         */
        template<typename Dimension>
        constexpr
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
        block_size_t() const;

        /**
         * @brief specialisation to give an error message.
         * @details  you should be able to eliminate this call using template magic
         */
        template<typename Dimension>
        typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, std::size_t>::type
        block_size_t() const;

        template<typename Job>
        void for_each_dimension(Job& job) const;

    protected:
        template<typename... Dims>
        MultiArray(typename std::enable_if<arg_helper<FirstDimension, Dims...>::value, bool>::type disable_resize_tag
                  , DimensionSize<Dims> const&...);

        template<typename DimensionType>
        MultiArray(typename std::enable_if<astrotypes::has_dimension<DimensionType, FirstDimension>::value, bool>::type disable_transpose_tag
                  , DimensionType const& d);

        /// resize
        template<typename Dimension, typename... Dims>
        typename std::enable_if<arg_helper<FirstDimension, Dimension, Dims...>::value, void>::type
        do_resize(std::size_t total_size, DimensionSize<Dimension> size, DimensionSize<Dims>...);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<arg_helper<FirstDimension, Dimension, Dims...>::value, void>::type
        do_resize(std::size_t total_size, DimensionSize<Dimension> size, DimensionSize<Dims>..., T const& value);

        template<typename... Dims>
        typename std::enable_if<!arg_helper<FirstDimension, Dims...>::value, void>::type
        do_resize(std::size_t total_size, DimensionSize<Dims>...);

        template<typename... Dims>
        typename std::enable_if<!arg_helper<FirstDimension, Dims...>::value, void>::type
        do_resize(std::size_t total_size, DimensionSize<Dims>..., T const& value);

        template<typename SelfSlice, typename OtherSlice>
        void do_transpose(SelfSlice&, OtherSlice const&);

        template<typename Dimension>
        DimensionIndex<Dimension> calculate_offset(std::size_t delta) const;

        std::size_t block_size() const;

    private:
        DimensionSize<FirstDimension> _size;
        std::vector<T> _data;
};

} // namespace multiarray
} // namespace astrotypes
} // namespace pss

#include "detail/MultiArray.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_H
