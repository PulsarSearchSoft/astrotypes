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
#include "pss/astrotypes/units/test/ModifiedJulianClockTest.h"
#include "pss/astrotypes/units/ModifiedJulianClock.h"
#include "pss/astrotypes/units/Time.h"
#include <ctime>


namespace pss {
namespace astrotypes {
namespace units {
namespace test {


ModifiedJulianClockTest::ModifiedJulianClockTest()
    : ::testing::Test()
{
}

ModifiedJulianClockTest::~ModifiedJulianClockTest()
{
}

void ModifiedJulianClockTest::SetUp()
{
}

void ModifiedJulianClockTest::TearDown()
{
}

TEST_F(ModifiedJulianClockTest, test_julian_day)
{
    ASSERT_EQ(julian_day(1), std::chrono::hours(24));
    ASSERT_EQ(julian_day(2), std::chrono::hours(48));
}

TEST_F(ModifiedJulianClockTest, test_time_since_epoch)
{
    std::chrono::time_point<ModifiedJulianClock> epoc;
    ASSERT_EQ(julian_day(0), epoc.time_since_epoch());
    
    std::chrono::time_point<ModifiedJulianClock> hundred = epoc + julian_day(100.0);
    ASSERT_EQ(julian_day(100.0), hundred.time_since_epoch());

}

TEST_F(ModifiedJulianClockTest, test_time_point_add)
{
    typename ModifiedJulianClock::time_point base(julian_day(48.0));
    typename ModifiedJulianClock::time_point expected(julian_day(48.5));
    typename ModifiedJulianClock::time_point midday = base + std::chrono::duration_cast<julian_day>(std::chrono::hours(12));
    ASSERT_TRUE(expected == midday);
}

TEST_F(ModifiedJulianClockTest, test_time_point_subtract)
{
    typename ModifiedJulianClock::time_point base(julian_day(48.0));
    typename ModifiedJulianClock::time_point expected(julian_day(47.5));
    typename ModifiedJulianClock::time_point midday = base - std::chrono::duration_cast<julian_day>(std::chrono::hours(12));
    ASSERT_TRUE(expected == midday);
}

TEST_F(ModifiedJulianClockTest, test_time_point_conversion)
{
    typename ModifiedJulianClock::time_point epoch(julian_day(40587.0));
    std::chrono::time_point<std::chrono::system_clock> system_epoch;
    ASSERT_TRUE( static_cast<std::chrono::time_point<std::chrono::system_clock>>(epoch) == system_epoch);
    ASSERT_TRUE( static_cast<ModifiedJulianClock::time_point>(system_epoch) == epoch);
}

TEST_F(ModifiedJulianClockTest, test_equal_time_points)
{
    typename ModifiedJulianClock::time_point tp(julian_day(40000.576453212111));
    typename ModifiedJulianClock::time_point tp2(tp);
    ASSERT_EQ(tp, tp2);
    ASSERT_TRUE( typename ModifiedJulianClock::duration(0.0) == tp -tp2);
    ASSERT_FALSE( (tp -tp2) >= boost::units::quantity<boost::units::si::time>(0.1 * boost::units::si::seconds) );
    ASSERT_FALSE( (tp -tp2) > boost::units::quantity<boost::units::si::time>(0.1 * boost::units::si::seconds) );
    ASSERT_TRUE( (tp -tp2) < boost::units::quantity<boost::units::si::time>(0.1 * boost::units::si::seconds) );
}

TEST_F(ModifiedJulianClockTest, test_cout)
{
    auto today = ModifiedJulianClock::now();
    std::cout << "time now=" << today << "\n";
}

} // namespace test
} // namespace units
} // namespace astrotypes
} // namespace pss
