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

#include "TimeUnits.h"
#include "BoostDurationCast.h"
#include "Quantity.h"
#include "JulianClock.h"
#include "J2000Clock.h"
#include "ModifiedJulianClock.h"
#include "cmath.h"
#include <chrono>

#ifndef ASTROTYPES_UNITS_TIME_H
#define ASTROTYPES_UNITS_TIME_H

namespace pss {
namespace astrotypes {
namespace units {

// Dimension
typedef boost::units::time_dimension Time;

// time points
typedef JulianClock::time_point JulianDate;
typedef ModifiedJulianClock::time_point ModifiedJulianDate;
typedef ModifiedJulianClock::time_point MJD;
typedef J2000Clock::time_point J2000;

/**
 * @brief Specialisation of Quantity unit for time dimension types
 * @details all the functionality of the normal Quantity object plus extra functions
 *          to ensure interoperability with std::chrono types
 */
template<typename TimeUnit, typename NumericalRep>
class Quantity<TimeUnit
                 , NumericalRep
                 , typename std::enable_if<boost::units::is_unit_of_dimension<TimeUnit, boost::units::time_dimension>::value>::type>
    : public boost::units::quantity<TimeUnit, NumericalRep>
{
        typedef boost::units::quantity<TimeUnit, NumericalRep> BaseT;
        typedef std::chrono::duration<NumericalRep, typename boost_unit_to_std_ratio<TimeUnit>::type> chrono_duration;

    public:
        // reproduce the chrono typedefs
        typedef NumericalRep rep;
        typedef typename chrono_duration::period period;

    public:
        /**
         * @brief Default empty constructor.
         */
        Quantity() {}
        Quantity(BaseT const& b) : BaseT(b) {}

        template<typename UnitType, typename OtherDataType
               , typename std::enable_if<!std::is_same<UnitType, TimeUnit>::value, void>::type>
        explicit Quantity(boost::units::quantity<UnitType, OtherDataType> const& o) : BaseT(o) {}
        explicit Quantity(chrono_duration const& d) : BaseT(d.count()) {}

        /**
         * @brief Conversion from other boost::units::quantity types.
         * @param quantity boost::units::quantity object from which to convert
         */
        template<typename UnitType, typename OtherDataType>
        explicit Quantity(boost::units::quantity<UnitType, OtherDataType> const & quantity) : BaseT(quantity) {}

        /**
         * @brief auto convert to the std::chrono::duration type
         */
        operator chrono_duration const& () const { duration_cast<chrono_duration>(*this); };
        operator chrono_duration& () { duration_cast<chrono_duration>(*this); };

        /**
         * @brief Conversion from std::chrono::duration types
         * @param duration std::chrono::duration from which to convert
         */
        template<typename DurationType, typename PeriodType>
        explicit Quantity(std::chrono::duration<DurationType, PeriodType> const & duration)
            : BaseT(std::chrono::duration_cast<std::chrono::duration<DurationType, std::ratio<1, 1>>>(duration).count()
                   * seconds) {}

        /**
         * @brief copy ssignment
         */
        template<typename UnitType, typename OtherDataType>
        Quantity& operator=(boost::units::quantity<UnitType, OtherDataType> const& o)
        {
            static_cast<BaseT&>(*this) = BaseT(o);
            return *this; 
        }

};

} // namespace units
} // namespace astrotypes
} // namespace pss

namespace std {
namespace chrono {

// interchangability between boost units and chrono types for > and < operators
template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator<(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c) > b;
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator>(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c) > b;
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator<=(std::chrono::duration<Rep, Ratio> const& lhs, boost::units::quantity<Unit, Rep2> const& rhs){ return !(lhs > rhs); }

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator>=(std::chrono::duration<Rep, Ratio> const& lhs, boost::units::quantity<Unit, Rep2> const& rhs){ return !(lhs < rhs); }

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator==(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return c == pss::astrotypes::units::duration_cast<std::chrono::duration<Rep, Ratio>>(b);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator!=(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return c != pss::astrotypes::units::duration_cast<std::chrono::duration<Rep, Ratio>>(b);
}

} // namespace chrono 
} // namespace std

// --------------------- std::crono::duration and boost::units::quantity interoperability ------------
namespace boost {
namespace units {

// simple mathc operators with chrono types
template<typename Ratio, typename Rep, typename Unit, typename Rep2>
boost::units::quantity<Unit, Rep2> operator+(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b + pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
boost::units::quantity<Unit, Rep2> operator-(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b - pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
boost::units::quantity<Unit, Rep2>& operator+=(boost::units::quantity<Unit, Rep2>& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b += pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
boost::units::quantity<Unit, Rep2>& operator-=(boost::units::quantity<Unit, Rep2>& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b -= pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

// deived by a duration
template<typename Ratio, typename Rep, typename Unit, typename Rep2>
auto operator/(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c) 
    -> decltype(boost::units::quantity<Unit, Rep2>()/boost::units::quantity<Unit, Rep>())
{
    return b / pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep>>(c);
}

// interchangability between boost units and chrono types for > and < operators
template<typename Ratio, typename Rep, typename Unit, typename Rep2>
bool operator<(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b < pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
bool operator>(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b > pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator<=(boost::units::quantity<Unit, Rep2> const& lhs, std::chrono::duration<Rep, Ratio> const& rhs){ return !(lhs > rhs); }

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
inline bool operator>=(boost::units::quantity<Unit, Rep2> const& lhs, std::chrono::duration<Rep, Ratio> const& rhs){ return !(lhs < rhs); }

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
bool operator==(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b == pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
bool operator!=(boost::units::quantity<Unit, Rep2> const& b, std::chrono::duration<Rep, Ratio> const& c)
{
    return b != pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep2>>(c);
}

} // namespace units
} // namespace boost

// overloads of some cmath functions to handle boost quantities
namespace std {

template<typename Unit, typename Rep, typename T2>
struct common_type<boost::units::quantity<Unit, Rep>, T2> : public std::common_type<Rep, T2>
{};

template<typename Unit, typename Rep, typename T2>
struct common_type<T2, boost::units::quantity<Unit, Rep>> : public std::common_type<T2, Rep>
{};

// cmath helpers
template<typename Ratio, typename Rep>
std::chrono::duration<Rep, Ratio> abs(std::chrono::duration<Rep, Ratio> const& v) {
    return std::chrono::duration<Rep, Ratio>(std::abs(v.count()));
}

namespace chrono {

// operators for standard math and boost::units::quantity types
template<typename Ratio, typename Rep, typename Unit, typename Rep2>
std::chrono::duration<Rep, Ratio> operator+(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return c + pss::astrotypes::units::duration_cast<std::chrono::duration<Rep, Ratio>>(b);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
std::chrono::duration<Rep, Ratio> operator-(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return c - pss::astrotypes::units::duration_cast<std::chrono::duration<Rep, Ratio>>(b);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
std::chrono::duration<Rep, Ratio>& operator+=(std::chrono::duration<Rep, Ratio>& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return c += pss::astrotypes::units::duration_cast<std::chrono::duration<Rep, Ratio>>(b);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
std::chrono::duration<Rep, Ratio>& operator-=(std::chrono::duration<Rep, Ratio>& c, boost::units::quantity<Unit, Rep2> const& b)
{
    return c -= pss::astrotypes::units::duration_cast<std::chrono::duration<Rep, Ratio>>(b);
}

template<typename Ratio, typename Rep, typename Unit, typename Rep2>
auto operator/(std::chrono::duration<Rep, Ratio> const& c, boost::units::quantity<Unit, Rep2> const& b) 
    -> decltype(boost::units::quantity<Unit, Rep>()/boost::units::quantity<Unit, Rep2>())
{
    return pss::astrotypes::units::duration_cast<boost::units::quantity<Unit, Rep>>(c) / b;
}

} // namespace chrono
} // namespace std

#endif // ASTROTYPES_UNITS_TIME_H
