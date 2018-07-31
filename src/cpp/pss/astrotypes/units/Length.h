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
#ifndef PSS_ASTROTYPES_UNITS_LENGTH_H
#define PSS_ASTROTYPES_UNITS_LENGTH_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/systems/si/length.hpp>
#include <boost/units/base_units/astronomical/parsec.hpp>
#pragma GCC diagnostic pop

namespace pss {
namespace astrotypes {
namespace units {

using Meter = boost::units::si::length;
using Meters = Meter;
using Metre = Meter;
using Metres = Meter;
using CentiMeter = boost::units::make_scaled_unit<boost::units::si::length, 
                    boost::units::scale<10, boost::units::static_rational<-3>>>::type;
using CentiMeters = CentiMeter;
using CentiMetres = CentiMeter;
using CentiMetre = CentiMeter;
using MilliMeter = boost::units::make_scaled_unit<boost::units::si::length, 
                   boost::units::scale<10, boost::units::static_rational<-3>>>::type;
using MilliMeters = MilliMeter;
using MilliMetres = MilliMeter;
using MilliMetre = MilliMeter;

// parsec
typedef boost::units::make_system<boost::units::astronomical::parsec_base_unit
                                 ,boost::units::si::second_base_unit>::type ParsecSecSystem;
using Parsec = boost::units::unit<boost::units::length_dimension, ParsecSecSystem>;
using Parsecs = Parsec;
BOOST_UNITS_STATIC_CONSTANT(parsec, Parsec); 
BOOST_UNITS_STATIC_CONSTANT(parsecs, Parsec); 

} // namespace units
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_UNITS_LENGTH_H
