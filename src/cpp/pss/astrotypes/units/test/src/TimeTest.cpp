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

#include "../TimeTest.h"

namespace pss {
namespace astrotypes {
namespace units {
namespace test {

void TimeTest::SetUp() {}

void TimeTest::TearDown() {}

TimeTest::TimeTest() {}

TimeTest::~TimeTest() {}

TEST(TimeTest, test_seconds)
{
    std::chrono::seconds chrono_seconds(10);
    std::chrono::milliseconds chrono_milliseconds(1000);
    Quantity<Seconds, int> local_seconds(chrono_seconds);

    ASSERT_EQ(chrono_seconds.count(), local_seconds.value());
    local_seconds = Quantity<Seconds, int>(chrono_milliseconds);
    ASSERT_EQ(1, local_seconds.value());
}

TEST(TimeTest, duration_cast_milliseconds_to_seconds)
{
    // chrono -> boost
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::duration<double, std::ratio<1,1000>> d(100);
    auto conversion = duration_cast<BoostType>(d);
    static_assert(std::is_same<decltype(conversion), BoostType>::value, "wrong type returned by cast");
    BoostType bs(100 * milliseconds);
    ASSERT_EQ(conversion.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::ratio<1>> ChronoType;
    boost::units::quantity<MilliSeconds, double> q(100 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), ChronoType>::value, "wrong type returned by cast");
    ASSERT_EQ(1000 * chrono_conversion.count(), q.value());
}

TEST(TimeTest, duration_cast_seconds_to_seconds)
{
    // chrono -> boost
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::duration<double, std::ratio<1,1>> d(100);
    auto conversion = duration_cast<BoostType>(d);
    static_assert(std::is_same<decltype(conversion), BoostType>::value, "wrong type returned by cast");
    BoostType bs(100 * seconds);
    ASSERT_EQ(conversion.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::ratio<1>> ChronoType;
    boost::units::quantity<Seconds, double> q(10 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), ChronoType>::value, "wrong type returned by cast");
    ASSERT_EQ(chrono_conversion.count(), q.value());
}

TEST(TimeTest, duration_cast_const_seconds_to_seconds)
{
    // chrono -> boost
    typedef boost::units::quantity<Seconds, double> BoostType;
    const std::chrono::duration<double, std::ratio<1,1>> d(100);
    auto conversion = duration_cast<BoostType>(d);
    static_assert(std::is_same<decltype(conversion), BoostType>::value, "wrong type returned by cast");
    BoostType bs(100 * seconds);
    ASSERT_EQ(conversion.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::ratio<1>> ChronoType;
    const boost::units::quantity<Seconds, double> q(10 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), ChronoType>::value, "wrong type returned by cast");
    ASSERT_EQ(chrono_conversion.count(), q.value());
}

TEST(TimeTest, duration_cast_seconds_to_milliseconds)
{
    // chrono -> boost
    typedef boost::units::quantity<MilliSeconds, double> BoostType;
    std::chrono::duration<double, std::ratio<1,1>> d(100);
    auto conversion = duration_cast<BoostType>(d);
    static_assert(std::is_same<decltype(conversion), BoostType>::value, "wrong type returned by cast");
    BoostType bs(100 * seconds);
    ASSERT_EQ(conversion.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::milli> ChronoType;
    boost::units::quantity<Seconds, double> q(100 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), ChronoType>::value, "wrong type returned by cast");
    ASSERT_EQ(chrono_conversion.count(), 1000 * q.value());
}

TEST(TimeTest, duration_cast_milliseconds_to_milliseconds)
{
    // chrono -> boost
    typedef boost::units::quantity<MilliSeconds, double> BoostType;
    std::chrono::duration<double, std::milli> d(100);
    auto conversion = duration_cast<BoostType>(d);
    static_assert(std::is_same<decltype(conversion), BoostType>::value, "wrong type returned by cast");
    BoostType bs(100 * milliseconds);
    ASSERT_EQ(conversion.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::milli> ChronoType;
    boost::units::quantity<MilliSeconds, double> q(10 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), ChronoType>::value, "wrong type returned by cast");
    ASSERT_EQ(chrono_conversion.count(), q.value());
}

} // namespace test
} // namespace units
} // namespace astrotypes
} // namespace pss

