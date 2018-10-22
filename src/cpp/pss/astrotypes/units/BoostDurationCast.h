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
#ifndef PSS_ASTROTYPES_UNITS_BOOSTDURATIONCAST_H
#define PSS_ASTROTYPES_UNITS_BOOSTDURATIONCAST_H

#include "TimeUnits.h"
#include <chrono>

namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief conversion helper from boost time units to the std::chrono::duration::period equivalent
 * @param type The converted type
 * @code
 * static_assert( std::is_same<boost_unit_to_std_ratio<MilliSecond>::type, std::milli>::value, "expecting milliseconds type");
 * @endcode
 */
template<class BoostUnit>
struct boost_unit_to_std_ratio;

/**
 * @brief determine is the units of the chrono::duration and the boost::quatity are equivalent
 * @param type:  will be either std::true_type or std::false_type
 * @param value: will be either true false
 */
template<typename BoostType, typename ChronoType>
struct has_equivalent_unit;

/**
 * @brief determine is the NumericalRep of the chrono::duration and the boost::quatity are equivalent
 * @param type:  will be either std::true_type or std::false_type
 * @param value: will be either true false
 */
template<typename BoostType, typename ChronoType>
struct has_equivalent_rep;

/**
 * @brief determine is the boost::quanity and the std::chrono:;duration are equivalent reperesentations
 *        i.e. both in unit and numerical representation
 * @param type:  will be either std::true_type or std::false_type
 * @param value: will be either true false
 * @code
 * typename is_equivalent<boost::units::quantity<boost::units::si::time, double>
 *                      , std::chrono::duration<double, std::ration<1>>::type      // std::true_type
 * typename is_equivalent<boost::units::quantity<boost::units::si::time, double>
 *                      , std::chrono::duration<double, std::ration<1, 100>>::type // std::false_type
 * is_equivalent<boost::units::quantity<boost::units::si::time, double>
 *             , std::chrono::duration<double, std::ration<1>>::value              // true
 * is_equivalent<boost::units::quantity<boost::units::si::time, double>
 *             , std::chrono::duration<int, std::ration<1>>::value                 // false
 * @endcode
 */
template<typename BoostType, typename ChronoType>
struct is_equivalent;

/**
 * @brief Mimic the std::duration_cast to convert to/from boost::units::quantity<time, X> tyeps
 * @details template overload to convert from a std::chrono::duration where the Unit is a time_dimesion
 *          where the two types are equivalent (in unit and numerical rep), then there is
 *          simply a reinterpret_cast (i.e no runtime overheads).
 *          Otherwise a new object is created.
 * @code
 * std:;chrono::duration<double, std::milli> my_duration(100);
 * boost::units::quantity<Seconds, double> my_boost_duration = duration_cast<boost::units::quantity<Software, double>>(my_duration);
 * @endcode
 */
template<typename BoostQuantity, typename ChronoNumericalRep, typename PeriodType>
constexpr
typename std::enable_if<boost::units::is_quantity<BoostQuantity>::value
                        && boost::units::is_unit_of_dimension<typename BoostQuantity::unit_type, boost::units::time_dimension>::value
                        && !is_equivalent<BoostQuantity, std::chrono::duration<ChronoNumericalRep, PeriodType>>::value
                       , BoostQuantity // this is the type to convert to
         >::type
duration_cast(const std::chrono::duration<ChronoNumericalRep, PeriodType>& duration)
{
    typedef std::chrono::duration<typename BoostQuantity::value_type
                                , typename boost_unit_to_std_ratio<typename BoostQuantity::unit_type>::type> chrono_duration;
    return BoostQuantity().from_value(std::chrono::duration_cast<chrono_duration>(duration).count());
}

// specialisation - when they are of equivalent types we can just reinterpet cast and save a copy
// non const version
template<typename BoostQuantity, typename ChronoNumericalRep, typename PeriodType>
constexpr
typename std::enable_if<boost::units::is_quantity<BoostQuantity>::value
                        && boost::units::is_unit_of_dimension<typename BoostQuantity::unit_type, boost::units::time_dimension>::value
                        && is_equivalent<BoostQuantity, std::chrono::duration<ChronoNumericalRep, PeriodType>>::value
                       , BoostQuantity& // this is the type to convert to
         >::type
duration_cast(std::chrono::duration<ChronoNumericalRep, PeriodType>& duration)
{
    return reinterpret_cast<BoostQuantity&>(duration);
}

// const version
template<typename BoostQuantity, typename ChronoNumericalRep, typename PeriodType>
constexpr
typename std::enable_if<boost::units::is_quantity<BoostQuantity>::value
                        && boost::units::is_unit_of_dimension<typename BoostQuantity::unit_type, boost::units::time_dimension>::value
                        && is_equivalent<BoostQuantity, std::chrono::duration<ChronoNumericalRep, PeriodType>>::value
                       , const BoostQuantity& // this is the type to convert to
         >::type
duration_cast(const std::chrono::duration<ChronoNumericalRep, PeriodType>& duration)
{
    return reinterpret_cast<const BoostQuantity&>(duration);
}

// --------- boost::units::quantity -> std::chron::duration  conversions
template<typename ChronoDuration, typename BoostNumericalRep, typename BoostUnit>
constexpr
typename std::enable_if<!boost::units::is_quantity<ChronoDuration>::value
                        && boost::units::is_unit_of_dimension<BoostUnit, boost::units::time_dimension>::value
                        && !is_equivalent<boost::units::quantity<BoostUnit, BoostNumericalRep>, ChronoDuration>::value
                       , ChronoDuration // this is the type to convert to
         >::type
duration_cast(const boost::units::quantity<BoostUnit, BoostNumericalRep>& duration)
{
    typedef std::chrono::duration<BoostNumericalRep
                                , typename boost_unit_to_std_ratio<BoostUnit>::type> equiv_chrono_duration;
    return std::chrono::duration_cast<ChronoDuration>(reinterpret_cast<const equiv_chrono_duration &>(duration));
}

template<typename ChronoDuration, typename BoostNumericalRep, typename BoostUnit>
constexpr
typename std::enable_if<!boost::units::is_quantity<ChronoDuration>::value
                        && boost::units::is_unit_of_dimension<BoostUnit, boost::units::time_dimension>::value
                        && is_equivalent<boost::units::quantity<BoostUnit, BoostNumericalRep>, ChronoDuration>::value
                       , ChronoDuration& // this is the type to convert to
         >::type
duration_cast(boost::units::quantity<BoostUnit, BoostNumericalRep>& duration)
{
    return reinterpret_cast<ChronoDuration&>(duration);
}

template<typename ChronoDuration, typename BoostNumericalRep, typename BoostUnit>
constexpr
typename std::enable_if<boost::units::is_unit_of_dimension<BoostUnit, boost::units::time_dimension>::value
                        && is_equivalent<boost::units::quantity<BoostUnit, BoostNumericalRep>, ChronoDuration>::value
                       , const ChronoDuration& // this is the type to convert to
         >::type
duration_cast(const boost::units::quantity<BoostUnit, BoostNumericalRep>& duration)
{
    return reinterpret_cast<const ChronoDuration&>(duration);
}

template<typename BoostDuration, typename BoostNumericalRep, typename BoostUnit>
constexpr
typename std::enable_if<boost::units::is_quantity<BoostDuration>::value
                        && !is_equivalent<boost::units::quantity<BoostUnit, BoostNumericalRep>, BoostDuration>::value
                       , BoostDuration // this is the type to convert to
         >::type
duration_cast(boost::units::quantity<BoostUnit, BoostNumericalRep> const& duration)
{
    return static_cast<BoostDuration>(duration);
}

} // namespace units
} // namespace astrotypes
} // namespace pss
#include "detail/BoostDurationCast.cpp"

#endif // PSS_ASTROTYPES_UNITS_BOOSTDURATIONCAST_H
