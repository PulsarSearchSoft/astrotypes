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
#include "ModifiedJulianClock.h"
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
        // export base class constructors
        using boost::units::quantity<TimeUnit, NumericalRep>::quantity;

        /**
         * @brief Default empty constructor.
         */
        Quantity(BaseT const& b) : BaseT(b) {}
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
        operator chrono_duration const& () const { reinterpret_cast<chrono_duration>(*this); };
        operator chrono_duration& () { reinterpret_cast<chrono_duration>(*this); };

        /**
         * @brief Conversion from std::chrono::duration types
         * @param duration std::chrono::duration from which to convert
         */
        template<typename DurationType, typename PeriodType>
        explicit Quantity(std::chrono::duration<DurationType, PeriodType> const & duration)
            : BaseT(std::chrono::duration_cast<std::chrono::duration<DurationType, std::ratio<1, 1>>>(duration).count()
                   * seconds) {}

};

} // namespace units
} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_UNITS_TIME_H
