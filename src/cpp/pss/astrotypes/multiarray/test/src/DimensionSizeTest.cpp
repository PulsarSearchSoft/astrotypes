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
#include "pss/astrotypes/multiarray/test/DimensionSizeTest.h"
#include "pss/astrotypes/multiarray/DimensionSize.h"


namespace pss {
namespace astrotypes {
namespace multiarray {
namespace test {


DimensionSizeTest::DimensionSizeTest()
    : ::testing::Test()
{
}

DimensionSizeTest::~DimensionSizeTest()
{
}

void DimensionSizeTest::SetUp()
{
}

void DimensionSizeTest::TearDown()
{
}

struct DimensionA;

TEST_F(DimensionSizeTest, test_operator_plus_plus)
{
    DimensionSize<DimensionA> d(100);
    // pre increment op
    ASSERT_EQ(DimensionSize<DimensionA>(101), ++d);

    // post increment op
    ASSERT_EQ(DimensionSize<DimensionA>(101), d++);
    ASSERT_EQ(DimensionSize<DimensionA>(102), d);
}

TEST_F(DimensionSizeTest, test_operator_multiply)
{
    DimensionSize<DimensionA> d(10);
    static_assert(std::is_same<DimensionSize<DimensionA>, decltype(d*10)>::value, "wrong type");
    ASSERT_EQ(d*10, DimensionSize<DimensionA>(100));
}

TEST_F(DimensionSizeTest, test_operator_divide)
{
    DimensionSize<DimensionA> d(100);
    static_assert(std::is_same<DimensionSize<DimensionA>, decltype(d/10)>::value, "wrong type");
    ASSERT_EQ(d/10, DimensionSize<DimensionA>(10));
}

} // namespace test
} // namespace multiarray
} // namespace astrotypes
} // namespace pss
