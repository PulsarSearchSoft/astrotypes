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
#include "pss/astrotypes/sigproc/test/SigProcFormatTest.h"
#include "pss/astrotypes/sigproc/SigProcFormat.h"
#include <algorithm>


namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {


SigProcFormatTest::SigProcFormatTest()
    : ::testing::Test()
{
}

SigProcFormatTest::~SigProcFormatTest()
{
}

void SigProcFormatTest::SetUp()
{
}

void SigProcFormatTest::TearDown()
{
}

TEST_F(SigProcFormatTest, test_time_frequency_tf_data)
{
    typedef SigProcFormat<units::Time, units::Frequency> TestType;
    TestType formatter;
    TimeFrequency<uint8_t> time_frequency(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    uint8_t n = 0;
    std::generate(time_frequency.begin(), time_frequency.end(), [&]() { return ++n;} );

    // write out the data to  the strea,
    std::stringstream ss;
    ss << formatter << time_frequency;

    // read in the data from the stream
    TimeFrequency<uint8_t> time_frequency_2(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    ss >> TestType() >> time_frequency_2;

    ASSERT_EQ(time_frequency, time_frequency_2);
}

TEST_F(SigProcFormatTest, test_time_frequency_ft_data)
{
    typedef SigProcFormat<units::Time, units::Frequency> TestType;
    TestType formatter;
    FrequencyTime<uint8_t> frequency_time(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    uint8_t n = 0;
    std::generate(frequency_time.begin(), frequency_time.end(), [&]() { return ++n;} );

    // write out the data to  the strea,
    std::stringstream ss;
    ss << formatter << frequency_time;
    // should also be able to handle Slices
    auto slice = frequency_time.channel(10);
    ss << formatter << slice << frequency_time.spectrum(6);

    // read in the data from the stream
    
    FrequencyTime<uint8_t> frequency_time_2(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    ss >> TestType() >> frequency_time_2;

    ASSERT_EQ(frequency_time, frequency_time_2);
}

TEST_F(SigProcFormatTest, test_frequency_time_tf_data)
{
    typedef SigProcFormat<units::Frequency, units::Time> TestType;
    TestType formatter;
    TimeFrequency<uint8_t> time_frequency(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    uint8_t n = 0;
    std::generate(time_frequency.begin(), time_frequency.end(), [&]() { return n + 1;} );

    // write out the data to  the strea,
    std::stringstream ss;
    ss << formatter << time_frequency;

    // read in the data from the stream
    TimeFrequency<uint8_t> time_frequency_2(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    ss >> TestType() >> time_frequency_2;

    ASSERT_EQ(time_frequency, time_frequency_2);
}

TEST_F(SigProcFormatTest, test_frequency_time_ft_data)
{
    typedef SigProcFormat<units::Frequency, units::Time> TestType;
    TestType formatter;
    FrequencyTime<uint8_t> frequency_time(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    uint8_t n = 0;
    std::generate(frequency_time.begin(), frequency_time.end(), [&]() { return n + 1;} );

    // write out the data to  the strea,
    std::stringstream ss;
    ss << formatter << frequency_time;
    
    FrequencyTime<uint8_t> frequency_time_2(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(20));
    ss >> TestType() >> frequency_time_2;

    ASSERT_EQ(frequency_time, frequency_time_2);
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
