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
#ifndef PSS_ASTROTYPES_MULTIARRAY_TEST_TESTMULTIARRAY_H
#define PSS_ASTROTYPES_MULTIARRAY_TEST_TESTMULTIARRAY_H

#include "pss/astrotypes/multiarray/MultiArray.h"

namespace pss {
namespace astrotypes {
namespace multiarray {
namespace test {

struct DimensionA {};
struct DimensionB {};
struct DimensionC {};

/**
 * @brief An instantiation of the MutliArray template for unit tests
 * @details
 */

template<typename T>
class TestMultiArrayMixin : public T
{
    public:
        TestMultiArrayMixin(T const& t) : T(t) {}
        using T::T;
};

template<typename T, typename... Dimensions>
class TestMultiArray : public MultiArray<std::allocator<T>, T, TestMultiArrayMixin, Dimensions...>
{
        typedef MultiArray<std::allocator<T>, T, TestMultiArrayMixin,  Dimensions...> BaseT;

    public:
        template<typename... Args>
        TestMultiArray(TestMultiArray<Args...> const& arg) : BaseT(arg) {
        }

        template<typename... Args>
        TestMultiArray(DimensionSize<Args>... args) : BaseT(args...) {
            unsigned n=0;
            // fill with a number sequence 1,2,3,4.....
            std::generate(this->begin(), this->end(), [&] () mutable { 
                                                                       return n++; 
                                                                     });
        }
};

} // namespace test
} // namespace multiarray

// this must be declared in the astrotypes namespace
template<typename Dimension, typename T, typename... Dimensions>
struct has_dimension<multiarray::test::TestMultiArray<T, Dimensions...>, Dimension> : public list_has_type<Dimension, Dimensions...>::type
{
};

typedef multiarray::test::DimensionA DimensionA;
typedef multiarray::test::DimensionB DimensionB;
typedef multiarray::test::DimensionC DimensionC;
static_assert(has_dimension<multiarray::test::TestMultiArray<int, DimensionA>, DimensionA>::value, "oh oh");
static_assert(!has_dimension<multiarray::test::TestMultiArray<int, DimensionA>,DimensionB>::value, "oh oh");
static_assert(has_dimension<multiarray::test::TestMultiArray<int, DimensionA, DimensionB>, DimensionA>::value, "oh oh");
static_assert(has_dimension<multiarray::test::TestMultiArray<int, DimensionA, DimensionB>, DimensionB>::value, "oh oh");
static_assert(!has_dimension<multiarray::test::TestMultiArray<int, DimensionA, DimensionB>, DimensionC>::value, "oh oh");

} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_MULTIARRAY_TEST_TESTMULTIARRAY_H
