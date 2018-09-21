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

        typedef SliceMixin<Slice<false, SelfType, SliceMixin, FirstDimension, OtherDimensions...>> SliceType;
        typedef SliceMixin<Slice<true, SelfType, SliceMixin, FirstDimension, OtherDimensions...>> ConstSliceType;
        typedef typename SliceType::template OperatorSliceType<FirstDimension>::type ReducedDimensionSliceType;
        typedef typename ConstSliceType::template ConstOperatorSliceType<FirstDimension>::type ConstReducedDimensionSliceType;

        typedef T& reference_type;
        typedef T const& const_reference_type;

    public:
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

        /**
         * @brief iterators acting over he entire data structure
         * @details useful for e.g std::copy of the entire structure
         *          without inspection
         */
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

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
        typename std::enable_if<has_dimension<MultiArray, Dim>::value
                            && !std::is_same<Dim, FirstDimension>::value
                            , typename OperatorSliceType<Dim>::type>::type
        operator[](DimensionIndex<Dim> const&);

        template<typename Dim>
        typename std::enable_if<has_dimension<MultiArray, Dim>::value
                            && !std::is_same<Dim, FirstDimension>::value
                            , typename ConstOperatorSliceType<Dim>::type>::type
        operator[](DimensionIndex<Dim> const&) const;

        /**
         * @brief return a slice of the specified dimension spanning the index_range provided
         */
        template<typename Dim, typename... Dims>
        SliceType slice(DimensionSpan<Dim>&& range, DimensionSpan<Dims>&&...);

        template<typename Dim, typename... Dims>
        ConstSliceType slice(DimensionSpan<Dim>&& range, DimensionSpan<Dims>&&...) const;

        /**
         * @brief resize the array in the specified dimension
         */
        template<typename... Dimensions>
        void resize(DimensionSize<Dimensions>... size);

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

    protected:
        template<typename... Dims>
        MultiArray(typename std::enable_if<arg_helper<FirstDimension, Dims...>::value, bool>::type disable_resize_tag
                  , DimensionSize<Dims> const&...);

        template<typename DimensionType>
        MultiArray(typename std::enable_if<has_dimensions<DimensionType, FirstDimension, OtherDimensions...>::value, bool>::type disable_transpose_tag
                  , DimensionType const& d);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, void>::type
        do_resize(std::size_t total, DimensionSize<Dimension> size, DimensionSize<Dims>&&... sizes);

        template<typename Dimension, typename... Dims>
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, void>::type
        do_resize(std::size_t total, DimensionSize<Dimension> size, DimensionSize<Dims>&&... sizes);

        void do_resize(std::size_t total);

        template<typename SelfSlice, typename OtherSlice>
        void do_transpose(SelfSlice&, OtherSlice const&);

    private:
        DimensionSize<FirstDimension>     _size;
};

// specialisation for single dimension arrays
template<typename Alloc, typename T, template<typename> class SliceMixin, typename FirstDimension>
class MultiArray<Alloc, T, SliceMixin, FirstDimension>
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

        typedef SliceMixin<Slice<false, SelfType, SliceMixin, FirstDimension>> SliceType;
        typedef SliceMixin<Slice<true, SelfType, SliceMixin, FirstDimension>> ConstSliceType;
        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;

        typedef T& reference_type;
        typedef T const& const_reference_type;
        typedef T value_type;

    public:
        template<typename Dim, typename... Dims>
        MultiArray(DimensionSize<Dim> const& size, DimensionSize<Dims> const&... sizes);
        explicit MultiArray(MultiArray const&) = default;
        ~MultiArray();


        /**
         * @brief
         */
        reference_type operator[](DimensionIndex<FirstDimension> index);
        const_reference_type operator[](DimensionIndex<FirstDimension> index) const;

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

        template<typename Dimension>
        void resize(DimensionSize<Dimension> size);

        /**
         * @brief compare data in the two arrays
         */
        bool operator==(MultiArray const&) const;

        /**
         * @brief return true if the sizes of each dimension are equivalent
         */
        bool equal_size(MultiArray const&) const;

        /// bulk iterators
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

    protected:
        template<typename... Dims>
        MultiArray(typename std::enable_if<arg_helper<FirstDimension, Dims...>::value, bool>::type disable_resize_tag
                  , DimensionSize<Dims> const&...);

        template<typename DimensionType>
        MultiArray(typename std::enable_if<has_dimension<DimensionType, FirstDimension>::value, bool>::type disable_transpose_tag
                  , DimensionType const& d);

        /// resize
        template<typename Dimension>
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, void>::type
        do_resize(std::size_t total_size, DimensionSize<Dimension> size);

        void do_resize(std::size_t total_size);

        template<typename SelfSlice, typename OtherSlice>
        void do_transpose(SelfSlice&, OtherSlice const&);

    private:
        DimensionSize<FirstDimension> _size;
        std::vector<T> _data;
};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename Dimension>
struct has_dimension<MultiArray<Alloc, T, SliceMixin, Dimension>, Dimension> : public std::true_type
{};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename Dimension>
struct has_exact_dimensions<MultiArray<Alloc, T, SliceMixin, Dimension>, Dimension> : public std::true_type
{};

template<typename Alloc, typename T, template<typename> class SliceMixin, typename Dimension1, typename Dimension2, typename... Dimensions>
struct has_exact_dimensions<MultiArray<Alloc, T, SliceMixin, Dimension1, Dimension2, Dimensions...>, Dimension1, Dimension2, Dimensions...> : public std::true_type
{};

} // namespace astrotypes
} // namespace pss

#include "detail/MultiArray.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_H
