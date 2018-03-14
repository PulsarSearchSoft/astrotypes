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

struct ParentType {
    typedef int value_type;
    typedef int& reference_type;
    typedef typename std::vector<value_type>::iterator iterator;

    ParentType(std::size_t size)
        : _vec(size)
    {
        for(std::size_t i=0; i<size; ++i)
        {
            _vec[i]=i;
        }
    }

    iterator begin() {
        return _vec.begin();
    }

    std::vector<value_type> _vec;
};

TEST_F(SliceTest, test_single_dimension)
{
    ParentType p(100);
    Slice<ParentType, DimensionA> slice(p, std::make_pair(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(0U, static_cast<std::size_t>(slice.size<DimensionB>()));

    // test operator[]
    for(std::size_t i = 0; i < slice.size<DimensionA>(); ++i) {
        ASSERT_EQ(slice[i], i+ 10U); // check we can read
        slice[i]=i;                  // and write
    }
}

} // namespace test
} // namespace astrotypes
} // namespace pss
