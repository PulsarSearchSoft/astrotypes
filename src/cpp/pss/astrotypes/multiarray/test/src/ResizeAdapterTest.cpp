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
namespace multiarray {
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

// Test stream that provides the dimension<Dimension> const interface
template<typename... Dimensions>
class TestDimensionStream
{
        typedef std::tuple<DimensionSize<Dimensions>...> TupleType;

    public:
        TestDimensionStream(DimensionSize<Dimensions>... sizes)
        {
            tuple_insert_type(_tuple, sizes...);
        }
        
        template<typename Dimension> 
        DimensionSize<Dimension> dimension() const
        {
            return std::get<find_type<TupleType, DimensionSize<Dimension>>::value>(_tuple);
        }

    private:
        TupleType _tuple;
};

template<typename T, typename Dim, typename... Dims, typename... SDims>
TestDimensionStream<SDims...>& operator>>(TestDimensionStream<SDims...>& is, TestMultiArray<T, Dim, Dims...>&) { return is;}

TEST_F(ResizeAdapterTest, test_single_adapter)
{
    TestMultiArray<uint8_t, DimensionA, DimensionB> data(DimensionSize<DimensionA>(0), DimensionSize<DimensionB>(0));

    TestDimensionStream<DimensionA> ss(DimensionSize<DimensionA>(2));
    DimensionSize<DimensionA> size_a(4);

    ResizeAdapter<DimensionA> adapter(size_a);
    ss >> adapter >> data;

    ASSERT_EQ(data.dimension<DimensionA>(), ss.dimension<DimensionA>());
}

TEST_F(ResizeAdapterTest, test_chained_adapter)
{
    TestMultiArray<uint8_t, DimensionA, DimensionB, DimensionC> data(
            DimensionSize<DimensionA>(0)
          , DimensionSize<DimensionB>(0)
          , DimensionSize<DimensionC>(0));

    DimensionSize<DimensionA> size_a(2);
    DimensionSize<DimensionB> size_b(3);
    DimensionSize<DimensionC> size_c(4);
    DimensionSize<DimensionB> size_b_override(6);
    DimensionSize<DimensionC> size_c_override(8);
    TestDimensionStream<DimensionA, DimensionB, DimensionC> ss(size_a, size_b, size_c);

    ResizeAdapter<DimensionA> adapter_a;
    ResizeAdapter<DimensionB> adapter_b(size_b_override);
    ResizeAdapter<DimensionC> adapter_c(size_c_override);
    ss >> adapter_a >> adapter_b >> adapter_c >> data;

    ASSERT_EQ(data.dimension<DimensionA>(), size_a);
    ASSERT_EQ(data.dimension<DimensionB>(), size_b_override);
    ASSERT_EQ(data.dimension<DimensionC>(), size_c_override);
}

TEST_F(ResizeAdapterTest, test_multi_dim_adapter)
{
    TestMultiArray<uint8_t, DimensionA, DimensionB> data(DimensionSize<DimensionA>(0), DimensionSize<DimensionB>(0));

    DimensionSize<DimensionA> size_a(2);
    DimensionSize<DimensionB> size_b(3);
    TestDimensionStream<DimensionA, DimensionB> ss(size_a, size_b);

    ResizeAdapter<DimensionA, DimensionB> adapter_a_b(size_a, size_b);
    ss >> adapter_a_b >> data;
    ASSERT_EQ(data.dimension<DimensionA>(), size_a);
    ASSERT_EQ(data.dimension<DimensionB>(), size_b);

    TestMultiArray<uint8_t, DimensionA, DimensionB> data_2(DimensionSize<DimensionA>(0), DimensionSize<DimensionB>(0));
    ResizeAdapter<DimensionB, DimensionA> adapter_b_a(size_a, size_b);
    ss >> adapter_b_a >> data_2;
    ASSERT_EQ(data_2.dimension<DimensionA>(), size_a);
    ASSERT_EQ(data_2.dimension<DimensionB>(), size_b);
}

} // namespace test
} // namespace multiarray
} // namespace astrotypes
} // namespace pss
