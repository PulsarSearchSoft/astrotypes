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
#ifndef PSS_ASTROTYPES_UNITS_TIMEUNITS_H
#define PSS_ASTROTYPES_UNITS_TIMEUNITS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/systems/si/time.hpp>
#include <boost/units/make_scaled_unit.hpp>
#include <boost/units/is_unit_of_dimension.hpp>
#pragma GCC diagnostic pop

namespace pss {
namespace astrotypes {
namespace units {

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

// time constants
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

} // namespace units
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_UNITS_TIMEUNITS_H
