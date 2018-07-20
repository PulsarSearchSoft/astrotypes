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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/systems/si/time.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/is_quantity.hpp>
#include <boost/units/is_unit_of_dimension.hpp>
#include <boost/units/get_system.hpp>
#include <boost/units/make_scaled_unit.hpp>
#pragma GCC diagnostic pop
#include <chrono>

#ifndef ASTROTYPES_UNITS_TIME_H
#define ASTROTYPES_UNITS_TIME_H

namespace pss {
namespace astrotypes {

// Dimension
typedef boost::units::time_dimension Time;

// Units (These are Types, not variables)
using Day = boost::units::make_scaled_unit<boost::units::si::time, 
            boost::units::scale<86400, boost::units::static_rational<1>>>::type;
using Second = boost::units::si::time;
using Seconds =  Second;
using MilliSecond = boost::units::make_scaled_unit<boost::units::si::time, 
                    boost::units::scale<10, boost::units::static_rational<-3>>>::type;
using MilliSeconds = MilliSecond;
using MicroSecond = boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10,
                    boost::units::static_rational<-6>>>::type;
using MicroSeconds = MicroSecond;
using NanoSecond = boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10,
                   boost::units::static_rational<-9>>>::type;
using NanoSeconds = NanoSecond;
using PicoSecond = boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10,
                   boost::units::static_rational<-12>>>::type;
using PicoSeconds = PicoSecond;

// constants
BOOST_UNITS_STATIC_CONSTANT(day, Day);
BOOST_UNITS_STATIC_CONSTANT(days, Day);
static boost::units::si::time const second = boost::units::si::second;
static boost::units::si::time const seconds = boost::units::si::second;
BOOST_UNITS_STATIC_CONSTANT(millisecond, MilliSecond);
BOOST_UNITS_STATIC_CONSTANT(milliseconds, MilliSecond);
BOOST_UNITS_STATIC_CONSTANT(microsecond, MicroSecond);
BOOST_UNITS_STATIC_CONSTANT(microseconds, MicroSecond);
BOOST_UNITS_STATIC_CONSTANT(nanosecond, NanoSecond);
BOOST_UNITS_STATIC_CONSTANT(nanoseconds, NanoSecond);
BOOST_UNITS_STATIC_CONSTANT(picosecond, PicoSecond);
BOOST_UNITS_STATIC_CONSTANT(picoseconds, PicoSecond);

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
 * @brief Second data type.
 */
template<typename TimeUnit, typename NumericalRep>
class TimeQuantity : public boost::units::quantity<TimeUnit, NumericalRep>
{
        typedef boost::units::quantity<TimeUnit, NumericalRep> BaseT;
        typedef std::chrono::duration<NumericalRep, typename boost_unit_to_std_ratio<TimeUnit>::type> chrono_duration;

    public:
        // export base class constructors
        using boost::units::quantity<TimeUnit, NumericalRep>::quantity;

        /**
         * @brief Default empty constructor.
         */
        TimeQuantity(BaseT const& b) : BaseT(b) {}
        TimeQuantity(chrono_duration const& d) : BaseT(d.count()) {}

        /**
         * @brief auto convert to the std::chrono::duration type
         */
        operator chrono_duration const& () const { reinterpret_cast<chrono_duration>(*this); };
        operator chrono_duration& () { reinterpret_cast<chrono_duration>(*this); };

        /**
         * @brief Conversion from other boost::units::quantity types.
         * @param quantity boost::units::quantity object from which to convert
         */
        template<typename UnitType, typename OtherDataType>
        explicit TimeQuantity(boost::units::quantity<UnitType, OtherDataType> const & quantity) : BaseT(quantity) {}

        /**
         * @brief Conversion from std::chrono::duration types
         * @param duration std::chrono::duration from which to convert
         */
        template<typename DurationType, typename PeriodType>
        explicit TimeQuantity(std::chrono::duration<DurationType, PeriodType> const & duration)
            : BaseT(std::chrono::duration_cast<std::chrono::duration<DurationType, std::ratio<1, 1>>>(duration).count()
                   * seconds) {}

};

/**
 * @brief Mimic the std::duration_cast to convert to  boost::units::quantity<time, X> tyeps
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

} // namespace astrotypes
} // namespace pss
#include "detail/Time.cpp"

#endif // ASTROTYPES_UNITS_TIME_H
