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
#pragma GCC diagnostic pop
#include <chrono>

#ifndef ASTROTYPES_UNITS_TIME_H
#define ASTROTYPES_UNITS_TIME_H

namespace pss {
namespace astrotypes {

// Units
using time = boost::units::si::time;
using second = time;
using seconds = second;
using millisecond = boost::units::make_scaled_unit<second, boost::units::scale<10,
    boost::units::static_rational<-3>>>::type;
using milliseconds = millisecond;
using microsecond = boost::units::make_scaled_unit<second, boost::units::scale<10,
    boost::units::static_rational<-6>>>::type;
using microseconds = microsecond;
using nanosecond = boost::units::make_scaled_unit<second, boost::units::scale<10,
    boost::units::static_rational<-9>>>::type;
using nanoseconds = nanosecond;

/**
 * @brief Time dimension type.
 */
using Time = boost::units::quantity<time, double>;

/**
 * @brief Second data type.
 */
template<typename DataType>
class Seconds : public boost::units::quantity<second, DataType>
{
    using BaseType = boost::units::quantity<second, DataType>;

    /**
     * @brief Default empty constructor.
     */
    Seconds() {}
    /**
     * @brief Conversion from other boost::units::quantity types.
     * @param quantity boost::units::quantity object from which to convert
     */
    template<typename UnitType, typename OtherDataType>
    Seconds(boost::units::quantity<UnitType, OtherDataType> const & quantity) : BaseType(quantity) {}
    /**
     * @brief Conversion from std::chrono::duration types
     * @param duration std::chrono::duration from which to convert
     */
    template<typename DurationType, typename PeriodType>
    explicit Seconds(std::chrono::duration<DurationType, PeriodType> const & duration)
        : BaseType(std::chrono::duration_cast<std::chrono::duration<DurationType, std::ratio<1, 1>>>(duration).count()
               * seconds) {}

};

/**
 * @brief Mimic the std::duration_cast for boost::units::quantity
 */
template<typename DataType, typename DurationType, typename PeriodType>
constexpr boost::units::quantity<time, DataType> duration_cast(const std::chrono::duration<DurationType, PeriodType> &
    duration) {
    return boost::units::quantity<time, DataType>(std::chrono::duration_cast<std::chrono::duration<DataType,
        std::ration<1, 1>>>(duration).count() * seconds);
};

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_UNITS_TIME_H
