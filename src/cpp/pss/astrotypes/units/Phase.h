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

#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Frequency.h"
#include <boost/units/quantity.hpp>
#include <boost/units/systems/angle/revolutions.hpp>

namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief representation of an angle as a real number between 0.0 and 1.0
 */
typedef boost::units::revolution::plane_angle PhaseAngle;
//class PhaseAngle : public boost::units::revolution::plane_angle
//{
//};

/**
 * @brief Representation of phase
 * @details
 */
template<typename T = double, class Unit=PhaseAngle>
class Phase
{

    public:
        Phase();
        Phase(T);

    private:
        boost::units::quantity<PhaseAngle, T> _quantity;

};

template<typename UnitType1
        , typename UnitType2
        , typename DataType1
        , typename DataType2
        , EnableIfIsTimeUnit<UnitType1, bool> = true
        , EnableIfIsFrequencyUnit<UnitType2, bool> = true
        >
Phase<DataType1> operator*(boost::units::quantity<UnitType1, DataType1> const& a, boost::units::quantity<UnitType2, DataType2> const& b)
{
    return Phase<DataType1>(a*b);
};

template<typename UnitType1
        , typename UnitType2
        , typename DataType1
        , typename DataType2
        , EnableIfIsFrequencyUnit<UnitType1, bool> = true
        , EnableIfIsTimeUnit<UnitType2, bool> = true
        >
Phase<DataType1> operator*(boost::units::quantity<UnitType1, DataType1> const& a, boost::units::quantity<UnitType2, DataType2> const& b)
{
    return Phase<DataType1>(a*b);
};


} // namespace units
} // namespace astrotypes
} // namespace pss
#include "pss/astrotypes/units/detail/Phase.cpp"

#endif // PSS_ASTROTYPES_UNITS_PHASE_H
