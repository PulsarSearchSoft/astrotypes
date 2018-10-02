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
#include "../ResizeAdapterTest.h"
#include "pss/astrotypes/multiarray/test/TestMultiArray.h"
#include "pss/astrotypes/multiarray/ResizeAdapter.h"
#include <sstream>


namespace pss {
namespace astrotypes {
namespace test {

ResizeAdapterTest::ResizeAdapterTest()
    : ::testing::Test()
{
}

ResizeAdapterTest::~ResizeAdapterTest()
{
}

void ResizeAdapterTest::SetUp()
{
}

void ResizeAdapterTest::TearDown()
{
}

template<typename T, typename Dim, typename... Dims>
std::istream& operator>>(std::istream& is, TestMultiArray<T, Dim, Dims...>&) { return is;}

TEST_F(ResizeAdapterTest, test_single_adapter)
{
    TestMultiArray<uint8_t, DimensionA, DimensionB> data(DimensionSize<DimensionA>(0), DimensionSize<DimensionB>(0));

    std::stringstream ss("hello");
    DimensionSize<DimensionA> size_a(2);

    ResizeAdapter<DimensionA> adapter(size_a);
    ss >> adapter >> data;

    ASSERT_EQ(data.dimension<DimensionA>(), size_a);
}

TEST_F(ResizeAdapterTest, test_multi_adapter)
{
    TestMultiArray<uint8_t, DimensionA, DimensionB, DimensionC> data(
            DimensionSize<DimensionA>(0)
          , DimensionSize<DimensionB>(0)
          , DimensionSize<DimensionC>(0));

    std::stringstream ss("hello");
    DimensionSize<DimensionA> size_a(2);
    DimensionSize<DimensionB> size_b(3);
    DimensionSize<DimensionC> size_c(4);

    ResizeAdapter<DimensionA> adapter_a(size_a);
    ResizeAdapter<DimensionB> adapter_b(size_b);
    ResizeAdapter<DimensionC> adapter_c(size_c);
    ss >> adapter_a >> adapter_b >> adapter_c >> data;

    ASSERT_EQ(data.dimension<DimensionA>(), size_a);
    ASSERT_EQ(data.dimension<DimensionB>(), size_b);
    ASSERT_EQ(data.dimension<DimensionC>(), size_c);
}

} // namespace test
} // namespace astrotypes
} // namespace pss
