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

// This class taken from the orignal ska::cheetah::units class

#include "../TimePoint.h"
#include "../BoostDurationCast.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/systems/si/time.hpp>
#pragma GCC diagnostic pop
#include <iomanip>
#include <ctime>

namespace pss {
namespace astrotypes {
namespace units {

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>::TimePoint( const Duration& d )
    : std::chrono::time_point<ClockType, Duration>(d)
{
}

template<typename ClockType, typename Duration>
template<typename QuantityType>
TimePoint<ClockType, Duration>::TimePoint( QuantityType const& d, typename std::enable_if<
                               boost::units::is_quantity<QuantityType>::value
                            && boost::units::is_unit_of_dimension<typename QuantityType::unit_type, boost::units::time_dimension>::value
                      , QuantityType>::type)
    : std::chrono::time_point<ClockType, Duration>(duration_cast<typename ClockType::duration>(d))
{
}

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>::TimePoint( const std::chrono::system_clock::time_point& tp )
    : std::chrono::time_point<ClockType, Duration>(std::chrono::duration_cast<typename ClockType::duration>(tp.time_since_epoch()) + ClockType::diff_from_system_epoch)
{
}

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>::TimePoint( const std::chrono::time_point<ClockType, Duration>& tp )
    : std::chrono::time_point<ClockType, Duration>(tp)
{
}

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>::TimePoint( std::chrono::time_point<ClockType, Duration>&& tp )
    : std::chrono::time_point<ClockType, Duration>(std::move(tp))
{
}

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>::operator typename std::chrono::system_clock::time_point() const
{
    auto dur = this->time_since_epoch() - ClockType::diff_from_system_epoch;
    return std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(dur));
}

template<typename ClockType, typename Duration>
std::time_t TimePoint<ClockType, Duration>::to_time_t() const
{
    return std::chrono::system_clock::to_time_t(static_cast<std::chrono::system_clock::time_point>(*this));
}

template<typename ClockType, typename Duration>
std::ostream& operator<<(std::ostream& os, TimePoint<ClockType, Duration> const& tp)
{
    //std::time_t tt = tp.to_time_t();
    //os << std::put_time(std::gmtime(&tt), "%F %T"); // not implemented till gcc 5.0
    //os << asctime(std::gmtime(&tt));
    os << tp.time_since_epoch().count() << " " << ClockType::symbol;
    return os;
}

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>& TimePoint<ClockType, Duration>::operator+=(Duration const& d)
{
    std::chrono::time_point<ClockType, Duration>::operator+=(d);
    return *this;
}

template<typename ClockType, typename Duration>
TimePoint<ClockType, Duration>& TimePoint<ClockType, Duration>::operator-=(Duration const& d)
{
    std::chrono::time_point<ClockType, Duration>::operator-=(d);
    return *this;
}

template<typename ClockType, typename Duration, typename Duration2>
TimePoint<ClockType, Duration> operator+(TimePoint<ClockType, Duration> const& lhs, Duration2 const& rhs) {
    return TimePoint<ClockType, Duration>(static_cast<std::chrono::time_point<ClockType, Duration>>(lhs) + rhs);
}

template<typename ClockType, typename DurationType, typename Rep>
TimePoint<ClockType, DurationType> operator+(TimePoint<ClockType, DurationType> const& lhs, boost::units::quantity<boost::units::si::time, Rep> const& rhs) {
    typedef typename ClockType::duration Duration;
    return lhs + duration_cast<Duration>(rhs);
}

template<typename ClockType, typename DurationType, typename Rep>
TimePoint<ClockType, DurationType>& operator+=(TimePoint<ClockType, DurationType>& lhs, boost::units::quantity<boost::units::si::time, Rep> const& rhs)
{
    typedef typename ClockType::duration Duration;
    return lhs += duration_cast<Duration>(rhs);
}

template<typename ClockType, typename DurationType, typename Rep>
TimePoint<ClockType, DurationType> operator-(TimePoint<ClockType, DurationType> const& lhs, boost::units::quantity<boost::units::si::time, Rep> const& rhs) {
    typedef typename ClockType::duration Duration;
    return lhs - duration_cast<Duration>(rhs);
}

template<typename ClockType, typename DurationType, typename Rep>
TimePoint<ClockType, DurationType>& operator-=(TimePoint<ClockType, DurationType>& lhs, boost::units::quantity<boost::units::si::time, Rep> const& rhs)
{
    typedef typename ClockType::duration Duration;
    return lhs -= duration_cast<Duration>(rhs);
}

template<typename ClockType, typename Duration, typename Duration2>
TimePoint<ClockType, Duration> operator-(TimePoint<ClockType, Duration> const& lhs, Duration2 const& rhs) {
    return TimePoint<ClockType, Duration>(static_cast<std::chrono::time_point<ClockType, Duration>>(lhs) - rhs);
}

template<typename ClockType, typename Duration, typename Duration2>
Duration operator-(TimePoint<ClockType, Duration> const& lhs, TimePoint<ClockType, Duration2> const& rhs)
{
    return static_cast<std::chrono::time_point<ClockType, Duration>>(lhs) - static_cast<std::chrono::time_point<ClockType, Duration2>>(rhs);
}

} // namespace units
} // namespace astrotypes
} // namespace pss
