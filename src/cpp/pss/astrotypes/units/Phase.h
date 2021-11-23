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
#ifndef PSS_ASTROTYPES_UNITS_PHASE_H
#define PSS_ASTROTYPES_UNITS_PHASE_H

#include <boost/units/quantity.hpp>
#include <boost/units/systems/angle/revolutions.hpp>

namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief representation of an angle as a real number between 0.0 and 1.0
 */
class PhaseAngle : public boost::units::revolution::plane_angle
{
};

/**
 * @brief Representation of phase
 * @details
 */
template<typename T = double, class Unit=PhaseAngle>
class Phase : public boost::units::quantity<PhaseAngle, T>
{
        typedef boost::units::quantity<PhaseAngle, T> BaseT;

    public:
        Phase();
        Phase(T);

};


} // namespace units
} // namespace astrotypes
} // namespace pss
#include "pss/astrotypes/units/detail/Phase.cpp"

#endif // PSS_ASTROTYPES_UNITS_PHASE_H
