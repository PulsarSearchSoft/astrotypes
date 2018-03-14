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

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      template classes to specify multiple dimesion arrays with explicit dimension types
 * @tparam T the type fo object to be stored in the array (e.g. float, int, Stikes, etc)
 *
 * @details
 */
template<typename T, typename... Dimensions>
class MultiArray
{
};

template<typename T, typename FirstDimension, typename SecondDimension, typename... OtherDimensions>
class MultiArray<T, FirstDimension, SecondDimension, OtherDimensions...>
{
        typedef MultiArray<T> SelfType;

    public:
        typedef T value_type;
        typedef T& reference_type;

    public:
        MultiArray(DimensionSize<SecondDimension> const&, DimensionSize<OtherDimensions> const& ... sizes);
        ~MultiArray();

        /**
         * @brief take a slice of data at the specified index of the first dimension
         */
        Slice<SelfType, OtherDimensions...> operator[](std::size_t index);

        /**
         * @brief resize the array in the specified dimension
         */
        template<typename Dimension>
        void resize(std::size_t size);

        /**
         * @brief resize the array in the specified dimension
         */
        template<typename Dimension>
        void size();

    private:
};

// specialisation for single dimension arrays
template<typename T, typename FirstDimension>
class MultiArray<T, FirstDimension>
{
        typedef MultiArray<T, FirstDimension> SelfType;
        typedef T& reference_type;
        typedef T const& const_reference_type;
        typedef T value_type;

    public:
        MultiArray(DimensionSize<FirstDimension> const& size);
        ~MultiArray();

        /**
         * @brief 
         */
        reference_type operator[](std::size_t index);
        const_reference_type operator[](std::size_t index) const;

    private:
};

} // namespace astrotypes
} // namespace pss

#include "detail/MultiArray.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_H
