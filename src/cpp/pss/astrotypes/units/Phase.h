/*
 * MIT License
 *
 * Copyright (c) 2022 PulsarSearchSoft
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
#ifndef PSS_ASTROTYPES_UNITS_PHASE_H
#define PSS_ASTROTYPES_UNITS_PHASE_H

#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Frequency.h"
#include "pss/astrotypes/utils/ModuloOne.h"
#include <boost/units/quantity.hpp>
#include <boost/units/systems/angle/revolutions.hpp>

namespace pss {
namespace astrotypes {
namespace units {

BOOST_UNITS_STATIC_CONSTANT(revolution, boost::units::revolution::plane_angle);
BOOST_UNITS_STATIC_CONSTANT(revolutions, boost::units::revolution::plane_angle);

/**
 * @brief Representation of an angle as a real number between 0.0 and <1.0
 *
 * @detail This unit can represent an angle (phase) between 0.0 (inclusive) and 1.0 (exclusive);
 *         a value of 1.0 will become 0.0. Due to the fact that all numbers with this type are
 *         by definition decimal numbers, the precision of any Phase unit is limited by the ability
 *         of the 'double' numerical type to accurately represent these values. Precision is on
 *         the order of 7-8 decimal places, but results are currently only guaranteed to 4 decimal
 *         places, as tested in the unit tests.
 */
typedef boost::units::revolution::plane_angle PhaseAngle;

template<typename T> using Phase = boost::units::quantity<PhaseAngle, utils::ModuloOne<T>>;
template<typename T> using Revolutions = boost::units::quantity<PhaseAngle, T>;

} // namespace units
} // namespace astrotypes
} // namespace pss

#include "detail/Phase.cpp"

#endif // PSS_ASTROTYPES_UNITS_PHASE_H
