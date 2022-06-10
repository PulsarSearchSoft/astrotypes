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

namespace multiarray {
namespace test {

template<int NDim, typename DimTuple>
struct ParentType;

} // namespace multiarray
} // namespace test

template<typename Dimension, typename DimTuple, int NDim>
struct has_dimension<multiarray::test::ParentType<NDim, DimTuple>, Dimension> : public has_type<DimTuple, Dimension>::type
{
};

namespace multiarray {
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
typedef std::tuple<DimensionA, DimensionB, DimensionC> DimensionTuple;

template<typename T>
class TestSliceMixin : public T
{
    public:
        TestSliceMixin() {}
        TestSliceMixin(T const& t) : T(t) {}
        using T::T;
};

template<int NDim, typename DimTuple=typename tuple_slice<DimensionTuple, 0, NDim-1>::type>
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

    // test copy assignment operator
    it = slice.begin();
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

TEST_F(SliceTest, test_single_dimension_iterators_plus_equal)
{
    ParentType<1> const p(30);
    Slice<true, ParentType<1>, TestSliceMixin, DimensionA> slice(p, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20)));

    auto const it = slice.begin();
    auto it2 = slice.begin();
    for(unsigned i=0; i < slice.size<DimensionA>(); ++i) {
        auto it3 = it;
        it3 += i; // apply the plus equal poperator
        ASSERT_EQ(&*it2, &*it3) << i;
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

TEST_F(SliceTest, test_one_dimensions_slice_copy)
{
    ParentType<1> p(50);
    const ParentType<1> const_p(50);
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              );
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> slice_copy = slice;
    ASSERT_EQ(&slice_copy.parent(), &slice.parent());

    Slice<true, ParentType<1>, TestSliceMixin, DimensionA> const_slice(const_p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              );
    Slice<true, ParentType<1>, TestSliceMixin, DimensionA> const_slice_copy = const_slice;
    ASSERT_EQ(&const_slice_copy.parent(), &const_slice.parent());
}

TEST_F(SliceTest, test_singe_dimension_slice_iterators_copy)
{
    ParentType<1> p(50);
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              );
    auto it = slice.begin();
    decltype(it) copy_it = it;
    ASSERT_TRUE(copy_it == it);

    auto const_it = slice.cbegin();
    decltype(const_it) copy_const_it = const_it;
    ASSERT_TRUE(copy_const_it == const_it);
}

TEST_F(SliceTest, test_one_dimensions_empty_constructor)
{
    ParentType<1> p(50);
    Slice<false, ParentType<1>, TestSliceMixin, DimensionA> empty_slice;
    ASSERT_EQ(&empty_slice.parent(), nullptr);
    ASSERT_EQ(empty_slice.data_size(), 0U);
    ASSERT_EQ(empty_slice.template dimension<DimensionA>(), DimensionSize<DimensionA>(0U));
    ASSERT_TRUE(empty_slice.begin() == empty_slice.end());
    ASSERT_TRUE(empty_slice.cbegin() == empty_slice.cend());
}

TEST_F(SliceTest, test_two_dimensions_empty_constructor)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> empty_slice;
    ASSERT_EQ(&empty_slice.parent(), nullptr);
    ASSERT_EQ(empty_slice.template dimension<DimensionA>(), DimensionSize<DimensionA>(0U));
    ASSERT_EQ(empty_slice.template dimension<DimensionB>(), DimensionSize<DimensionB>(0U));
    ASSERT_EQ(empty_slice.data_size(), 0U);
    ASSERT_TRUE(empty_slice.begin() == empty_slice.end());
    ASSERT_TRUE(empty_slice.cbegin() == empty_slice.cend());
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
    // check parent()
    ASSERT_EQ(&p, &slice.parent());
    ASSERT_EQ(&p, &const_slice.parent());

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

TEST_F(SliceTest, test_two_dimensions_inner_one_dimensional_slice)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice_a(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(19))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(22))
                                              );

    for(DimensionIndex<DimensionA> index_a(0); index_a < slice_a.template dimension<DimensionA>(); ++index_a)
    {
        auto slice_1D = slice_a[index_a];
        static_assert(std::is_same<has_dimension<typename decltype(slice_1D)::Parent, DimensionB>
                                 , has_dimension<typename decltype(slice_a)::Parent, DimensionB>>::value, "parent type not as expected");
        static_assert(std::is_same<has_dimension<typename decltype(slice_1D)::Parent, DimensionA>
                                 , has_dimension<typename decltype(slice_a)::Parent, DimensionA>>::value, "parent type not as expected");
        static_assert(is_slice<decltype(slice_1D)>::value, "expecting slice");
        ASSERT_EQ(slice_1D.template dimension<DimensionB>(), slice_a.template dimension<DimensionB>());
        static_assert(has_dimension<typename decltype(slice_1D)::Parent, DimensionA>::value, " type");
        static_assert(has_dimension<ParentType<2>, DimensionA>::value, " type");
        ASSERT_EQ(slice_1D.template dimension<DimensionA>(), DimensionSize<DimensionA>(1));

        // further slices
        auto& slice_a_of_slice = slice_1D[DimensionIndex<DimensionA>(0)];
        static_assert(std::is_same<typename std::remove_reference<decltype(slice_a_of_slice)>::type, decltype(slice_1D)>::value, "expecting same type");
        ASSERT_EQ(&slice_1D, &slice_a_of_slice);

        auto& slice_b_of_slice = slice_1D[DimensionIndex<DimensionB>(0)];
        static_assert(std::is_same<typename std::remove_reference<decltype(slice_b_of_slice)>::type, int>::value, "expecting an int&");
        ASSERT_EQ(slice_b_of_slice, *slice_1D.begin());
    }
    for(DimensionIndex<DimensionB> index_b(0); index_b < slice_a.template dimension<DimensionB>(); ++index_b)
    {
        auto slice_1D = slice_a[index_b];
        static_assert(std::is_same<has_dimension<typename decltype(slice_1D)::Parent, DimensionB>
                                 , has_dimension<typename decltype(slice_a)::Parent, DimensionB>>::value, "parent type not as expected");
        static_assert(std::is_same<has_dimension<typename decltype(slice_1D)::Parent, DimensionA>
                                 , has_dimension<typename decltype(slice_a)::Parent, DimensionA>>::value, "parent type not as expected");
        static_assert(is_slice<decltype(slice_1D)>::value, "expecting slice");
        ASSERT_EQ(slice_1D.template dimension<DimensionB>(), DimensionSize<DimensionB>(1));
        ASSERT_EQ(slice_1D.template dimension<DimensionA>(), slice_a.template dimension<DimensionA>());

        // further slices
        auto& slice_b_of_slice = slice_1D[DimensionIndex<DimensionB>(0)];
        ASSERT_EQ(&slice_1D, &slice_b_of_slice); // can't further divide by type so just returns the same object
        auto& slice_a_of_slice = slice_1D[DimensionIndex<DimensionA>(0)];
        static_assert(std::is_same<typename std::remove_reference<decltype(slice_a_of_slice)>::type, int>::value, "expecting an int&");
        ASSERT_EQ(slice_a_of_slice, *slice_1D.begin());
    }
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

    // test copy assignment operator
    it = slice.begin();
}

TEST_F(SliceTest, test_two_dimensions_slice_copy)
{
    ParentType<2> p(50);
    const ParentType<2> const_p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice_copy = slice;
    ASSERT_EQ(&slice_copy.parent(), &slice.parent());

    Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> const_slice(const_p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> const_slice_copy = const_slice;
    ASSERT_EQ(&const_slice_copy.parent(), &const_slice.parent());
}


TEST_F(SliceTest, test_two_dimensions_slice_iterators_copy)
{
    ParentType<2> p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );
    auto it = slice.begin();
    decltype(it) copy_it = it;
    ASSERT_TRUE(it == copy_it);

    auto const_it = slice.cbegin();
    decltype(const_it) copy_const_it = const_it;
    ASSERT_TRUE(const_it == copy_const_it);
}

TEST_F(SliceTest, test_two_dimension_iterators_plus_equal)
{
    ParentType<2> const p(50);
    Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );

    auto const it = slice.begin();
    auto it2 = slice.begin();

    // check with simple std::size_t
    for(std::size_t i=0; i < slice.data_size(); ++i) {
        auto it3 = it;
        it3 += i; // apply the plus equal operator
        ASSERT_EQ(&*it2, &*it3) << i << "begin=" << &*it;
        ++it2;
    }
}

TEST_F(SliceTest, test_two_dimensions_slice_iterators_overlay)
{
    ParentType<2> p(50);
    ParentType<2> overlay_p(50);
    Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(10), DimensionIndex<DimensionA>(20))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              );

    auto it = slice.begin();
    auto const_it = slice.cbegin();
    ASSERT_EQ(&*it(overlay_p), &*overlay_p.begin() + 10 * overlay_p.size<DimensionB>() + 20);
    ASSERT_EQ(&*const_it(p), &*p.begin() + 10 * p.size<DimensionB>() + 20);
    ASSERT_EQ(&*const_it(overlay_p), &*overlay_p.begin() + 10 * p.size<DimensionB>() + 20) << "overlay begin=" << &*overlay_p.begin();

    // if of a different type
    ParentType<2, double> overlay_different_type(50);
    ASSERT_EQ(&*const_it(overlay_different_type), &*overlay_different_type.begin() + 10 * p.size<DimensionB>() + 20);
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

TEST_F(SliceTest, test_three_dimension_iterators_plus_equal)
{
    ParentType<3> p(50);
    Slice<true, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> slice(p
                                              , DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(1), DimensionIndex<DimensionA>(11))
                                              , DimensionSpan<DimensionB>(DimensionIndex<DimensionB>(20), DimensionIndex<DimensionB>(23))
                                              , DimensionSpan<DimensionC>(DimensionIndex<DimensionC>(4), DimensionIndex<DimensionC>(7))
    );

    auto const it = slice.begin();
    auto it2 = slice.begin();

    // check with simple std::size_t
    for(std::size_t i=0; i < slice.data_size(); ++i) {
        auto it3 = it;
        it3 += i; // apply the plus equal operator
        ASSERT_EQ(&*it2, &*it3) << i << "begin=" << &*it;
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
    static_assert(std::is_same<typename astrotypes::has_dimension<TestSlice1d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename astrotypes::has_dimension<TestSlice1d, DimensionB>::type, std::false_type>::value, "expecting false");
    static_assert(TestSlice1d::has_dimension<DimensionA>(), "expecting true");

    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;
    static_assert(TestSlice2d::has_dimension<DimensionA>(), "expecting true");
    static_assert(TestSlice2d::has_dimension<DimensionB>(), "expecting true");
    static_assert(!TestSlice2d::has_dimension<DimensionC>(), "expecting not to find C");
    static_assert(std::is_same<typename astrotypes::has_dimension<TestSlice2d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename astrotypes::has_dimension<TestSlice2d, DimensionB>::type, std::true_type>::value, "expecting false");
    static_assert(std::is_same<typename astrotypes::has_dimension<TestSlice2d, DimensionC>::type, std::false_type>::value, "expecting false");

    typedef TestSliceMixin<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MTestSlice2d;
    static_assert(MTestSlice2d::has_dimension<DimensionA>(), "expecting true");
    static_assert(MTestSlice2d::has_dimension<DimensionB>(), "expecting true");
    static_assert(!MTestSlice2d::has_dimension<DimensionC>(), "expecting not to find C");
    static_assert(std::is_same<typename astrotypes::has_dimension<MTestSlice2d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename astrotypes::has_dimension<MTestSlice2d, DimensionB>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename astrotypes::has_dimension<MTestSlice2d, DimensionC>::type, std::false_type>::value, "expecting false");
}

TEST_F(SliceTest, test_flip_const_1d)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA> ConstTestSlice1d;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA> TestSlice1d;
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice2d;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;

    TestSlice1d test_slice_1d;
    static_assert(is_slice<TestSlice1d>::value, "expecting a slice");
    ConstTestSlice1d& const_test_slice_1d = multiarray::flip_const(test_slice_1d);
    TestSlice1d& test_slice_1d_ref = multiarray::flip_const(const_test_slice_1d);
    ASSERT_EQ((void*)&test_slice_1d_ref, (void*)&const_test_slice_1d);
}

TEST_F(SliceTest, test_flip_const_1d_const)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA> ConstTestSlice1d;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA> TestSlice1d;
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice2d;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;

    const TestSlice1d test_slice_1d;
    ConstTestSlice1d const& const_test_slice_1d = multiarray::flip_const(test_slice_1d);
    TestSlice1d const& test_slice_1d_ref = multiarray::flip_const(const_test_slice_1d);
    ASSERT_EQ((void*)&test_slice_1d_ref, (void*)&const_test_slice_1d);
}

TEST_F(SliceTest, test_flip_const_2d)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice2d;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;

    TestSlice2d test_slice_2d;
    static_assert(is_slice<TestSlice2d>::value, "expecting a slice");
    ConstTestSlice2d& const_test_slice_2d = multiarray::flip_const(test_slice_2d);
    TestSlice2d& test_slice_2d_ref = multiarray::flip_const(const_test_slice_2d);
    ASSERT_EQ((void*)&test_slice_2d_ref, (void*)&const_test_slice_2d);
}

TEST_F(SliceTest, test_flip_const_2d_reduced_slice)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice2d;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;
    typedef TestSliceMixin<Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> TestSliceMixin2d;

    TestSlice2d test_slice_2d;
    typename TestSlice2d::OperatorSliceType<DimensionA>::type reduced_slice = test_slice_2d[DimensionIndex<DimensionA>(0)];
    typename TestSlice2d::ConstOperatorSliceType<DimensionA>::type& const_reduced_slice_2d = multiarray::flip_const(reduced_slice);
    typename TestSlice2d::OperatorSliceType<DimensionA>::type& reduced_slice_2d_ref = multiarray::flip_const(const_reduced_slice_2d);
    ASSERT_EQ((void*)&reduced_slice_2d_ref, (void*)&reduced_slice);

    typename TestSlice2d::OperatorSliceType<DimensionB>::type reduced_slice_b = test_slice_2d[DimensionIndex<DimensionB>(0)];
    typename TestSlice2d::ConstOperatorSliceType<DimensionB>::type& const_reduced_slice_b = multiarray::flip_const(reduced_slice_b);
    typename TestSlice2d::OperatorSliceType<DimensionB>::type reduced_slice_b_ref = multiarray::flip_const(const_reduced_slice_b);
    ASSERT_EQ((void*)&const_reduced_slice_b, (void*)&reduced_slice_b);

    TestSliceMixin2d test_slice_mixin_2d;
    typename TestSliceMixin2d::OperatorSliceType<DimensionA>::type reduced_slice_mixin = test_slice_mixin_2d[DimensionIndex<DimensionA>(0)];
    typename TestSliceMixin2d::ConstOperatorSliceType<DimensionA>::type& const_reduced_slice_mixin_2d = multiarray::flip_const(reduced_slice_mixin);
    typename TestSliceMixin2d::OperatorSliceType<DimensionA>::type& reduced_slice_mixin_2d_ref = multiarray::flip_const(const_reduced_slice_mixin_2d);
    ASSERT_EQ((void*)&reduced_slice_mixin_2d_ref, (void*)&reduced_slice_mixin);

    typedef TestSliceMixin2d::OperatorSliceType<DimensionA>::type::OperatorSliceType<DimensionB>::type ReducedReducedType;
    typedef TestSliceMixin2d::ConstOperatorSliceType<DimensionA>::type::ConstOperatorSliceType<DimensionB>::type ConstReducedReducedType;
    ReducedReducedType reduced_2_slice_mixin = test_slice_mixin_2d[DimensionIndex<DimensionA>(0)][DimensionIndex<DimensionB>(0)];;
    ConstReducedReducedType& const_reduced_2_slice_mixin_2d = multiarray::flip_const(reduced_2_slice_mixin);
    static_assert(std::is_same<ConstReducedReducedType, int const&>::value, "oh oh");
    static_assert(std::is_same<ReducedReducedType, int&>::value, "oh oh");
    ReducedReducedType& reduced_2_slice_mixin_ref = multiarray::flip_const(const_reduced_2_slice_mixin_2d);
    ASSERT_EQ(&const_reduced_2_slice_mixin_2d, &reduced_2_slice_mixin_ref);

}

TEST_F(SliceTest, test_flip_const_3d_reduced_slice)
{
    typedef Slice<true, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> ConstTestSlice3d;
    typedef Slice<false, ParentType<3>, TestSliceMixin, DimensionA, DimensionB, DimensionC> TestSlice3d;

    ParentType<3> parent(20);
    TestSlice3d test_slice_3d(parent, DimensionSpan<DimensionA>(DimensionIndex<DimensionA>(0), DimensionSize<DimensionA>(10)));
    typename TestSlice3d::OperatorSliceType<DimensionA>::type reduced_slice = test_slice_3d[DimensionIndex<DimensionA>(0)];
    typename TestSlice3d::ConstOperatorSliceType<DimensionA>::type& const_reduced_slice_3d = multiarray::flip_const(reduced_slice);
    typename TestSlice3d::OperatorSliceType<DimensionA>::type& reduced_slice_3d_ref = multiarray::flip_const(const_reduced_slice_3d);
    ASSERT_EQ((void*)&reduced_slice_3d_ref, (void*)&reduced_slice);

    typename TestSlice3d::OperatorSliceType<DimensionB>::type reduced_slice_b = test_slice_3d[DimensionIndex<DimensionB>(0)];
    typename TestSlice3d::ConstOperatorSliceType<DimensionB>::type& const_reduced_slice_b = multiarray::flip_const(reduced_slice_b);
    typename TestSlice3d::OperatorSliceType<DimensionB>::type reduced_slice_b_ref = multiarray::flip_const(const_reduced_slice_b);
    ASSERT_EQ((void*)&const_reduced_slice_b, (void*)&reduced_slice_b);

    typename TestSlice3d::OperatorSliceType<DimensionC>::type reduced_slice_c = test_slice_3d[DimensionIndex<DimensionC>(0)];
    typename TestSlice3d::ConstOperatorSliceType<DimensionC>::type& const_reduced_slice_c = multiarray::flip_const(reduced_slice_c);
    typename TestSlice3d::OperatorSliceType<DimensionC>::type reduced_slice_c_ref = multiarray::flip_const(const_reduced_slice_c);
    ASSERT_EQ((void*)&const_reduced_slice_c, (void*)&reduced_slice_c);

    /*
    typename TestSlice3d::OperatorSliceType<DimensionB>::type::OperatorSliceType<DimensionC>::type reduced_slice_b_c = test_slice_3d[DimensionIndex<DimensionB>(0)][DimensionIndex<DimensionC>(2)];
    typename TestSlice3d::ConstOperatorSliceType<DimensionB>::type::ConstOperatorSliceType<DimensionC>::type& const_reduced_slice_b_c = multiarray::flip_const(reduced_slice_b_c);
    typename TestSlice3d::OperatorSliceType<DimensionC>::type reduced_slice_b_c_ref = multiarray::flip_const(const_reduced_slice_b_c);
    ASSERT_EQ((void*)&const_reduced_slice_b_c, (void*)&reduced_slice_b_c);
    */
}

TEST_F(SliceTest, is_const)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice;
    static_assert(ConstTestSlice::is_const(), "expecting const");
    typedef TestSliceMixin<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MixinConstTestSlice;
    static_assert(MixinConstTestSlice::is_const(), "expecting const");
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice;
    static_assert(!TestSlice::is_const(), "expecting non const");
    typedef TestSliceMixin<Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MixinTestSlice;
    static_assert(!MixinTestSlice::is_const(), "expecting non const");
}

TEST_F(SliceTest, test_cast_to_non_const_slice_2d)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice;
    typedef ConstTestSlice::OperatorSliceType<DimensionA>::type ReducedAConstSliceType;
    typedef ConstTestSlice::OperatorSliceType<DimensionB>::type ReducedBConstSliceType;
    typedef TestSliceMixin<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MixinConstTestSlice;
    typedef MixinConstTestSlice::OperatorSliceType<DimensionA>::type ReducedAMixinConstSliceType;
    typedef MixinConstTestSlice::OperatorSliceType<DimensionB>::type ReducedBMixinConstSliceType;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice;
    typedef TestSlice::OperatorSliceType<DimensionA>::type ReducedASliceType;
    typedef TestSlice::OperatorSliceType<DimensionB>::type ReducedBSliceType;
    typedef TestSliceMixin<Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MixinTestSlice;
    typedef MixinTestSlice::OperatorSliceType<DimensionA>::type ReducedAMixinSliceType;
    typedef MixinTestSlice::OperatorSliceType<DimensionB>::type ReducedBMixinSliceType;

    { // cast from Const types
        ConstTestSlice slice;
        TestSlice& slice_ref =cast_to_non_const_slice(slice);
        ASSERT_EQ((void*)&slice_ref, (void*)&slice);

        MixinConstTestSlice mixin_slice;
        MixinTestSlice& mixin_slice_ref = cast_to_non_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedAConstSliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedASliceType& const_reduced_A_slice = cast_to_non_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBConstSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBSliceType& const_reduced_B_slice = cast_to_non_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);

        ReducedAMixinConstSliceType mixin_reduced_A_slice = mixin_slice[DimensionIndex<DimensionA>(0)];
        ReducedAMixinSliceType& const_mixin_reduced_A_slice = cast_to_non_const_slice(mixin_reduced_A_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_A_slice, (void*)&mixin_reduced_A_slice);

        ReducedBMixinConstSliceType mixin_reduced_B_slice = mixin_slice[DimensionIndex<DimensionB>(0)];
        ReducedBMixinSliceType& const_mixin_reduced_B_slice = cast_to_non_const_slice(mixin_reduced_B_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_B_slice, (void*)&mixin_reduced_B_slice);
    }
    { // cast from const Const types
        ConstTestSlice const slice;
        TestSlice const& slice_ref =cast_to_non_const_slice(slice);
        ASSERT_EQ((void*)&slice_ref, (void*)&slice);

        MixinConstTestSlice const mixin_slice;
        MixinTestSlice const& mixin_slice_ref = cast_to_non_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedAConstSliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedASliceType& const_reduced_A_slice = cast_to_non_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBConstSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBSliceType& const_reduced_B_slice = cast_to_non_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);

        ReducedAMixinConstSliceType mixin_reduced_A_slice = mixin_slice[DimensionIndex<DimensionA>(0)];
        ReducedAMixinSliceType& const_mixin_reduced_A_slice = cast_to_non_const_slice(mixin_reduced_A_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_A_slice, (void*)&mixin_reduced_A_slice);

        ReducedBMixinConstSliceType mixin_reduced_B_slice = mixin_slice[DimensionIndex<DimensionB>(0)];
        ReducedBMixinSliceType& const_mixin_reduced_B_slice = cast_to_non_const_slice(mixin_reduced_B_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_B_slice, (void*)&mixin_reduced_B_slice);
    }
    { // cast from non Const types
        TestSlice slice;
        TestSlice& slice_ref=cast_to_non_const_slice(slice);
        ASSERT_EQ((void*)&slice_ref, (void*)&slice);

        MixinTestSlice mixin_slice;
        MixinTestSlice& mixin_slice_ref = cast_to_non_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedASliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedASliceType& reduced_A_slice_ref = cast_to_non_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&reduced_A_slice_ref, (void*)&reduced_A_slice);

        ReducedBSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBSliceType& reduced_B_slice_ref = cast_to_non_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&reduced_B_slice_ref, (void*)&reduced_B_slice);

        ReducedAMixinSliceType mixin_reduced_A_slice = mixin_slice[DimensionIndex<DimensionA>(0)];
        ReducedAMixinSliceType& mixin_reduced_A_slice_ref = cast_to_non_const_slice(mixin_reduced_A_slice);
        ASSERT_EQ((void*)&mixin_reduced_A_slice_ref, (void*)&mixin_reduced_A_slice);

        ReducedBMixinSliceType mixin_reduced_B_slice = mixin_slice[DimensionIndex<DimensionB>(0)];
        ReducedBMixinSliceType& mixin_reduced_B_slice_ref = cast_to_non_const_slice(mixin_reduced_B_slice);
        ASSERT_EQ((void*)&mixin_reduced_B_slice_ref, (void*)&mixin_reduced_B_slice);
    }
    { // cast from const non Const types
        TestSlice const slice;
        TestSlice const& const_slice_ref=cast_to_non_const_slice(slice);
        ASSERT_EQ((void*)&const_slice_ref, (void*)&slice);

        MixinTestSlice const mixin_slice;
        MixinTestSlice const& mixin_slice_ref = cast_to_non_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedAConstSliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedASliceType& const_reduced_A_slice = cast_to_non_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBConstSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBSliceType& const_reduced_B_slice = cast_to_non_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);
    }
}

TEST_F(SliceTest, test_cast_to_const_slice_2d)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> ConstTestSlice;
    typedef ConstTestSlice::OperatorSliceType<DimensionA>::type ReducedAConstSliceType;
    typedef ConstTestSlice::OperatorSliceType<DimensionB>::type ReducedBConstSliceType;
    typedef TestSliceMixin<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MixinConstTestSlice;
    typedef MixinConstTestSlice::OperatorSliceType<DimensionA>::type ReducedAMixinConstSliceType;
    typedef MixinConstTestSlice::OperatorSliceType<DimensionB>::type ReducedBMixinConstSliceType;
    typedef Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice;
    typedef TestSlice::OperatorSliceType<DimensionA>::type ReducedASliceType;
    typedef TestSlice::OperatorSliceType<DimensionB>::type ReducedBSliceType;
    typedef TestSliceMixin<Slice<false, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>> MixinTestSlice;
    typedef MixinTestSlice::OperatorSliceType<DimensionA>::type ReducedAMixinSliceType;
    typedef MixinTestSlice::OperatorSliceType<DimensionB>::type ReducedBMixinSliceType;

    { // cast from Const types
        ConstTestSlice slice;
        ConstTestSlice& slice_ref =cast_to_const_slice(slice);
        ASSERT_EQ((void*)&slice_ref, (void*)&slice);

        MixinConstTestSlice mixin_slice;
        MixinConstTestSlice& mixin_slice_ref = cast_to_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedAConstSliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedAConstSliceType& const_reduced_A_slice = cast_to_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBConstSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBConstSliceType& const_reduced_B_slice = cast_to_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);

        ReducedAMixinConstSliceType mixin_reduced_A_slice = mixin_slice[DimensionIndex<DimensionA>(0)];
        ReducedAMixinConstSliceType& const_mixin_reduced_A_slice = cast_to_const_slice(mixin_reduced_A_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_A_slice, (void*)&mixin_reduced_A_slice);

        ReducedBMixinConstSliceType mixin_reduced_B_slice = mixin_slice[DimensionIndex<DimensionB>(0)];
        ReducedBMixinConstSliceType& const_mixin_reduced_B_slice = cast_to_const_slice(mixin_reduced_B_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_B_slice, (void*)&mixin_reduced_B_slice);
    }
    { // cast from const Const types
        ConstTestSlice const slice;
        ConstTestSlice const& slice_ref =cast_to_const_slice(slice);
        ASSERT_EQ((void*)&slice_ref, (void*)&slice);

        MixinConstTestSlice const mixin_slice;
        MixinConstTestSlice const& mixin_slice_ref = cast_to_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedAConstSliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedAConstSliceType& const_reduced_A_slice = cast_to_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBConstSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBConstSliceType& const_reduced_B_slice = cast_to_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);

        ReducedAMixinConstSliceType mixin_reduced_A_slice = mixin_slice[DimensionIndex<DimensionA>(0)];
        ReducedAMixinConstSliceType& const_mixin_reduced_A_slice = cast_to_const_slice(mixin_reduced_A_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_A_slice, (void*)&mixin_reduced_A_slice);

        ReducedBMixinConstSliceType mixin_reduced_B_slice = mixin_slice[DimensionIndex<DimensionB>(0)];
        ReducedBMixinConstSliceType& const_mixin_reduced_B_slice = cast_to_const_slice(mixin_reduced_B_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_B_slice, (void*)&mixin_reduced_B_slice);
    }
    { // cast from non Const types
        TestSlice slice;
        ConstTestSlice& const_slice_ref=cast_to_const_slice(slice);
        ASSERT_EQ((void*)&const_slice_ref, (void*)&slice);

        MixinTestSlice mixin_slice;
        MixinConstTestSlice& mixin_slice_ref = cast_to_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedASliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedAConstSliceType& const_reduced_A_slice = cast_to_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBConstSliceType& const_reduced_B_slice = cast_to_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);

        ReducedAMixinSliceType mixin_reduced_A_slice = mixin_slice[DimensionIndex<DimensionA>(0)];
        ReducedAMixinConstSliceType& const_mixin_reduced_A_slice = cast_to_const_slice(mixin_reduced_A_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_A_slice, (void*)&mixin_reduced_A_slice);

        ReducedBMixinSliceType mixin_reduced_B_slice = mixin_slice[DimensionIndex<DimensionB>(0)];
        ReducedBMixinConstSliceType& const_mixin_reduced_B_slice = cast_to_const_slice(mixin_reduced_B_slice);
        ASSERT_EQ((void*)&const_mixin_reduced_B_slice, (void*)&mixin_reduced_B_slice);
    }
    { // cast from const non Const types
        TestSlice const slice;
        ConstTestSlice const& const_slice_ref=cast_to_const_slice(slice);
        ASSERT_EQ((void*)&const_slice_ref, (void*)&slice);

        MixinTestSlice const mixin_slice;
        MixinConstTestSlice const& mixin_slice_ref = cast_to_const_slice(mixin_slice);
        ASSERT_EQ((void*)&mixin_slice_ref, (void*)&mixin_slice);

        ReducedAConstSliceType reduced_A_slice = slice[DimensionIndex<DimensionA>(0)];
        ReducedAConstSliceType& const_reduced_A_slice = cast_to_const_slice(reduced_A_slice);
        ASSERT_EQ((void*)&const_reduced_A_slice, (void*)&reduced_A_slice);

        ReducedBConstSliceType reduced_B_slice = slice[DimensionIndex<DimensionB>(0)];
        ReducedBConstSliceType& const_reduced_B_slice = cast_to_const_slice(reduced_B_slice);
        ASSERT_EQ((void*)&const_reduced_B_slice, (void*)&reduced_B_slice);
    }
}

TEST_F(SliceTest, test_is_multi_dimension)
{
    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA> TestSlice1d;
    static_assert(std::is_same<typename astrotypes::has_exact_dimensions<TestSlice1d, DimensionA>::type, std::true_type>::value, "expecting true");
    static_assert(std::is_same<typename astrotypes::has_exact_dimensions<TestSlice1d, DimensionB>::type, std::false_type>::value, "expecting false");

    typedef Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB> TestSlice2d;
    bool r = astrotypes::has_exact_dimensions<TestSlice2d, DimensionA, DimensionB>::value;
    static_assert(std::is_same<typename has_exact_dimensions<TestSlice2d, DimensionA, DimensionB>::type, std::true_type>::value, "expecting true");
    ASSERT_TRUE(r);
    r = astrotypes::has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionA, DimensionB>::value;
    ASSERT_TRUE(r);
    r = astrotypes::has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionA>::value;
    ASSERT_FALSE(r);
    r = astrotypes::has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionB>::value;
    ASSERT_FALSE(r);
    r = astrotypes::has_exact_dimensions<Slice<true, ParentType<2>, TestSliceMixin, DimensionA, DimensionB>, DimensionB, DimensionA>::value;
    ASSERT_FALSE(r);
}

} // namespace test
} // namespace multiarray
} // namespace astrotypes
} // namespace pss
