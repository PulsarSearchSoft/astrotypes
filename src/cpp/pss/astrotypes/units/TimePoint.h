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

#ifndef PSS_ASTROTYPES_UTILS_TIMEPOINT_H
#define PSS_ASTROTYPES_UTILS_TIMEPOINT_H
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/systems/si/time.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/is_quantity.hpp>
#include <boost/units/is_unit_of_dimension.hpp>
#pragma GCC diagnostic pop
#include <chrono>
#include <ostream>


namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief
 *     extension of std::chrono::time_point for astrotypes clocks
 * @tparam ClockType must support the std::clock conecpt as as well as defining a duration type @member diff_from_system_epoch
 */
template<typename ClockType, typename Duration = typename ClockType::duration>
struct TimePoint : public std::chrono::time_point<ClockType, Duration>
{
    explicit TimePoint( const Duration& d = typename ClockType::duration() );
    template<typename QuantityType>
    /// construct with boost::units::quantities
    explicit TimePoint( QuantityType const& d, typename std::enable_if<
                               boost::units::is_quantity<QuantityType>::value
                            && boost::units::is_unit_of_dimension<typename QuantityType::unit_type, boost::units::time_dimension>::value
                      , QuantityType>::type = 0);
    explicit TimePoint( const std::chrono::system_clock::time_point& tp );
    TimePoint( const std::chrono::time_point<ClockType, Duration>& tp );
    TimePoint( std::chrono::time_point<ClockType, Duration>&& );

    operator typename std::chrono::system_clock::time_point() const;
    TimePoint<ClockType, Duration>& operator+=(Duration const&);
    TimePoint<ClockType, Duration>& operator-=(Duration const&);

    /**
     * @brief convert to a C style time struct.
     * @details very useful if you want to output the time as a string with e.g. std::put_time
     */
    std::time_t to_time_t() const;
};

template<typename ClockType, typename Duration, typename Duration2>
TimePoint<ClockType, Duration> operator+(TimePoint<ClockType, Duration> const& lhs, Duration2 const& rhs);

template<typename ClockType, typename Duration, typename Duration2>
TimePoint<ClockType, Duration> operator-(TimePoint<ClockType, Duration> const& lhs, Duration2 const& rhs);

template<typename ClockType, typename Duration, typename Duration2>
Duration operator-(TimePoint<ClockType, Duration> const& lhs, TimePoint<ClockType, Duration2> const& rhs);

/**
 * @brief output a TimePoint as a string to the ostream
 * @details in ISO8601 format UTC. For other formats you must
 *          use the to_time_t() method and the std::put_time function
 */
template<typename ClockType, typename Duration>
std::ostream& operator<<(std::ostream&, TimePoint<ClockType> const& tp);

} // namespace units
} // namespace astrotypes
} // namespace pss
#include "detail/TimePoint.cpp"

#endif // PSS_ASTROTYPES_UTILS_TIMEPOINT_H
