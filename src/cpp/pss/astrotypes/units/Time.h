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
#pragma GCC diagnostic pop

#ifndef ASTROTYPES_UNITS_TIME_H
#define ASTROTYPES_UNITS_TIME_H

namespace pss {
namespace astrotypes {

// Units
using second = boost::units::si::time;
using seconds = second;
using millisecond = boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10,
        boost::units::static_rational<-3>>>::type;
using milliseconds = millisecond;
using microsecond = boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10,
        boost::units::static_rational<-6>>>::type;
using microseconds = microsecond;
using nanosecond = boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10,
        boost::units::static_rational<-9>>>::type;
using nanoseconds = nanosecond;

/**
 * @brief Time dimension type.
 */
using Time = boost::units::quantity<boost::units::si::time, double>;

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_UNITS_TIME_H
