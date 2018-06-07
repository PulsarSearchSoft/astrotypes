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
    typedef typename std::vector<value_type>::const_iterator const_iterator;

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

    const_iterator begin() const {
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
    Slice<false, ParentType<1>, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(0U, static_cast<std::size_t>(slice.size<DimensionB>()));

    // test operator[]
    for(std::size_t i = 0; i < slice.size<DimensionA>(); ++i) {
        ASSERT_EQ(slice[i], i+ 10U); // check we can read
        slice[i]=i;                  // and write
    }
}

TEST_F(SliceTest, test_single_dimension_iterators)
{
    ParentType<1> p(50);
    Slice<false, ParentType<1>, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));

    // test operator[]
    auto it = slice.begin();
    auto const& const_slice = slice;   
    auto it2 = const_slice.begin();
    auto it3 = const_slice.cbegin();

    for(std::size_t i = 0; i < slice.size<DimensionA>(); ++i) {
        ASSERT_FALSE(it == slice.end()) << "i=" << i << " end=" << *slice.end() << " it=" << *it;
        ASSERT_FALSE(it2 == slice.end()) << "i=" << i << " end=" << *slice.end() << " it=" << *it2;
        ASSERT_FALSE(it3 == slice.cend()) << "i=" << i << " end=" << *slice.cend() << " it=" << *it3;
        ASSERT_EQ(*it, i+ 10U); // check we can read
        ASSERT_EQ(*it2, i+ 10U); // check we can read
        ASSERT_EQ(*it3, i+ 10U); // check we can read
        // check non const iterator can write
        (*it) += 1;
        ASSERT_EQ(*it, i+ 11U); // check we can write
        ++it;
        ++it2;
        ++it3;
    }
    ASSERT_TRUE(it == slice.end()) << "end=" << *slice.end() << " it=" << *it; 
    ASSERT_TRUE(it2 == slice.end()) << "end=" << *slice.cend() << " it2=" << *it2; 
    ASSERT_TRUE(it3 == slice.cend()) << "end=" << *slice.cend() << " it3=" << *it3; 
}

TEST_F(SliceTest, test_two_dimensions)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
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
    Slice<false, ParentType<3>, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
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

TEST_F(SliceTest, test_three_dimensions_same_dim_sub_slice)
{
    ParentType<3> p(50);
    Slice<false, ParentType<3>, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
                                              );
    // cut a sub slice (should be of the same type - hence no auto
    Slice<false, ParentType<3>, DimensionA, DimensionB, DimensionC> sub_slice =  
                    slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionIndex<DimensionA>(6)));

    ASSERT_EQ(4U, static_cast<std::size_t>(sub_slice.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(sub_slice.size<DimensionB>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(sub_slice.size<DimensionC>()));
    
    for(std::size_t i = 0; i < sub_slice.size<DimensionA>(); ++i) {
        for(std::size_t j = 0; j < sub_slice.size<DimensionB>(); ++j) {
            for(std::size_t k = 0; k < sub_slice.size<DimensionC>(); ++k) {
            ASSERT_EQ(sub_slice[i][j][k], 
                          (i + 1U + 2U) * static_cast<std::size_t>(p.size<DimensionB>()) * static_cast<std::size_t>(p.size<DimensionC>())
                        + (j + 20U) * static_cast<std::size_t>(p.size<DimensionC>())
                        + k + 2U) << "i=" << i << " j=" << j << " k=" << k; // check we can read
            }
        }
    }
}

TEST_F(SliceTest, test_three_dimensions_slice_iterators)
{
    ParentType<3> p(50);
    Slice<false, ParentType<3>, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    Slice<false, ParentType<3>, DimensionA, DimensionB, DimensionC> sub_slice =  
                    slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionIndex<DimensionA>(4)));

    auto it = sub_slice.begin();
    for(std::size_t i = 0; i < sub_slice.size<DimensionA>(); ++i) {
        for(std::size_t j = 0; j < sub_slice.size<DimensionB>(); ++j) {
            for(std::size_t k = 0; k < sub_slice.size<DimensionC>(); ++k) {
                unsigned val = *it;
                ASSERT_EQ( val, sub_slice[i][j][k]) << "i=" << i << " j=" << j << " k=" << k; // check we can read
                ASSERT_FALSE(it == sub_slice.end()) << "i=" << i << " j=" << j << " k=" << k << " end=" << *sub_slice.end() << " it=" << *it;
                ++it;
            }
        }
    }
    ASSERT_TRUE(it == sub_slice.end()) << "end=" << *sub_slice.end() << " it=" << *it; 
}

TEST_F(SliceTest, const_test_three_dimensions_slice_iterators)
{
    ParentType<3> const p(50);
    Slice<true, ParentType<3>, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    Slice<true, ParentType<3>, DimensionA, DimensionB, DimensionC> sub_slice =  
                    slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionIndex<DimensionA>(4)));

    auto it = sub_slice.begin();
    auto it2 = sub_slice.cbegin();
    auto const& const_sub_slice = sub_slice;
    auto it3 = const_sub_slice.begin();
    for(std::size_t i = 0; i < sub_slice.size<DimensionA>(); ++i) {
        for(std::size_t j = 0; j < sub_slice.size<DimensionB>(); ++j) {
            for(std::size_t k = 0; k < sub_slice.size<DimensionC>(); ++k) {
                unsigned val = *it;
                ASSERT_EQ( val, sub_slice[i][j][k]) << "i=" << i << " j=" << j << " k=" << k; // check we can read
                ASSERT_FALSE(it == sub_slice.end()) << "i=" << i << " j=" << j << " k=" << k << " end=" << *sub_slice.end() << " it=" << *it;
                ASSERT_FALSE(it2 == sub_slice.cend()) << "i=" << i << " j=" << j << " k=" << k << " end=" << *sub_slice.cend() << " it2=" << *it2;
                ASSERT_FALSE(it3 == const_sub_slice.end()) << "i=" << i << " j=" << j << " k=" << k << " end=" << *const_sub_slice.cend() << " it3=" << *it3;
                ++it;
                ++it2;
                ++it3;
            }
        }
    }
    ASSERT_TRUE(it == sub_slice.end()) << "end=" << *sub_slice.end() << " it=" << *it; 
    ASSERT_TRUE(it2 == sub_slice.cend()) << "end=" << *sub_slice.cend() << " it2=" << *it2; 
    ASSERT_TRUE(it3 == sub_slice.cend()) << "end=" << *sub_slice.cend() << " it2=" << *it2; 
}

} // namespace test
} // namespace astrotypes
} // namespace pss
