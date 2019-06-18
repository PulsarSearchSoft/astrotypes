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

template<typename T>
class TestSliceMixin : public T
{
    public:
        TestSliceMixin(T const& t) : T(t) {}
        using T::T;
};

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
        return DimensionSize<Dimension>(_size);
    }

    template<typename Dimension>
    DimensionSize<Dimension> dimension() const {
        return DimensionSize<Dimension>(_size);
    }

    std::vector<value_type> _vec;
    std::size_t _size;
};


TEST_F(SliceTest, test_single_dimension)
{
    ParentType<1> p(50);
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));
    // test size
    ASSERT_EQ(11U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(0U, static_cast<std::size_t>(slice.size<DimensionB>()));
    ASSERT_EQ(11U, static_cast<std::size_t>(slice.dimension<DimensionA>()));
    ASSERT_EQ(0U, static_cast<std::size_t>(slice.dimension<DimensionB>()));

    // test operator[]
    for(std::size_t i = 0; i < slice.dimension<DimensionA>(); ++i) {
        ASSERT_EQ(slice[i], i+ 10U); // check we can read
        slice[i]=i;                  // and write
    }
}

TEST_F(SliceTest, test_single_dimension_iterators)
{
    ParentType<1> p(50);
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19)));

    // test operator[]
    auto it = slice.begin();
    typedef typename std::iterator_traits<typename decltype(p)::iterator>::iterator_category expected_iterator_category;
    static_assert(std::is_same<std::iterator_traits<decltype(it)>::iterator_category, expected_iterator_category>::value,
                                 "expecting a vector iterator category");
    auto const& const_slice = slice;
    auto it2 = const_slice.begin();
    static_assert(std::is_same<std::iterator_traits<decltype(it2)>::iterator_category, expected_iterator_category>::value,
                                 "expecting a vector iterator category");
    auto it3 = const_slice.cbegin();
    static_assert(std::is_same<std::iterator_traits<decltype(it3)>::iterator_category, expected_iterator_category>::value,
                                 "expecting a vector iterator category");

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

TEST_F(SliceTest, test_single_dimension_iterators_diff)
{
    ParentType<1> const p(50);
    Slice<true, ParentType<1>, TestSliceMixin, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));

    auto it = slice.begin();
    auto it2 = slice.begin();
    for(unsigned i=0; i < slice.size<DimensionA>(); ++i) {
        ASSERT_EQ(i, it2 - it) << i;
        ++it2;
    }
}

TEST_F(SliceTest, test_single_dimension_equals)
{
    ParentType<1> const p(50);
    ParentType<1> p2(50);
    Slice<true, ParentType<1>, TestSliceMixin, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> slice_2(p2, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));
    ASSERT_EQ(slice, slice_2);
    slice_2[1] = 0;
    ASSERT_FALSE(slice==slice_2);
}

TEST_F(SliceTest, test_two_dimensions)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              );
    Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> const_slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              );
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice.size<DimensionB>()));
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.dimension<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice.dimension<DimensionB>()));

    // test operator[]
    ASSERT_EQ(3U, static_cast<std::size_t>(slice[DimensionIndex<DimensionA>(0)].size<DimensionB>()));
    ASSERT_EQ(10U, static_cast<std::size_t>(slice[DimensionIndex<DimensionB>(0)].size<DimensionA>()));
    ASSERT_EQ(1U, static_cast<std::size_t>(slice[DimensionIndex<DimensionB>(0)].size<DimensionB>()));

    for(DimensionIndex<DimensionA> i(0); i < slice.size<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < slice.size<DimensionB>(); ++j) {
            ASSERT_EQ(slice[i][j], (static_cast<std::size_t>(i) + 10U) * static_cast<std::size_t>(p.size<DimensionB>()) + static_cast<std::size_t>(j) + 20U) << "i=" << i << " j=" << j; // check we can read
            ASSERT_EQ(const_slice[i][j], (static_cast<std::size_t>(i) + 10U) * static_cast<std::size_t>(p.size<DimensionB>()) + static_cast<std::size_t>(j) + 20U) << "i=" << i << " j=" << j; // check we can read
            ASSERT_EQ(slice[j][i], slice[i][j]) << "i=" << i << " j=" << j; // check we can read
            ASSERT_EQ(const_slice[j][i], slice[i][j]) << "i=" << i << " j=" << j; // check we can read
        }
    }
}

TEST_F(SliceTest, test_two_dimensions_contructor_out_of_order_dims)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice_a(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              );
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice_b(p
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19))
                                              );

    ASSERT_EQ(10U, static_cast<std::size_t>(slice_a.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice_a.size<DimensionB>()));
    ASSERT_EQ(10U, static_cast<std::size_t>(slice_b.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice_b.size<DimensionB>()));

    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice_c(p
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              );
    ASSERT_EQ(50U, static_cast<std::size_t>(slice_c.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice_c.size<DimensionB>()));

    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice_d(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19))
                                              );

    ASSERT_EQ(10U, static_cast<std::size_t>(slice_d.size<DimensionA>()));
    ASSERT_EQ(50U, static_cast<std::size_t>(slice_d.size<DimensionB>()));
}

TEST_F(SliceTest, test_two_dimensions_slice_iterators)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    auto it = slice.begin();
    auto const_it = slice.cbegin();
    ASSERT_EQ(&*it, &*p.begin() + 10 * p.size<DimensionB>() + 20);
    for(DimensionIndex<DimensionA> i(0); i < slice.size<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < slice.size<DimensionB>(); ++j) {
            ASSERT_EQ(slice[i][j], *it);
            ASSERT_EQ(slice[i][j], *const_it);
            ++it;
            ++const_it;
        }
    }

}

TEST_F(SliceTest, test_two_dimensions_slice_of_slice)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> big_slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    {
        // sub slice in DimensionA
        auto slice = big_slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionSize<DimensionA>(6)));
        auto it = slice.begin();
        auto const_it = slice.cbegin();
        ASSERT_EQ(&*it, &*p.begin() + 12 * p.size<DimensionB>() + 20);
        for(DimensionIndex<DimensionA> i(0); i < slice.size<DimensionA>(); ++i) {
            for(DimensionIndex<DimensionB> j(0); j < slice.size<DimensionB>(); ++j) {
                ASSERT_EQ(slice[i][j], *it) << "i=" << i << " j=" << j;
                ASSERT_EQ(slice[i][j], *const_it) << "i=" << i << " j=" << j;
                ++it;
                ++const_it;
            }
        }
    }
    {
        // sub slice in DimensionA and Dimesnion B
        auto slice = big_slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionSize<DimensionA>(6))
                                    ,DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(1), DimensionSize<DimensionB>(2)));
        auto it = slice.begin();
        auto const_it = slice.cbegin();
        ASSERT_EQ(&*it, &*p.begin() + 12 * p.size<DimensionB>() + 21);
        for(DimensionIndex<DimensionA> i(0); i < slice.size<DimensionA>(); ++i) {
            for(DimensionIndex<DimensionB> j(0); j < slice.size<DimensionB>(); ++j) {
                ASSERT_EQ(slice[i][j], *it);
                ASSERT_EQ(slice[i][j], *const_it);
                ++it;
                ++const_it;
            }
        }
    }
    {
        // sub slice in DimensionB only
        auto slice = big_slice.slice(DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(1), DimensionSize<DimensionB>(2)));
        auto it = slice.begin();
        auto const_it = slice.cbegin();
        ASSERT_EQ(&*it, &*p.begin() + 10 * p.size<DimensionB>() + 21);
        for(DimensionIndex<DimensionA> i(0); i < slice.size<DimensionA>(); ++i) {
            for(DimensionIndex<DimensionB> j(0); j < slice.size<DimensionB>(); ++j) {
                ASSERT_EQ(slice[i][j], *it);
                ASSERT_EQ(slice[i][j], *const_it);
                ++it;
                ++const_it;
            }
        }
    }
}

TEST_F(SliceTest, test_two_dimensions_iterators_diff)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    auto it = slice.begin();
    auto it2 = slice.begin();
    // check inner loop
    std::size_t count = 0;
    while(it2 != slice.end()) {
        ASSERT_EQ(count, it2 - it) << count;
        ++count;
        ++it2;
    }
}

TEST_F(SliceTest, test_three_dimensions)
{
    ParentType<3> p(50);
    Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(10))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(6))
                                              );
    // test size
    ASSERT_EQ(10U, static_cast<std::size_t>(slice.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(slice.size<DimensionB>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(slice.size<DimensionC>()));

    // test operator[]
    ASSERT_EQ(3U, static_cast<std::size_t>(slice[DimensionIndex<DimensionA>(0)].size<DimensionB>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(slice[DimensionIndex<DimensionA>(0)].size<DimensionC>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(slice[DimensionIndex<DimensionA>(0)][DimensionIndex<DimensionB>(0)].size<DimensionC>()));

    for(DimensionIndex<DimensionA> i(0); i < slice.size<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < slice.size<DimensionB>(); ++j) {
            for(DimensionIndex<DimensionC> k(0); k < slice.size<DimensionC>(); ++k) {
            ASSERT_EQ(slice[i][j][k],
                          ((std::size_t)i + 1U) * static_cast<std::size_t>(p.size<DimensionB>()) * static_cast<std::size_t>(p.size<DimensionC>())
                        + ((std::size_t)j + 20U) * static_cast<std::size_t>(p.size<DimensionC>())
                        + (std::size_t)k + 2U) << "i=" << i << " j=" << j << " k=" << k; // check we can read
            ASSERT_EQ(slice[i][j][k], slice[i][k][j]);
//            ASSERT_EQ(slice[i][j][k], slice[j][i][k]) << "i=" << i << " j=" << j << " k=" << k;
//            ASSERT_EQ(slice[i][j][k], slice[j][k][i]) << "i=" << i << " j=" << j << " k=" << k;
//            ASSERT_EQ(slice[i][j][k], slice[k][j][i]) << "i=" << i << " j=" << j << " k=" << k;
//            ASSERT_EQ(slice[i][j][k], slice[k][i][j]) << "i=" << i << " j=" << j << " k=" << k;
            }
        }
    }
}

TEST_F(SliceTest, test_three_dimensions_same_dim_sub_slice)
{
    ParentType<3> p(50);
    Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionSize<DimensionA>(10))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionSize<DimensionB>(3))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionSize<DimensionC>(5))
                                              );
    // cut a sub slice (should be of the same type - hence no auto
    Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> sub_slice =
                    slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionIndex<DimensionA>(6)));

    ASSERT_EQ(5U, static_cast<std::size_t>(sub_slice.size<DimensionA>()));
    ASSERT_EQ(3U, static_cast<std::size_t>(sub_slice.size<DimensionB>()));
    ASSERT_EQ(5U, static_cast<std::size_t>(sub_slice.size<DimensionC>()));

    for(DimensionIndex<DimensionA> i(0); i < sub_slice.size<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < sub_slice.size<DimensionB>(); ++j) {
            for(DimensionIndex<DimensionC> k(0); k < sub_slice.size<DimensionC>(); ++k) {
            ASSERT_EQ(sub_slice[i][j][k],
                          ((std::size_t)i + 1U + 2U) * static_cast<std::size_t>(p.size<DimensionB>()) * static_cast<std::size_t>(p.size<DimensionC>())
                        + ((std::size_t)j + 20U) * static_cast<std::size_t>(p.size<DimensionC>())
                        + (std::size_t)k + 2U) << "i=" << i << " j=" << j << " k=" << k; // check we can read
            }
        }
    }
}

TEST_F(SliceTest, test_three_dimensions_slice_iterators)
{
    ParentType<3> p(50);
    Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> sub_slice =
                    slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionIndex<DimensionA>(4)));

    auto it = sub_slice.begin();
    auto end = sub_slice.end();
    for(DimensionIndex<DimensionA> i(0); i < sub_slice.size<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < sub_slice.size<DimensionB>(); ++j) {
            for(DimensionIndex<DimensionC> k(0); k < sub_slice.size<DimensionC>(); ++k) {
                unsigned val = *it;
                ASSERT_EQ( val, sub_slice[i][j][k]) << "i=" << i << " j=" << j << " k=" << k; // check we can read
                ASSERT_FALSE(it == end) << "i=" << i << " j=" << j << " k=" << k << " end=" << *end << " it=" << *it;
                ++it;
            }
        }
    }
    ASSERT_TRUE(it == sub_slice.end()) << "end=" << *sub_slice.end() << " it=" << *it;
}

TEST_F(SliceTest, const_test_three_dimensions_slice_iterators)
{
    ParentType<3> const p(50);
    Slice<true, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    Slice<true, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> sub_slice =
                    slice.slice(DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(2), DimensionIndex<DimensionA>(4)));

    auto it = sub_slice.begin();
    static_assert(std::is_same<std::iterator_traits<decltype(it)>::iterator_category, std::forward_iterator_tag>::value, "expecting a forward iterator");
    auto it2 = sub_slice.cbegin();
    static_assert(std::is_same<std::iterator_traits<decltype(it2)>::iterator_category, std::forward_iterator_tag>::value, "expecting a forward iterator");
    for(DimensionIndex<DimensionA> i(0); i < sub_slice.size<DimensionA>(); ++i) {
        for(DimensionIndex<DimensionB> j(0); j < sub_slice.size<DimensionB>(); ++j) {
            for(DimensionIndex<DimensionC> k(0); k < sub_slice.size<DimensionC>(); ++k) {
                unsigned val = *it;
                ASSERT_EQ( val, sub_slice[i][j][k]) << "i=" << i << " j=" << j << " k=" << k; // check we can read
                ASSERT_FALSE(it == sub_slice.end()) << "i=" << i << " j=" << j << " k=" << k << " end=" << *sub_slice.end() << " it=" << *it;
                ASSERT_FALSE(it2 == sub_slice.cend()) << "i=" << i << " j=" << j << " k=" << k << " end=" << *sub_slice.cend() << " it2=" << *it2;
                ++it;
                ++it2;
            }
        }
    }
    ASSERT_TRUE(it == sub_slice.end()) << "end=" << *sub_slice.end() << " it=" << *it;
    ASSERT_TRUE(it2 == sub_slice.cend()) << "end=" << *sub_slice.cend() << " it2=" << *it2;
}

TEST_F(SliceTest, test_three_dimensions_iterators_diff)
{
    ParentType<3> p(50);
    Slice<true, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    auto it = slice.begin();
    auto it2 = slice.begin();
    // check inner loop
    std::size_t count = 0;
    while(it2 != slice.end()) {
        ASSERT_EQ(count, it2 - it) << count;
        ++count;
        ++it2;
    }
}

TEST_F(SliceTest, test_three_dimensions_equals)
{
    ParentType<3> const p(50);
    Slice<true, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    ParentType<3> p2(50);
    Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice_2(p2
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(2), DimensionIndex<DimensionC>(7))
    );
    ASSERT_EQ(slice, slice_2);

    // dirrent data
    slice_2[DimensionIndex<DimensionA>(0)][DimensionIndex<DimensionB>(1)][DimensionIndex<DimensionC>(2)] = 0;
    ASSERT_NE(slice_2[DimensionIndex<DimensionA>(0)][DimensionIndex<DimensionB>(1)][DimensionIndex<DimensionC>(2)]
            , slice[DimensionIndex<DimensionA>(0)][DimensionIndex<DimensionB>(1)][DimensionIndex<DimensionC>(2)]);
    ASSERT_FALSE(slice==slice_2);

}

TEST_F(SliceTest, test_has_dimension)
{
    typedef Slice<true, ParentType<1>, TestSliceMixin, DimensionA> TestSlice1d;
    static_assert(std::is_same<typename has_dimension<TestSlice1d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename has_dimension<TestSlice1d, DimensionB>::type, std::false_type>::value, "expecting false");
}

TEST_F(SliceTest, test_is_multi_dimension)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA> TestSlice1d;
    static_assert(std::is_same<typename has_exact_dimensions<TestSlice1d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename has_exact_dimensions<TestSlice1d, DimensionB>::type, std::false_type>::value, "expecting false");

    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;
    bool r = has_exact_dimensions<TestSlice2d, DimensionA, DimensionB>::value;
    static_assert(std::is_same<typename has_exact_dimensions<TestSlice2d, DimensionA, DimensionB>::type, std::true_type>::value, "expecting true");
    ASSERT_TRUE(r);
    r = has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionA, DimensionB>::value;
    ASSERT_TRUE(r);
    r = has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionA>::value;
    ASSERT_FALSE(r);
    r = has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionB>::value;
    ASSERT_FALSE(r);
    r = has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionB, DimensionA>::value;
    ASSERT_FALSE(r);
}

} // namespace test
} // namespace astrotypes
} // namespace pss
