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
#include "pss/astrotypes/multiarray/test/DimensionIndexTest.h"
#include "pss/astrotypes/multiarray/DimensionIndex.h"


namespace pss {
namespace astrotypes {
namespace multiarray {
namespace test {


DimensionIndexTest::DimensionIndexTest()
    : ::testing::Test()
{
}

DimensionIndexTest::~DimensionIndexTest()
{
}

void DimensionIndexTest::SetUp()
{
}

void DimensionIndexTest::TearDown()
{
}

struct DimensionA;

TEST_F(DimensionIndexTest, test_add_integer)
{
    DimensionIndex<DimensionA> i(10);
    auto r = i + 1;
    static_assert(std::is_same<DimensionIndex<DimensionA>, decltype(r)>::value, "wrong type returned from operator+");
    ASSERT_EQ(r, DimensionIndex<DimensionA>(11));
}

} // namespace test
} // namespace multiarray
} // namespace astrotypes
} // namespace pss
