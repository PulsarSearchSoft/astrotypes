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
#pragma GCC diagnostic pop

#ifndef ASTROTYPES_UNITS_FREQUENCY_H
#define ASTROTYPES_UNITS_FREQUENCY_H

namespace pss {
namespace astrotypes {

// Units
using frequency = boost::units::si::frequency;
using hertz = frequency;
using hz = hertz;
using megahertz = boost::units::make_scaled_unit<frequency , boost::units::scale<10,
    boost::units::static_rational<6>>>::type;

/**
 * @brief Frequency dimension type.
 */
using Frequency = boost::units::quantity<megahertz, double>;
/**
 * @brief Fourier frequency type.
 */
using FourierFrequency = boost::units::quantity<frequency, double>;

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_UNITS_FREQUENCY_H
