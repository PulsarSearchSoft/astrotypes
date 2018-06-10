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
    
    typename TimeFrequency<uint8_t>::Spectra s = tf1.spectrum(0);
    unsigned n=0;
    std::for_each(s.cbegin(), s.cend(), [&](uint8_t) { ++n; } );
    ASSERT_EQ(n, (unsigned)(freq_size));

    TimeFrequency<uint8_t> const& tf2 = tf1;
    typename TimeFrequency<uint8_t>::ConstSpectra s2 = tf2.spectrum(3);
}

TEST_F(TimeFrequencyTest, test_time_freq_channel)
{
    DimensionSize<Time> time_size(2);
    DimensionSize<Frequency> freq_size(10);
    TimeFrequency<int> tf1(time_size, freq_size);
    unsigned val=0;
    std::generate(tf1.begin(), tf1.end(), [&]() { return val++; });
    
    typedef typename TimeFrequency<int>::Channel Channel; 
    Channel c = tf1.channel(0);
    ASSERT_EQ(c.size<Frequency>(), DimensionSize<Frequency>(1));
    ASSERT_EQ(c.size<Time>(), time_size);
    unsigned count=0;
    SliceIterator<Channel, false> it=c.begin();
    SliceIterator<Channel, true> it_const=c.cbegin();
    while(it != c.end()) {
        ASSERT_EQ(count * (std::size_t)freq_size, *it) << " count=" << count;
        ++count;
        ++it;
    }
    ASSERT_EQ(count, (unsigned)(time_size));

    ASSERT_EQ(&*tf1.cbegin(), &*c.begin());
    ASSERT_EQ(&*tf1.cbegin() + (std::size_t)freq_size * time_size, &*c.end()) << " tf data end=" << &*tf1.end();
    count = 0;
    std::for_each(c.cbegin(), c.cend(), [&](int const& val)
                                        { 
                                            ASSERT_EQ(count * (std::size_t)freq_size, val) << " count=" << count;
                                            ++count;
                                        } );
    ASSERT_EQ(count, (unsigned)(time_size));

    TimeFrequency<int> const& tf2 = tf1;
    typename TimeFrequency<int>::ConstChannel c2 = tf2.channel(5);
    ASSERT_EQ(c2.size<Frequency>(), DimensionSize<Frequency>(1));
    ASSERT_EQ(c2.size<Time>(), time_size);
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
    FrequencyTime<uint8_t> tf1(time_size, freq_size);
    
    typename FrequencyTime<uint8_t>::Spectra s = tf1.spectrum(0);
    unsigned n=0;
    std::for_each(s.cbegin(), s.cend(), [&](uint8_t) { ++n; } );
    ASSERT_EQ(n, (unsigned)(freq_size));

    FrequencyTime<uint8_t> const& tf2 = tf1;
    typename FrequencyTime<uint8_t>::ConstSpectra s2 = tf2.spectrum(3);
}

TEST_F(TimeFrequencyTest, test_freq_time_channel)
{
    DimensionSize<Time> time_size(50);
    DimensionSize<Frequency> freq_size(10);
    FrequencyTime<uint8_t> tf1(time_size, freq_size);
    
    typename FrequencyTime<uint8_t>::Channel c = tf1.channel(0);
    ASSERT_EQ(c.size<Frequency>(), DimensionSize<Frequency>(0));
    ASSERT_EQ(c.size<Time>(), time_size);

    FrequencyTime<uint8_t> const& tf2 = tf1;
    typename FrequencyTime<uint8_t>::ConstChannel c2 = tf2.channel(5);
    ASSERT_EQ(c2.size<Frequency>(), DimensionSize<Frequency>(0));
    ASSERT_EQ(c2.size<Time>(), time_size);
}

} // namespace test
} // namespace astrotypes
} // namespace pss
