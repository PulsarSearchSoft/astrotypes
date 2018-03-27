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
#include <boost/units/systems/si/frequency.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/make_scaled_unit.hpp>
#include <type_traits>
#pragma GCC diagnostic pop

#ifndef ASTROTYPES_UNITS_FREQUENCY_H
#define ASTROTYPES_UNITS_FREQUENCY_H

namespace pss {
namespace astrotypes {

// Dimension
typedef boost::units::frequency_dimension Frequency;

// Units
using Hertz = boost::units::si::frequency;
using MegaHertz = boost::units::make_scaled_unit<boost::units::si::frequency, boost::units::scale<10, boost::units::static_rational<6>>>::type;

static boost::units::si::frequency const hertz = boost::units::si::hertz;
static boost::units::si::frequency const hz = boost::units::si::hertz;

/**
 * @brief Frequency Quantity
 */
template<typename NumericalRep, typename FreqUnit=Hertz>
struct FreqQuantity : public boost::units::quantity<typename std::enable_if<std::is_same<Frequency, typename FreqUnit::dimension_type>::value, FreqUnit>::type, NumericalRep>
{
};

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_UNITS_FREQUENCY_H
