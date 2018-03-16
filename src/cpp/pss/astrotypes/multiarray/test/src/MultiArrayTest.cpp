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
#include "../MultiArrayTest.h"
#include "pss/astrotypes/multiarray/MultiArray.h"


namespace pss {
namespace astrotypes {
namespace test {


MultiArrayTest::MultiArrayTest()
    : ::testing::Test()
{
}

MultiArrayTest::~MultiArrayTest()
{
}

void MultiArrayTest::SetUp()
{
}

void MultiArrayTest::TearDown()
{
}

struct DimensionA {};
struct DimensionB {};
struct DimensionC {};

TEST_F(MultiArrayTest, test_single_dimension_size)
{
    DimensionSize<DimensionA> size(10);
    MultiArray<int, DimensionA> ma(size);
    ASSERT_EQ(ma.size<DimensionA>(), size);
    ASSERT_EQ(ma.size<DimensionB>(), 0U);
    ASSERT_EQ(ma.size<DimensionC>(), 0U);
}

TEST_F(MultiArrayTest, test_single_dimension_slice)
{
    DimensionSize<DimensionA> size(30);
    MultiArray<int, DimensionA> ma(size);
    auto slice = ma[DimensionIndex<DimensionA>(0)];
    ASSSERT_EQ(slice.template size<DimensionA>(), size);
    ASSSERT_EQ(slice.template size<DimensionB>(), DimensionSize<DimensionB>(0));
}

TEST_F(MultiArrayTest, test_three_dimension_size)
{
    DimensionSize<DimensionA> size_a(30);
    DimensionSize<DimensionB> size_b(30);
    DimensionSize<DimensionC> size_c(30);
    MultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    ASSERT_EQ(ma.size<DimensionA>(), size_a);
    ASSERT_EQ(ma.size<DimensionB>(), size_b);
    ASSERT_EQ(ma.size<DimensionC>(), size_c);
}

TEST_F(MultiArrayTest, test_three_dimension_slice_operator)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(30);
    DimensionSize<DimensionC> size_c(30);
    MultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    for(DimensionIndex<DimensionA> index(0); index < size_a; ++index)
    {
        auto slice = ma[index];
        ASSERT_EQ(slice.size<DimensionA>(), size_a, DimensionIndex<DimensionA>(9) - index);
        ASSERT_EQ(slice.size<DimensionB>(), size_b, DimensionSize<DimensionB>);
        ASSERT_EQ(slice.size<DimensionC>(), size_c, DimensionSize<DimensionC>);
    }
}

} // namespace test
} // namespace astrotypes
} // namespace pss
