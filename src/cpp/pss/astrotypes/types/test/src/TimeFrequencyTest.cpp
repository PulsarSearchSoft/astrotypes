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
#include "pss/astrotypes/types/test/TimeFrequencyTest.h"
#include "pss/astrotypes/types/TimeFrequency.h"


using namespace pss::astrotypes::units;

namespace pss {
namespace astrotypes {
namespace test {


TimeFrequencyTest::TimeFrequencyTest()
    : ::testing::Test()
{
}

TimeFrequencyTest::~TimeFrequencyTest()
{
}

void TimeFrequencyTest::SetUp()
{
}

void TimeFrequencyTest::TearDown()
{
}

TEST_F(TimeFrequencyTest, test_time_freq_instantiation)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    TimeFrequency<uint8_t> tf1(time_size, freq_size);
    ASSERT_EQ(time_size, tf1.size<Time>());
    ASSERT_EQ(freq_size, tf1.size<Frequency>());

    TimeFrequency<uint16_t> tf2(freq_size, time_size);
    ASSERT_EQ(time_size, tf2.size<Time>());
    ASSERT_EQ(freq_size, tf2.size<Frequency>());
}

TEST_F(TimeFrequencyTest, test_time_freq_spectrum)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    TimeFrequency<uint8_t> tf1(time_size, freq_size);

    for(unsigned spectrum_num =0; spectrum_num < (std::size_t)time_size; ++spectrum_num)
    {
        unsigned n=0;
        typename TimeFrequency<uint8_t>::Spectra s = tf1.spectrum(spectrum_num);
        std::for_each(s.begin(), s.end(), [&](uint8_t& v) { v = ++n; } );
        ASSERT_EQ(n, (unsigned)(freq_size));
    }

    // test assignment operator
    typename TimeFrequency<uint8_t>::Spectra s3 = tf1.spectrum(3);
    s3 = tf1.spectrum(5);
    ASSERT_TRUE(s3.begin() == tf1.spectrum(5).begin());

    // test spectrum() on const object
    TimeFrequency<uint8_t> const& tf2 = tf1;
    typename TimeFrequency<uint8_t>::ConstSpectra s2 = tf2.spectrum(3);
    s2 = tf2.spectrum(5);
}

TEST_F(TimeFrequencyTest, test_time_freq_slice)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    const TimeFrequency<uint8_t> tf1(time_size, freq_size);

    for(unsigned spectrum_num =0; spectrum_num < (std::size_t)time_size; ++spectrum_num)
    {
        unsigned n=0;
        auto s = tf1.slice(DimensionSpan<Time>(DimensionSize<Time>(spectrum_num)));
        std::for_each(s.cbegin(), s.cend(), [&](uint8_t) { ++n; } );
        ASSERT_EQ(n, (unsigned)(freq_size * spectrum_num));
        // a slice of a slice
        auto s2 = s.slice(DimensionSpan<Time>(DimensionSize<Time>(1)));
        ASSERT_EQ(s2.data_size(), (unsigned)(freq_size));
    }
}

TEST_F(TimeFrequencyTest, test_time_freq_slice_copy)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    const TimeFrequency<uint8_t> tf1(time_size, freq_size);
    auto s = tf1.slice(DimensionSpan<Time>(DimensionSize<Time>(5)));
    decltype(s) slice_copy = s;
    ASSERT_TRUE(slice_copy.parent() == s.parent());
}

TEST_F(TimeFrequencyTest, test_time_freq_channel)
{
    DimensionSize<Time> time_size(2);
    DimensionSize<Frequency> freq_size(10);
    TimeFrequency<int> tf1(time_size, freq_size);
    unsigned val=0;
    std::generate(tf1.begin(), tf1.end(), [&]() { return val++; });

    typedef typename TimeFrequency<int>::Channel Channel;
    for(unsigned channel_num=0; channel_num < (std::size_t)freq_size; ++ channel_num) {
        SCOPED_TRACE(channel_num);
        SCOPED_TRACE("channel_num:");
        Channel c = tf1.channel(channel_num);
        Channel c_copy = c;
        ASSERT_EQ(&*c.begin(), &*c_copy.begin());

        for(DimensionIndex<Time> num(0); num < time_size; ++num) {
            ASSERT_EQ(c[num], (int)((unsigned)num * freq_size) + channel_num) << num;
            c[num] += 1; // can we modify the value?
            c[num] -= 1;
        }
        ASSERT_EQ(c.size<Frequency>(), DimensionSize<Frequency>(1));
        ASSERT_EQ(c.size<Time>(), time_size);
        multiarray::SliceIterator<Channel, false> it=c.begin();
        multiarray::SliceIterator<Channel, true> it_const=c.cbegin();
        ASSERT_EQ(c[DimensionIndex<Time>(0)], *it);
        ASSERT_EQ(it, it_const);
        unsigned count=0;
        while(it != c.end()) {
            ASSERT_EQ(count * (std::size_t)freq_size + channel_num, *it) << " count=" << count;
            ++count;
            ++it;
        }
        ASSERT_EQ(count, (unsigned)(time_size));
        unsigned count2=0;
        std::for_each(c.cbegin(), c.cend(), [&](int const& val)
                                            {
                                                ASSERT_EQ(count2 * (std::size_t)freq_size + channel_num, val) << " count=" << count2;
                                                ++count2;
                                            } );
        ASSERT_EQ(count, (unsigned)(time_size));
    }

    TimeFrequency<int> const& tf2 = tf1;
    typename TimeFrequency<int>::ConstChannel c2 = tf2.channel(5);
    ASSERT_EQ(c2.size<Frequency>(), DimensionSize<Frequency>(1));
    ASSERT_EQ(c2.size<Time>(), time_size);
}

TEST_F(TimeFrequencyTest, test_time_freq_channel_begin_end)
{
    DimensionSize<Time> time_size(2);
    DimensionSize<Frequency> freq_size(10);
    TimeFrequency<int> tf1(time_size, freq_size);
    typename TimeFrequency<int>::Channel channel = tf1.channel(0);
    std::fill(channel.begin(), channel.end(), 0);
}

TEST_F(TimeFrequencyTest, test_freq_time_instantiation)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    FrequencyTime<uint8_t> tf1(time_size, freq_size);
    ASSERT_EQ(time_size, tf1.size<Time>());
    ASSERT_EQ(freq_size, tf1.size<Frequency>());

    FrequencyTime<uint16_t> tf2(freq_size, time_size);
    ASSERT_EQ(time_size, tf2.size<Time>());
    ASSERT_EQ(freq_size, tf2.size<Frequency>());
}

TEST_F(TimeFrequencyTest, test_freq_time_spectrum)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    FrequencyTime<uint8_t> ft1(time_size, freq_size);

    typename FrequencyTime<uint8_t>::Spectra s = ft1.spectrum(0);
    unsigned n=0;
    std::for_each(s.cbegin(), s.cend(), [&](uint8_t) { ++n; } );
    ASSERT_EQ(n, (unsigned)(freq_size));

    FrequencyTime<uint8_t> const& ft2 = ft1;
    typename FrequencyTime<uint8_t>::ConstSpectra s2 = ft2.spectrum(3);

    // verify spectra interface works for slices
    typename FrequencyTime<uint8_t>::SliceType slice = ft1.slice( DimensionSpan<Frequency>(DimensionIndex<Frequency>(2), DimensionSize<Frequency>(4))
                         , DimensionSpan<Time>(DimensionIndex<Time>(10), DimensionSize<Time>(10)));
    auto spectra = slice.spectrum(2);
    auto const& const_spectra = const_cast<decltype(slice) const&>(slice).spectrum(2);
    ASSERT_EQ(spectra.size<Frequency>(), DimensionSize<Frequency>(4));
    ASSERT_EQ(const_spectra.size<Frequency>(), DimensionSize<Frequency>(4));
}

TEST_F(TimeFrequencyTest, test_freq_time_channel)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    FrequencyTime<uint8_t> ft1(time_size, freq_size);

    typename FrequencyTime<uint8_t>::Channel c = ft1.channel(0);
    ASSERT_EQ(c.size<Frequency>(), DimensionSize<Frequency>(1));
    ASSERT_EQ(c.size<Time>(), time_size);
    std::fill(c.begin(), c.end(), 0);

    FrequencyTime<uint8_t> const& tf2 = ft1;
    typename FrequencyTime<uint8_t>::ConstChannel c2 = tf2.channel(5);
    ASSERT_EQ(c2.size<Frequency>(), DimensionSize<Frequency>(1));
    ASSERT_EQ(c2.size<Time>(), time_size);

    // verify channel interface works for slices
    typename FrequencyTime<uint8_t>::SliceType slice = ft1.slice( DimensionSpan<Frequency>(DimensionIndex<Frequency>(0), DimensionSize<Frequency>(2))
                         , DimensionSpan<Time>(DimensionIndex<Time>(10), DimensionSize<Time>(10)));
    auto channel = slice.channel(2);
    auto const& const_channel = const_cast<decltype(slice) const&>(slice).channel(2);
    ASSERT_EQ((std::size_t)channel.size<Time>(), 10U);
    ASSERT_EQ((std::size_t)const_channel.size<Time>(), 10U);
}

TEST_F(TimeFrequencyTest, test_time_freq_transpose)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    TimeFrequency<uint8_t> tf(time_size, freq_size);
    unsigned val=0;
    std::generate(tf.begin(), tf.end(), [&]() { return val++; });
    static_assert(has_exact_dimensions<decltype(tf), units::Time, units::Frequency>::value, "oh oh");
    static_assert(has_dimension<decltype(tf), units::Time>::value, "oh oh");

    // call the transpose constructor
    FrequencyTime<uint8_t> ft(tf);

    ASSERT_EQ(ft.dimension<Time>(), time_size);
    ASSERT_EQ(ft.dimension<Frequency>(), freq_size);
    for(DimensionIndex<Time> ti(0); ti < time_size; ++ti)
    {
        for(DimensionIndex<Frequency> fi(0); fi < freq_size; ++fi)
        {
            ASSERT_EQ(tf[ti][fi], ft[fi][ti]);
        }
    }
}

TEST_F(TimeFrequencyTest, test_freq_time_transpose)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    FrequencyTime<uint8_t> ft1(time_size, freq_size);
    unsigned val=0;
    std::generate(ft1.begin(), ft1.end(), [&]() { return val++; });

    // call the transpose constructor
    TimeFrequency<uint8_t> tf1(ft1);

    ASSERT_EQ(tf1.dimension<Time>(), time_size);
    ASSERT_EQ(tf1.dimension<Frequency>(), freq_size);
    for(DimensionIndex<Time> ti(0); ti < time_size; ++ti)
    {
        for(DimensionIndex<Frequency> fi(0); fi < freq_size; ++fi)
        {
            ASSERT_EQ(tf1[ti][fi], ft1[fi][ti]);
        }
    }
}


TEST_F(TimeFrequencyTest, test_time_freq_has_exact_dimensions)
{
    static_assert(std::is_same<std::true_type, typename has_exact_dimensions<TimeFrequency<double>, units::Time, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>, units::Frequency, units::Time>::type>::value, "expecting false");
}

TEST_F(TimeFrequencyTest, test_freq_time_has_exact_dimensions)
{
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>, units::Time, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::true_type, typename has_exact_dimensions<FrequencyTime<double>, units::Frequency, units::Time>::type>::value, "expecting true");
}

TEST_F(TimeFrequencyTest, test_time_freq_slice_has_dimension)
{
    static_assert(std::is_same<std::true_type, typename has_dimension<TimeFrequency<double>, units::Time>::type>::value, "expecting true");
    static_assert(std::is_same<std::true_type, typename has_dimension<TimeFrequency<double>, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::true_type, typename has_dimensions<TimeFrequency<double>, units::Time, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::true_type, typename has_dimensions<TimeFrequency<double>, units::Frequency, units::Time>::type>::value, "expecting true");
}

TEST_F(TimeFrequencyTest, test_freq_time_slice_has_dimension)
{
    static_assert(std::is_same<std::true_type, typename has_dimension<FrequencyTime<double>, units::Time>::type>::value, "expecting true");
    static_assert(std::is_same<std::true_type, typename has_dimension<FrequencyTime<double>, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::true_type, typename has_dimensions<FrequencyTime<double>, units::Time, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::true_type, typename has_dimensions<FrequencyTime<double>, units::Frequency, units::Time>::type>::value, "expecting true");
}

TEST_F(TimeFrequencyTest, test_time_freq_slice_has_exact_dimensions)
{
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>::Channel, units::Time, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>::Channel, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::true_type, typename has_exact_dimensions<TimeFrequency<double>::Channel, units::Time>::type>::value, "expecting true");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>::Spectra, units::Frequency, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>::Spectra, units::Time, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>::Spectra, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::true_type, typename has_exact_dimensions<TimeFrequency<double>::Spectra, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<TimeFrequency<double>::Spectra, units::Frequency, units::Time>::type>::value, "expecting false");
}

TEST_F(TimeFrequencyTest, test_freq_time_slice_has_exact_dimensions)
{
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>::Spectra, units::Time, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>::Spectra, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>::Spectra, units::Frequency, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::true_type, typename has_exact_dimensions<FrequencyTime<double>::Spectra, units::Frequency>::type>::value, "expecting true");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>::Channel, units::Time, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>::Channel, units::Frequency, units::Time>::type>::value, "expecting false");
    static_assert(std::is_same<std::false_type, typename has_exact_dimensions<FrequencyTime<double>::Channel, units::Frequency>::type>::value, "expecting false");
    static_assert(std::is_same<std::true_type, typename has_exact_dimensions<FrequencyTime<double>::Channel, units::Time>::type>::value, "expecting true");
}

} // namespace test
} // namespace astrotypes
} // namespace pss
