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

static constexpr decltype(boost::units::revolution::revolution) revolutions = boost::units::revolution::revolution;

/**
 * @brief representation of an angle as a real number between 0.0 and 1.0
 */
typedef boost::units::revolution::plane_angle PhaseAngle;

/**
 * @brief Representation of phase
 * @details
template<typename T = double, class Unit=PhaseAngle>
class Phase
{

    public:
        Phase();
        Phase(T);

    private:
        boost::units::quantity<PhaseAngle, T> _quantity;

};
*/

template<typename T> using Phase = boost::units::quantity<PhaseAngle, T>;

} // namespace units
} // namespace astrotypes
} // namespace pss

namespace boost {
namespace units {

template< class X
        , class Y
        , class System
        >
struct multiply_typeof_helper<boost::units::quantity<unit<frequency_dimension, System>,X>
                            , boost::units::quantity<unit<time_dimension, System> ,Y>>
{
    typedef typename multiply_typeof_helper<X,Y>::type          value_type;
    //typedef typename multiply_typeof_helper<Unit1,Unit2>::type  unit_type;
    typedef pss::astrotypes::units::PhaseAngle  unit_type;
    typedef quantity<unit_type,value_type>                      type;
};

template< class X
        , class Y
        , class System
        >
struct multiply_typeof_helper<boost::units::quantity<unit<time_dimension, System>,X>
                            , boost::units::quantity<unit<frequency_dimension, System> ,Y>>
{
    typedef typename multiply_typeof_helper<X,Y>::type          value_type;
    //typedef typename multiply_typeof_helper<Unit1,Unit2>::type  unit_type;
    typedef pss::astrotypes::units::PhaseAngle  unit_type;
    typedef quantity<unit_type,value_type>                      type;
};

} // units
} // boost

//#include "pss/astrotypes/units/detail/Phase.cpp"

#endif // PSS_ASTROTYPES_UNITS_PHASE_H
