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

TEST(TimeTest, operator_equal_chrono_quantity)
{
    boost::units::quantity<Seconds, double> b(10 * seconds);
    const std::chrono::seconds chrono_seconds(10);

    ASSERT_TRUE(chrono_seconds == b);
    ASSERT_TRUE(b == chrono_seconds);
    ASSERT_FALSE(chrono_seconds != b);
    ASSERT_FALSE(b != chrono_seconds);

}

TEST(TimeTest, chrono_duration_operator_less_than_quantity)
{
    boost::units::quantity<Seconds, double> b(10 * seconds);
    ASSERT_FALSE(std::chrono::milliseconds(10001) < b);
    ASSERT_FALSE(std::chrono::milliseconds(10000) < b);
    ASSERT_TRUE(std::chrono::milliseconds(9999) < b);

    ASSERT_FALSE(std::chrono::seconds(11) < b);
    ASSERT_FALSE(std::chrono::seconds(10) < b);
    ASSERT_TRUE(std::chrono::seconds(9) < b);
}

TEST(TimeTest, chrono_duration_operator_less_than_equal_quantity)
{
    boost::units::quantity<Seconds, double> b(10 * seconds);
    ASSERT_FALSE(std::chrono::milliseconds(10001) <= b);
    ASSERT_TRUE(std::chrono::milliseconds(10000) <= b);
    ASSERT_TRUE(std::chrono::milliseconds(9999) <= b);

    ASSERT_FALSE(std::chrono::seconds(11) <= b);
    ASSERT_TRUE(std::chrono::seconds(10) <= b);
    ASSERT_TRUE(std::chrono::seconds(9) <= b);
}

TEST(TimeTest, chrono_duration_operator_greater_than_quantity)
{
    boost::units::quantity<Seconds, double> b(10 * seconds);
    ASSERT_TRUE(std::chrono::milliseconds(10001) > b);
    ASSERT_FALSE(std::chrono::milliseconds(10000) > b);
    ASSERT_FALSE(std::chrono::milliseconds(9999) > b);

    ASSERT_TRUE(std::chrono::seconds(11) > b);
    ASSERT_FALSE(std::chrono::seconds(10) > b);
    ASSERT_FALSE(std::chrono::seconds(9) > b);
}

TEST(TimeTest, chrono_duration_operator_greater_than_equal_quantity)
{
    boost::units::quantity<Seconds, double> b(10 * seconds);
    ASSERT_TRUE(std::chrono::milliseconds(10001) >= b);
    ASSERT_TRUE(std::chrono::milliseconds(10000) >= b);
    ASSERT_FALSE(std::chrono::milliseconds(9999) >= b);

    ASSERT_TRUE(std::chrono::seconds(11) >= b);
    ASSERT_TRUE(std::chrono::seconds(10) >= b);
    ASSERT_FALSE(std::chrono::seconds(9) >= b);
}


TEST(TimeTest, quantity_operator_plus)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    const std::chrono::seconds chrono_seconds(10);
    const BoostType a(100 * seconds);

    BoostType b = a + chrono_seconds;
    ASSERT_EQ( b, BoostType(110 * seconds) );

}

TEST(TimeTest, time_point_plus_quantity)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePointT;
    const TimePointT chrono_time;
    const BoostType a(10 * seconds);

    TimePointT c = chrono_time + a;
    ASSERT_EQ( c, TimePointT(std::chrono::seconds(10)) );
}

TEST(TimeTest, time_point_plus_equal_quantity)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePointT;
    TimePointT chrono_time;
    const BoostType a(10 * seconds);

    TimePointT const& c = chrono_time += a;
    ASSERT_EQ(&c, &chrono_time);
    ASSERT_EQ( c, TimePointT(std::chrono::seconds(10)) );
}

TEST(TimeTest, time_point_minus_quantity)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePointT;
    const TimePointT chrono_time(std::chrono::seconds(100));
    const BoostType a(10 * seconds);

    TimePointT c = chrono_time - a;
    ASSERT_EQ( c, TimePointT(std::chrono::seconds(90)) );
}

TEST(TimeTest, time_point_minus_equal_quantity)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePointT;
    TimePointT chrono_time(std::chrono::seconds(100));
    const BoostType a(10 * seconds);

    TimePointT const& c = chrono_time -= a;
    ASSERT_EQ(&c, &chrono_time);
    ASSERT_EQ( c, TimePointT(std::chrono::seconds(90)) );
}

TEST(TimeTest, duration_operator_plus)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    const std::chrono::seconds chrono_seconds(100);
    const BoostType a(10 * seconds);

    std::chrono::seconds c = chrono_seconds + a;
    ASSERT_EQ( c, std::chrono::seconds(110) );
}

TEST(TimeTest, quantity_operator_minus)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::seconds chrono_seconds(10);
    BoostType a(100 * seconds);

    BoostType b = a - chrono_seconds;
    ASSERT_EQ( b, BoostType(90 * seconds) );

}

TEST(TimeTest, duration_operator_minus)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::seconds chrono_seconds(100);
    BoostType a(10 * seconds);

    std::chrono::seconds c = chrono_seconds - a;
    ASSERT_EQ( c, std::chrono::seconds(90) );
}

TEST(TimeTest, quantity_operator_plus_equal)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::seconds chrono_seconds(10);
    BoostType a(100 * seconds);

    a += chrono_seconds;
    ASSERT_EQ( a, BoostType(110 * seconds) );

}

TEST(TimeTest, duration_operator_plus_equal)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::seconds chrono_seconds(10);
    BoostType b(100 * seconds);

    chrono_seconds += b;
    ASSERT_EQ( chrono_seconds, std::chrono::seconds(110) );
}

TEST(TimeTest, quantity_operator_minus_equal)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::seconds chrono_seconds(10);
    BoostType a(100 * seconds);

    a -= chrono_seconds;
    ASSERT_EQ( a, BoostType(90 * seconds) );
}

TEST(TimeTest, duration_operator_minus_equal)
{
    typedef boost::units::quantity<Seconds, double> BoostType;
    std::chrono::seconds chrono_seconds(100);
    BoostType b(10 * seconds);

    chrono_seconds -= b;
    ASSERT_EQ( chrono_seconds, std::chrono::seconds(90) );
}

TEST(TimeTest, quantity_operator_divide)
{
    typedef boost::units::quantity<MilliSeconds, double> BoostType;
    const std::chrono::seconds chrono_seconds(10);
    const BoostType a(100 * seconds);

    auto res = a / chrono_seconds;
    ASSERT_EQ( res, 10.0 );
}

TEST(TimeTest, duration_operator_divide)
{
    typedef boost::units::quantity<MilliSeconds, double> BoostType;
    const std::chrono::seconds chrono_seconds(100);
    const BoostType a(10 * seconds);

    auto res = chrono_seconds / a;
    ASSERT_EQ( res, 10.0 );
}

TEST(TimeTest, Quantity_operator_construct)
{
    Quantity<boost::units::si::time, double> a(100 * seconds);
    Quantity<MilliSeconds, double> b(a);
    Quantity<MilliSeconds, double> expect(100 * seconds);
    ASSERT_EQ(b, expect);

    // from a boost::units::quantity
    boost::units::quantity<MilliSeconds, double> c(a);
    ASSERT_EQ(c, expect);
}

TEST(TimeTest, Quantity_operator_equal_assign)
{
    Quantity<boost::units::si::time, double> a(100 * seconds);
    Quantity<MilliSeconds, double> expect(100 * seconds);
    Quantity<MilliSeconds, double> b;
    ASSERT_NE(b, expect);
    b = a;
    ASSERT_EQ(b, expect);

    // with a boost::quantity
    boost::units::quantity<boost::units::si::time, double> c(200 * seconds);
    Quantity<MilliSeconds, double> expect2(200 * seconds);
    b = c;
    ASSERT_EQ(b, expect2);
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

    // boost -> boost
    BoostType bs2 = duration_cast<BoostType>(bs);
    ASSERT_EQ(bs2.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::ratio<1>> ChronoType;
    const boost::units::quantity<Seconds, double> q(10 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), ChronoType>::value, "wrong type returned by cast");
    ASSERT_EQ(chrono_conversion.count(), q.value());
}

TEST(TimeTest, duration_cast_const_seconds_to_milliseconds)
{
    // chrono -> boost
    typedef boost::units::quantity<MilliSeconds, double> BoostType;
    const std::chrono::duration<double, std::ratio<1,1>> d(100);
    auto conversion = duration_cast<BoostType>(d);
    static_assert(std::is_same<decltype(conversion), BoostType>::value, "wrong type returned by cast");
    BoostType bs(100 * seconds);
    ASSERT_EQ(conversion.value(), bs.value());

    // boost -> chrono
    typedef std::chrono::duration<double, std::milli> ChronoType;
    boost::units::quantity<Seconds, double> q(100 * seconds);;
    auto chrono_conversion = duration_cast<ChronoType>(q);
    static_assert(std::is_same<decltype(chrono_conversion), typename std::remove_cv<ChronoType>::type>::value, "wrong type returned by cast");
    ASSERT_EQ(chrono_conversion.count(), 1000 * q.value());

    // boost -> boost
    const BoostType bs2 = duration_cast<BoostType>(q);
    ASSERT_EQ(bs2.value(), q.value() * 1000);
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

    // boost -> boost
    const BoostType bs2 = duration_cast<BoostType>(q);
    ASSERT_EQ(bs2.value(), q.value() * 1000);
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

