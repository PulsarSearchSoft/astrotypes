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
#include "pss/astrotypes/utils/ModuloOne.h"
#include <boost/units/quantity.hpp>
#include <boost/units/base_units/angle/degree.hpp>
#include <boost/units/systems/angle/revolutions.hpp>
#include <boost/units/conversion.hpp>

namespace pss {
namespace astrotypes {
namespace units {

BOOST_UNITS_STATIC_CONSTANT(revolution, boost::units::revolution::plane_angle);
BOOST_UNITS_STATIC_CONSTANT(revolutions, boost::units::revolution::plane_angle);

/**
 * @brief Representation of an angle as a real number between 0.0 and 1.0
 */
typedef boost::units::revolution::plane_angle PhaseAngle;

template<typename T> using Phase = boost::units::quantity<PhaseAngle, utils::ModuloOne<T>>;
template<typename T> using Revolutions = boost::units::quantity<PhaseAngle, T>;

} // namespace units
} // namespace astrotypes
} // namespace pss

namespace boost {
namespace units {

// Multiplication helpers
namespace {
template<typename T>
struct PssValueTypeHelper
{
    typedef T type;
};

template<typename T>
struct PssValueTypeHelper<pss::astrotypes::utils::ModuloOne<T>>
{
    typedef T type;
};

template<typename T>
using PssValueTypeHelper_t = typename PssValueTypeHelper<T>::type;

} // namespace


template<class Unit, class X>
inline
//typename multiply_typeof_helper<X, quantity<Unit, pss::astrotypes::utils::ModuloOne<X>> >::type
typename multiply_typeof_helper<quantity<Unit, pss::astrotypes::utils::ModuloOne<X>>, X >::type
operator*(const X& lhs, const quantity<Unit, pss::astrotypes::utils::ModuloOne<X>>& rhs)
{
    typedef typename multiply_typeof_helper<X, quantity<Unit, pss::astrotypes::utils::ModuloOne<X>>>::type type;

    return type::from_value(lhs*static_cast<X>(rhs.value()));
}


template<class Unit, class X>
inline
typename multiply_typeof_helper<quantity<Unit, pss::astrotypes::utils::ModuloOne<X>>, X >::type
operator*(const quantity<Unit, pss::astrotypes::utils::ModuloOne<X>>& lhs, const X& rhs)
{
    typedef typename multiply_typeof_helper<quantity<Unit, pss::astrotypes::utils::ModuloOne<X>>, X >::type type;

    return type::from_value(rhs*static_cast<X>(lhs.value()));
}

template< class X
        , class Y
        , class System
        >
struct multiply_typeof_helper< boost::units::quantity<unit<frequency_dimension, System>, X>
                             , boost::units::quantity<unit<time_dimension, System>, Y>
                             >
{
    typedef PssValueTypeHelper_t<typename multiply_typeof_helper<X, Y>::type>    value_type;
    typedef pss::astrotypes::units::PhaseAngle                                   unit_type;
    typedef quantity<unit_type, value_type>                                      type;
};

template< class X
        , class Y
        , class System
        >
struct multiply_typeof_helper<boost::units::quantity<unit<time_dimension, System>, X>
                            , boost::units::quantity<unit<frequency_dimension, System>, Y>
                            >
{
    typedef PssValueTypeHelper_t<typename multiply_typeof_helper<X, Y>::type>    value_type;
    typedef pss::astrotypes::units::PhaseAngle                                   unit_type;
    typedef quantity<unit_type, value_type>                                      type;
};


// Division helpers
template< class X
        , class Y
        , class System
        >
struct divide_typeof_helper< boost::units::quantity<pss::astrotypes::units::PhaseAngle, X>
                           , boost::units::quantity<unit<frequency_dimension, System>, Y>
                           >
{
    typedef PssValueTypeHelper_t<typename divide_typeof_helper<X, Y>::type>           value_type;
    typedef unit<time_dimension, System>                        unit_type;
    typedef quantity<unit_type, value_type>                     type;
};

template< class X
        , class Y
        , class System
        >
struct divide_typeof_helper< boost::units::quantity<pss::astrotypes::units::PhaseAngle, X>
                           , boost::units::quantity<unit<time_dimension, System>, Y>
                           >
{
    typedef PssValueTypeHelper_t<typename divide_typeof_helper<X, Y>::type>           value_type;
    typedef unit<frequency_dimension, System>                   unit_type;
    typedef quantity<unit_type, value_type>                     type;
};

} // units
} // boost

//#include "pss/astrotypes/units/detail/Phase.cpp"

#endif // PSS_ASTROTYPES_UNITS_PHASE_H