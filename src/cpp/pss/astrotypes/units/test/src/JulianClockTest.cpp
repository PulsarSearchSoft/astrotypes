/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 The SKA organisation
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
#include "pss/astrotypes/units/test/JulianClockTest.h"
#include "pss/astrotypes/units/JulianClock.h"


namespace pss {
namespace astrotypes {
namespace units {
namespace test {


JulianClockTest::JulianClockTest()
    : ::testing::Test()
{
}

JulianClockTest::~JulianClockTest()
{
}

void JulianClockTest::SetUp()
{
}

void JulianClockTest::TearDown()
{
}

TEST_F(JulianClockTest, test_julian_day)
{
    ASSERT_EQ(julian_day(1), std::chrono::hours(24));
    ASSERT_EQ(julian_day(2), std::chrono::hours(48));
}

TEST_F(JulianClockTest, test_time_since_epoch)
{
    std::chrono::time_point<JulianClock> epoc;
    ASSERT_EQ(julian_day(0), epoc.time_since_epoch());
    
    std::chrono::time_point<JulianClock> hundred = epoc + julian_day(100.0);
    ASSERT_EQ(julian_day(100.0), hundred.time_since_epoch());

    std::chrono::time_point<JulianClock> hundred_hours = epoc + std::chrono::duration_cast<julian_day>(std::chrono::hours(24));
    ASSERT_EQ(julian_day(1.0), hundred_hours.time_since_epoch());
}

TEST_F(JulianClockTest, test_time_point_conversion)
{
    typename JulianClock::time_point epoch(julian_day(2440587.5));
    std::chrono::time_point<std::chrono::system_clock> system_epoch;
    ASSERT_TRUE( static_cast<std::chrono::time_point<std::chrono::system_clock>>(epoch) == system_epoch);
    ASSERT_TRUE( static_cast<JulianClock::time_point>(system_epoch) == epoch);

}

TEST_F(JulianClockTest, test_cout)
{
    auto today = JulianClock::now();
    std::cout << "time now=" << today << "\n";
}

} // namespace test
} // namespace units
} // namespace astrotypes
} // namespace pss
