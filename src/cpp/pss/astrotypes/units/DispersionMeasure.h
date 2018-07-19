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
#ifndef PSS_ASTROTYPES_UNITS_DISPERSIONMEASURE_H
#define PSS_ASTROTYPES_UNITS_DISPERSIONMEASURE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpragmas"
#include <boost/units/quantity.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/systems/cgs/length.hpp>
#include <boost/units/io.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/physical_dimensions/length.hpp>
#include <boost/units/base_units/astronomical/parsec.hpp>
#pragma GCC diagnostic pop


namespace pss {
namespace astrotypes {

/**
 * @brief
 *     Representtive of a Dispersion Measure (dimensions and units)
 *
 * @details
 *    Implemented using the boost units library. The DedispersionMeasureType will be a boost::units::quantity
 *    with the base unit parsec/cm^3.
 *    Note you must specify the units when initialising the variable.
 *
 * @example
 * @code
 * #include "pss/astrotypes/DispersionMeasure.h"
 *
 * DispersionMeasure<float> dm_float(1.6 * parsecs_per_cube_cm); // multiple parsecs/cm^3
 * DispersionMeasure<double> dm_dble(1.0 * parsec_per_cube_cm);  // a single parsec/cm^3 n.b.just syntactical sugar, it refers to the same unit
 *
 * @endcode
 * 
 */

typedef boost::units::derived_dimension<boost::units::astronomical::parsec_base_unit, 1, boost::units::length_base_dimension, -3>::type DispersionMeasureDimensionType;
typedef boost::units::unit<DispersionMeasureDimensionType, boost::units::cgs::system> DispersionMeasureUnit;
BOOST_UNITS_STATIC_CONSTANT(parsecs_per_cube_cm, DispersionMeasureUnit); 
BOOST_UNITS_STATIC_CONSTANT(parsec_per_cube_cm, DispersionMeasureUnit); 

template<typename T>
using DispersionMeasure = boost::units::quantity<DispersionMeasureUnit, T>;

} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_UNITS_DISPERSIONMEASURE_H
