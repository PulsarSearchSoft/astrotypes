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
#include "../TestMultiArray.h"
#include "pss/astrotypes/multiarray/MultiArray.h"
#include <algorithm>


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

TEST_F(MultiArrayTest, test_single_dimension_size)
{
    DimensionSize<DimensionA> size(10);
    TestMultiArray<int, DimensionA> ma(size);
    ASSERT_EQ(ma.dimension<DimensionA>(), size);
    ASSERT_EQ(ma.dimension<DimensionB>(), DimensionSize<DimensionB>(0U));
    ASSERT_EQ(ma.dimension<DimensionC>(), DimensionSize<DimensionC>(0U));
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size);
}

TEST_F(MultiArrayTest, test_single_dimension_const_iterator)
{
    DimensionSize<DimensionA> size(30);
    TestMultiArray<int, DimensionA> ma(size);
    auto it = ma.cbegin();
    auto end = ma.cend();
    ASSERT_EQ(std::distance(it, end), (std::size_t)size);
    unsigned n=0;
    std::for_each(it, end, [&](int const& val) 
                           {
                                ASSERT_EQ(val, n);
                                ++n;
                           });
    ASSERT_EQ(n, (unsigned)size);
}

TEST_F(MultiArrayTest, test_two_dimension_1_slice_operator_DimensionB_const_iterator)
{
    DimensionSize<DimensionA> size_a(30);
    DimensionSize<DimensionB> size_b(20);
    TestMultiArray<int, DimensionA, DimensionB> ma(size_a, size_b);

    // slice 0 - sanity checks for first slice
    auto slice0 = ma.slice(DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(0), DimensionSize<DimensionB>(1)));
    ASSERT_EQ(&*slice0.begin(), &*ma.begin());
    int* end0=&*ma.begin() + (size_b * (size_a - 1) +1 );
    ASSERT_EQ(&*slice0.end(), end0) << "delta=" << end0 - &*slice0.end();

    ASSERT_EQ(slice0.dimension<DimensionA>(), size_a);
    ASSERT_EQ(slice0.dimension<DimensionB>(), DimensionSize<DimensionB>(1));
    auto it = slice0.cbegin();
    auto end = slice0.cend();
    ASSERT_EQ(std::distance(it, end), (std::size_t)size_a);
    unsigned n=0;
    std::for_each(it, end, [&](int const& val) 
                           {
                                ASSERT_EQ(val, n);
                                n += (std::size_t)size_b;
                           });
    ASSERT_EQ(n, (unsigned)size_b * size_a);

    // slice q - sanity checks for first slice
    auto slice1 = ma.slice(DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(1), DimensionSize<DimensionB>(1)));
    ASSERT_EQ(&*slice1.begin(), (&*ma.begin()) + 1);
    int* end1=&*slice1.begin() + (size_b * (size_a - 1) +1 );
    ASSERT_EQ(&*slice1.end(), end1) << "delta=" << end1 - &*slice1.end();

}

TEST_F(MultiArrayTest, test_two_dimension_1_slice_DimensionA_const_iterator)
{
    DimensionSize<DimensionA> size_a(30);
    DimensionSize<DimensionB> size_b(20);
    TestMultiArray<int, DimensionA, DimensionB> ma(size_a, size_b);
    auto slice = ma[DimensionIndex<DimensionA>(1)];
    ASSERT_EQ(slice.dimension<DimensionB>(), size_b);
    auto it = slice.cbegin();
    auto end = slice.cend();
    ASSERT_EQ(std::distance(it, end), (std::size_t)size_b);
    unsigned n=(unsigned)size_b;
    std::for_each(it, end, [&](int const& val) 
                           {
                                ASSERT_EQ(val, n);
                                ++n;
                           });
    ASSERT_EQ(n-(unsigned)size_b, (unsigned)size_b);
}

TEST_F(MultiArrayTest, test_two_dimension_1_slice_operator_DimensionA_const_iterator)
{
    DimensionSize<DimensionA> size_a(30);
    DimensionSize<DimensionB> size_b(20);
    TestMultiArray<int, DimensionA, DimensionB> ma(size_a, size_b);
    auto slice = ma.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionSize<DimensionA>(1)));
    ASSERT_EQ(slice.dimension<DimensionB>(), size_b);
    auto it = slice.cbegin();
    auto end = slice.cend();
    ASSERT_EQ(std::distance(it, end), (std::size_t)size_b);
    unsigned n=(unsigned)size_b;
    std::for_each(it, end, [&](int const& val) 
                           {
                                ASSERT_EQ(val, n);
                                ++n;
                           });
    ASSERT_EQ(n-(unsigned)size_b, (unsigned)size_b);
}

/*
TEST_F(MultiArrayTest, test_two_dimension_slice_range_outside_of_array_DimensionA)
{
    DimensionSize<DimensionA> size_a(30);
    DimensionSize<DimensionB> size_b(20);
    TestMultiArray<int, DimensionA, DimensionB> ab(size_a, size_b);

    auto slice = ab.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(31), DimensionSize<DimensionA>(2)));
    ASSERT_EQ(slice.dimension<DimensionA>(), DimensionSize<DimensionA>(0));
    ASSERT_EQ(slice.begin(), slice.end());
}

TEST_F(MultiArrayTest, test_two_dimension_slice_range_outside_of_array_DimensionB)
{
    DimensionSize<DimensionA> size_a(6);
    DimensionSize<DimensionB> size_b(5);
    TestMultiArray<int, DimensionA, DimensionB> ab(size_a, size_b);

    auto slice = ab.slice(DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(8), DimensionSize<DimensionB>(2)));
    ASSERT_EQ(slice.dimension<DimensionB>(), DimensionSize<DimensionB>(0));
    ASSERT_EQ(slice.dimension<DimensionA>(), size_a);
    ASSERT_EQ(&*slice.begin(), &*ab.end()) << "begin=" << (void*) &*slice.begin() << " ab.begin=" << &*ab.begin() << " ab.end=" << (void*)&*ab.end();
    ASSERT_EQ(slice.begin(), slice.end()) << "begin=" << (void*) &*slice.begin() << " end=" << (void*)&*slice.end();
}

TEST_F(MultiArrayTest, test_two_dimension_slice_range_extends_beyond_array)
{
    DimensionSize<DimensionA> size_a(20);
    DimensionSize<DimensionB> size_b(20);
    TestMultiArray<int, DimensionA, DimensionB> ab(size_a, size_b);

    auto slice = ab.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionSize<DimensionA>(30)));
    ASSERT_EQ(slice.dimension<DimensionA>(), DimensionSize<DimensionA>(10));
    ASSERT_NE(slice.begin(), slice.end());
    ASSERT_EQ(&*ab.end(), &*slice.end());
}
*/


TEST_F(MultiArrayTest, test_two_dimension_transpose_constructor)
{
    DimensionSize<DimensionA> size_a(30);
    DimensionSize<DimensionB> size_b(20);
    TestMultiArray<int, DimensionA, DimensionB> ab(size_a, size_b);

    TestMultiArray<int, DimensionB, DimensionA> ba(ab);
    ASSERT_EQ(ba.dimension<DimensionA>(), size_a);
    ASSERT_EQ(ba.dimension<DimensionB>(), size_b);

    for(DimensionIndex<DimensionA> i(0); i < ab.dimension<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < ab.dimension<DimensionB>(); ++j) {
            ASSERT_EQ(ba[j][i], ab[i][j]) << " i=" << i << " j=" << j << " " << &ba[j][i];
        }
    }
}

TEST_F(MultiArrayTest, test_three_dimension_size)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    ASSERT_EQ(ma.dimension<DimensionA>(), size_a);
    ASSERT_EQ(ma.dimension<DimensionB>(), size_b);
    ASSERT_EQ(ma.dimension<DimensionC>(), size_c);
}

TEST_F(MultiArrayTest, test_three_dimension_square_bracket_operator)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    for(DimensionIndex<DimensionA> index(0); index < size_a; ++index)
    {
        auto slice = ma[index];
        //ASSERT_EQ(slice.dimension<DimensionA>(), DimensionIndex<DimensionA>(9) - index);
        //ASSERT_EQ(slice.dimension<DimensionA>(), DimensionSize<DimensionA>(0));
        ASSERT_EQ(slice.dimension<DimensionB>(), size_b);
        ASSERT_EQ(slice.dimension<DimensionC>(), size_c);
    }
}

TEST_F(MultiArrayTest, test_const_three_dimension_square_bracket_operator)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> const ma( size_a, size_b, size_c);
    for(DimensionIndex<DimensionA> index(0); index < size_a; ++index)
    {
        auto slice = ma[index];
        //ASSERT_EQ(slice.dimension<DimensionA>(), DimensionIndex<DimensionA>(9) - index);
        //ASSERT_EQ(slice.dimension<DimensionA>(), DimensionSize<DimensionA>(0));
        ASSERT_EQ(slice.dimension<DimensionB>(), size_b);
        ASSERT_EQ(slice.dimension<DimensionC>(), size_c);
    }
}

TEST_F(MultiArrayTest, test_three_dimension_slice_operator)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    for(DimensionIndex<DimensionA> index(0); index < size_a; ++index)
    {
        SCOPED_TRACE(index);
        for(DimensionIndex<DimensionA> index_2(index + DimensionSize<DimensionA>(1)); index_2 < size_a; ++index_2)
        {
            SCOPED_TRACE(index_2);
            auto slice = ma.slice(DimensionSpan<DimensionA>(index, index_2 ));
            ASSERT_EQ(static_cast<std::size_t>(slice.dimension<DimensionA>()), (index_2 - index) + 1 );
            ASSERT_EQ(slice.dimension<DimensionB>(), size_b);
            ASSERT_EQ(slice.dimension<DimensionC>(), size_c);
        }
    }
}

TEST_F(MultiArrayTest, test_const_three_dimension_slice_operator)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> const ma( size_a, size_b, size_c);
    for(DimensionIndex<DimensionA> index(0); index < size_a; ++index)
    {
        for(DimensionIndex<DimensionA> index_2(index + DimensionSize<DimensionA>(1)); index_2 < size_a; ++index_2)
        {
            auto slice = ma.slice(DimensionSpan<DimensionA>(index, index_2 ));
            ASSERT_EQ(static_cast<std::size_t>(slice.dimension<DimensionA>()), (index_2 - index) + 1 );
            ASSERT_EQ(slice.dimension<DimensionB>(), size_b);
            ASSERT_EQ(slice.dimension<DimensionC>(), size_c);
        }
    }
}

TEST_F(MultiArrayTest, test_three_dimension_std_copy)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);
    // fill some data
    std::vector<unsigned> data(size_a * size_b * size_c);
    unsigned val=0;
    std::generate(data.begin(), data.end(), [&]() { return val++; } );
    
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    std::copy(data.begin(), data.end(), ma.begin()); // copy in

    // ensure the [] operators return the correct values
    for(DimensionIndex<DimensionA> i(0); i < ma.dimension<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < ma.dimension<DimensionB>(); ++j) {
            ASSERT_EQ( i * size_b * size_c + j * size_c + 20, ma[i][j][DimensionIndex<DimensionC>(20)]) << "i=" << i
                                                                                                        << " j=" << j;
        }
    }

    std::vector<unsigned> dst;
    dst.reserve(size_a * size_b * size_c);
    std::copy(ma.begin(), ma.end(), std::back_inserter(dst)); //copy our
    ASSERT_EQ(data.size(), dst.size());
    ASSERT_EQ(data, dst);

    // test cbegin/cend
    dst.clear();
    std::copy(ma.cbegin(), ma.cend(), std::back_inserter(dst)); //copy our
    ASSERT_EQ(data, dst);
}

TEST_F(MultiArrayTest, test_one_dimension_resize)
{
    DimensionSize<DimensionA> size_a(10);

    TestMultiArray<unsigned, DimensionA> ma( size_a);
    
    size_a = DimensionSize<DimensionA>(5);
    ma.resize(size_a);
    ASSERT_EQ(ma.dimension<DimensionA>(), size_a);
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size_a);
}

TEST_F(MultiArrayTest, test_three_dimension_resize)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);

    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    
    size_a = DimensionSize<DimensionA>(5);
    ma.resize(size_a);
    ASSERT_EQ(ma.dimension<DimensionA>(), size_a);
    ASSERT_EQ(ma.dimension<DimensionB>(), size_b);
    ASSERT_EQ(ma.dimension<DimensionC>(), size_c);
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size_a * size_b * size_c);

    size_b = DimensionSize<DimensionB>(15);
    ma.resize(size_b);
    ASSERT_EQ(ma.dimension<DimensionA>(), size_a);
    ASSERT_EQ(ma.dimension<DimensionB>(), size_b);
    ASSERT_EQ(ma.dimension<DimensionC>(), size_c);
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size_a * size_b * size_c);

    size_c = DimensionSize<DimensionC>(25);
    ma.resize(size_c);
    ASSERT_EQ(ma.dimension<DimensionA>(), size_a);
    ASSERT_EQ(ma.dimension<DimensionB>(), size_b);
    ASSERT_EQ(ma.dimension<DimensionC>(), size_c);
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size_a * size_b * size_c);
}

TEST_F(MultiArrayTest, test_three_dimension_resize_multiple_args)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);

    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    size_a = DimensionSize<DimensionA>(5);
    size_b = DimensionSize<DimensionB>(15);
    ma.resize(size_a, size_b);
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size_a * size_b * size_c);

    size_b = DimensionSize<DimensionB>(20);
    size_c = DimensionSize<DimensionC>(5);
    ma.resize(size_b, size_c);
    ASSERT_EQ(std::distance(ma.begin(), ma.end()), size_a * size_b * size_c);
}

TEST_F(MultiArrayTest, test_three_dimension_data_size)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionC> size_c(30);

    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma( size_a, size_b, size_c);
    ASSERT_EQ(ma.data_size(), size_a * size_b * size_c);
}

TEST_F(MultiArrayTest, test_three_dimension_equal_operator)
{
    DimensionSize<DimensionA> size_a(10);
    DimensionSize<DimensionB> size_b(20);
    DimensionSize<DimensionB> size_b_2(30);
    DimensionSize<DimensionC> size_c(30);
    DimensionSize<DimensionC> size_c_2(20);

    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma_1( size_a, size_b, size_c);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma_2( size_a, size_b, size_c);
    TestMultiArray<unsigned, DimensionA, DimensionB, DimensionC> ma_3( size_a, size_b_2, size_c_2);
    ASSERT_EQ(ma_1, ma_2);

    // check dirrefent dimensions (but same overall size)
    ASSERT_EQ(ma_1.data_size(), ma_3.data_size());
    ASSERT_FALSE(ma_1 == ma_3);

    // change the data
    ma_1[DimensionIndex<DimensionA>(0)][DimensionIndex<DimensionB>(0)][DimensionIndex<DimensionC>(5)] = 0;
    ASSERT_FALSE(ma_1 == ma_2);
}

TEST_F(MultiArrayTest, test_has_dimension)
{
    typedef MultiArray<std::allocator<unsigned>, unsigned, TestMultiArrayMixin, DimensionA> TestType1d;
    static_assert(std::is_same<typename has_dimension<TestType1d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename has_dimension<TestType1d, DimensionB>::type, std::false_type>::value, "expecting false");
    typedef MultiArray<std::allocator<unsigned>, unsigned, TestMultiArrayMixin, DimensionA, DimensionB> TestType2d;
    static_assert(std::is_same<typename has_dimension<TestType2d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename has_dimension<TestType2d, DimensionB>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename has_dimension<TestType2d, DimensionC>::type, std::false_type>::value, "expecting false");
}

TEST_F(MultiArrayTest, test_has_exact_dimensions)
{
    typedef MultiArray<std::allocator<unsigned>, unsigned, TestMultiArrayMixin, DimensionA> TestType1d;
    typedef MultiArray<std::allocator<unsigned>, unsigned, TestMultiArrayMixin, DimensionA, DimensionB> TestType2d;

    // 1D
    static_assert(std::is_same<typename has_exact_dimensions<TestType1d, DimensionA, DimensionB>::type, std::false_type>::value, "expecting false");
    static_assert(std::is_same<typename has_exact_dimensions<TestType1d, DimensionB>::type, std::false_type>::value, "expecting false");
    static_assert(std::is_same<typename has_exact_dimensions<TestType1d, DimensionA>::type, std::true_type>::value, "expecting true");

    // 2d
    bool r = has_exact_dimensions<TestType2d, DimensionA, DimensionB>::value;
    ASSERT_TRUE(r);
    static_assert(std::is_same<typename has_exact_dimensions<TestType2d, DimensionB, DimensionA>::type, std::false_type>::value, "expecting false");
    static_assert(std::is_same<typename has_exact_dimensions<TestType2d, DimensionB>::type, std::false_type>::value, "expecting false");
    static_assert(std::is_same<typename has_exact_dimensions<TestType2d, DimensionA, DimensionB, DimensionC>::type, std::false_type>::value, "expecting false");
    static_assert(std::is_same<typename has_exact_dimensions<TestType2d, DimensionA, DimensionB>::type, std::true_type>::value, "expecting true");
}

} // namespace test
} // namespace astrotypes
} // namespace pss
