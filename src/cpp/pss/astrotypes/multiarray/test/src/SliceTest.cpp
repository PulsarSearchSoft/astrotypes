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
#include "../SliceTest.h"
#include "pss/astrotypes/multiarray/Slice.h"

#include <vector>
#include <cmath>


namespace pss {
namespace astrotypes {
namespace test {


SliceTest::SliceTest()
    : ::testing::Test()
{
}

SliceTest::~SliceTest()
{
}

void SliceTest::SetUp()
{
}

void SliceTest::TearDown()
{
}

struct DimensionA {};
struct DimensionB {};
struct DimensionC {};

template<int NDim>
struct ParentType {
    typedef int value_type;
    typedef int& reference_type;
    typedef typename std::vector<value_type>::iterator iterator;

    ParentType(std::size_t size)
        : _vec(std::pow(size, NDim))
        , _size(size)
    {
        unsigned index=0;
        //int offset=0;
        
        while(index<_vec.size()) {
            _vec[index]=index;
            ++index;
        }
    }

    iterator begin() {
        return _vec.begin();
    }

    template<typename Dimension>
    DimensionSize<Dimension> size() const {
        return _size;
    }

    std::vector<value_type> _vec;
    std::size_t _size;
};


TEST_F(SliceTest, test_single_dimension)
{
    ParentType<1> p(50);
    Slice<ParentType<1>, DimensionA> slice(p, std::make_pair(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(0U, static_cast<std::size_t>(slice.size<DimensionB>()));

    // test operator[]
    for(std::size_t i = 0; i < slice.size<DimensionA>(); ++i) {
        ASSERT_EQ(slice[i], i+ 10U); // check we can read
        slice[i]=i;                  // and write
    }
}

TEST_F(SliceTest, test_two_dimensions)
{
    ParentType<2> p(50);
    Slice<ParentType<2>, DimensionA, DimensionB> slice(p
                                              , std::make_pair(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , std::make_pair(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice.size<DimensionB>()));

    // test operator[]
    ASSERT_EQ(3U, static_cast<std::size_t>(slice[0].size<DimensionB>()));

    for(std::size_t i = 0; i < slice.size<DimensionA>(); ++i) {
        for(std::size_t j = 0; j < slice.size<DimensionB>(); ++j) {
            ASSERT_EQ(slice[i][j], (i + 10U) * static_cast<std::size_t>(p.size<DimensionB>()) + j + 20U) << "i=" << i << " j=" << j; // check we can read
        }
    }
}


TEST_F(SliceTest, test_three_dimensions)
{
    ParentType<3> p(50);
    Slice<ParentType<3>, DimensionA, DimensionB, DimensionC> slice(p
                                              , std::make_pair(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , std::make_pair(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , std::make_pair(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
                                              );
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice.size<DimensionB>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(slice.size<DimensionC>()));

    // test operator[]
    ASSERT_EQ(3U, static_cast<std::size_t>(slice[0].size<DimensionB>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(slice[0].size<DimensionC>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(slice[0][0].size<DimensionC>()));

    for(std::size_t i = 0; i < slice.size<DimensionA>(); ++i) {
        for(std::size_t j = 0; j < slice.size<DimensionB>(); ++j) {
            for(std::size_t k = 0; k < slice.size<DimensionC>(); ++k) {
            ASSERT_EQ(slice[i][j][k], 
                          (i + 1U) * static_cast<std::size_t>(p.size<DimensionB>()) * static_cast<std::size_t>(p.size<DimensionC>())
                        + (j + 20U) * static_cast<std::size_t>(p.size<DimensionC>())
                        + k + 2U) << "i=" << i << " j=" << j << " k=" << k; // check we can read
            }
        }
    }
}

} // namespace test
} // namespace astrotypes
} // namespace pss
