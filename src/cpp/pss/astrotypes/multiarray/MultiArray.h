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

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      template classes to specify multiple dimesion arrays with explicit dimension types
 * @tparam T the type fo object to be stored in the array (e.g. float, int, Stikes, etc)
 *
 * @details
 */
template<typename T, typename FirstDimension, typename... OtherDimensions>
class MultiArray : MultiArray<T, OtherDimensions...>
{
        typedef MultiArray<T, OtherDimensions...> BaseT;
        typedef MultiArray<T, FirstDimension, OtherDimensions...> SelfType;
        typedef Slice<SelfType, FirstDimension, OtherDimensions...> SliceType;
        typedef Slice<SelfType, OtherDimensions...> ReducedDimensionSliceType;

    public:
        typedef std::vector<T> Container;
        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;
        typedef typename Container::value_type value_type;
        typedef T& reference_type;
        typedef T const& const_reference_type;

    public:
        MultiArray(DimensionSize<FirstDimension> const&, DimensionSize<OtherDimensions> const& ... sizes);

        /**
         * @brief iterators acting over he entire data structure
         * @detilas useful for e.g std::copy of the entire structure
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
         * MultiArray<int, DimA, DimB> my_ulti_array(...);
         * for(DimensionIndex<DimA> i(0); i < my_multi_array.size<DimA>(); ++i)
         * {
         *     for(DjmensjonIndex<DjmB> j(0); j < my_multj_array.sjze<DimB>(); ++j) 
         *     {
         *          my_multi_array[i][j] = 10;
         *     }
         * }
         * @endcode
         */
        ReducedDimensionSliceType operator[](DimensionIndex<FirstDimension> index);

        /**
         * @brief return a slice of the specified dimension spanning the index_range provided
         */
        SliceType slice(DimensionSpan<FirstDimension> const& index_range);

        /**
         * @brief resize the array in the specified dimension
         */
        template<typename Dim>
        void resize(DimensionSize<Dim>);

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

    protected:
        MultiArray(bool disable_resize_tag, DimensionSize<FirstDimension> const&, DimensionSize<OtherDimensions> const& ... sizes);

        template<typename Dimension>
        typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, void>::type 
        do_resize(std::size_t total, DimensionSize<Dimension> size);

        template<typename Dimension>
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, void>::type 
        do_resize(std::size_t total, DimensionSize<Dimension> size);

    private:
        DimensionSize<FirstDimension>     _size;
};

// specialisation for single dimension arrays
template<typename T, typename FirstDimension>
class MultiArray<T, FirstDimension>
{
        typedef MultiArray<T, FirstDimension> SelfType;
        typedef Slice<SelfType, FirstDimension> SliceType;

        typedef std::vector<T> Container;

    public:
        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;

        typedef T& reference_type;
        typedef T const& const_reference_type;
        typedef T value_type;

    public:
        MultiArray(DimensionSize<FirstDimension> const& size);
        ~MultiArray();

        /**
         * @brief 
         */
        reference_type operator[](DimensionIndex<FirstDimension> index);

        /// size
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, FirstDimension>::value, DimensionSize<FirstDimension>>::type 
        size() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, FirstDimension>::value, DimensionSize<Dim>>::type 
        size() const;

        template<typename Dimension>
        void resize(DimensionSize<Dimension> size);

        /// bulk iterators
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

    protected:
        MultiArray(bool disable_resize_tag, DimensionSize<FirstDimension> const&);

        /// resize
        template<typename Dimension>
        typename std::enable_if<std::is_same<Dimension, FirstDimension>::value, void>::type 
        do_resize(std::size_t total_size, DimensionSize<Dimension> size);

        template<typename Dimension>
        typename std::enable_if<!std::is_same<Dimension, FirstDimension>::value, void>::type 
        do_resize(std::size_t total_size, DimensionSize<Dimension> size);

    private:
        DimensionSize<FirstDimension> _size;
        std::vector<T> _data;
};

} // namespace astrotypes
} // namespace pss

#include "detail/MultiArray.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_H
