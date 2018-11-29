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
#include "pss/astrotypes/units/test/TimePointTest.h"
#include "pss/astrotypes/units/TimePoint.h"


namespace pss {
namespace astrotypes {
namespace units {
namespace test {


TimePointTest::TimePointTest()
    : ::testing::Test()
{
}

TimePointTest::~TimePointTest()
{
}

void TimePointTest::SetUp()
{
}

void TimePointTest::TearDown()
{
}

struct TestClock : public std::chrono::system_clock
{
    static constexpr const char* symbol = "TestClock";
};

TEST_F(TimePointTest, test_time_point_plus_boost_quantity)
{
    typedef std::chrono::duration<double, std::ratio<100>> Duration;
    typedef TimePoint<TestClock, Duration> TimePoint;
    TimePoint time_point(Duration(4000));
    boost::units::quantity<boost::units::si::time, double> boost_type(100 * boost::units::si::seconds);
    TimePoint res = time_point + boost_type;
    ASSERT_EQ(res, TimePoint(Duration(4001))); 
}

TEST_F(TimePointTest, test_time_point_plus_equal_boost_quantity)
{
    typedef std::chrono::duration<double, std::ratio<100>> Duration;
    typedef TimePoint<TestClock, Duration> TimePoint;
    TimePoint time_point(Duration(4000));
    boost::units::quantity<boost::units::si::time, double> boost_type(100 * boost::units::si::seconds);
    time_point += boost_type;
    ASSERT_EQ(time_point, TimePoint(Duration(4001))); 
}

TEST_F(TimePointTest, test_time_point_minus_boost_quantity)
{
    typedef std::chrono::duration<double, std::ratio<100>> Duration;
    typedef TimePoint<TestClock, Duration> TimePoint;
    TimePoint time_point(Duration(4000));
    boost::units::quantity<boost::units::si::time, double> boost_type(100 * boost::units::si::seconds);
    TimePoint res = time_point - boost_type;
    ASSERT_EQ(res, TimePoint(Duration(3999))); 
}

TEST_F(TimePointTest, test_time_point_minus_equal_boost_quantity)
{
    typedef std::chrono::duration<double, std::ratio<100>> Duration;
    typedef TimePoint<TestClock, Duration> TimePoint;
    TimePoint time_point(Duration(4000));
    boost::units::quantity<boost::units::si::time, double> boost_type(100 * boost::units::si::seconds);
    time_point -= boost_type;
    ASSERT_EQ(time_point, TimePoint(Duration(3999))); 
}

} // namespace test
} // namespace units
} // namespace astrotypes
} // namespace pss
