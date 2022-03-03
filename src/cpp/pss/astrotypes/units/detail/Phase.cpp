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

namespace boost {
namespace units {

// Output type helpers
namespace {

// Set return type of object to templated type T
template<typename T>
struct PssValueTypeHelper
{
    typedef T type;
};

// Set return type of ModuloOne<T> object to templated type T
template<typename T>
struct PssValueTypeHelper<pss::astrotypes::utils::ModuloOne<T>>
{
    typedef T type;
};

template<typename T>
using PssValueTypeHelper_t = typename PssValueTypeHelper<T>::type;

} // namespace

// Helper to set output type of freq * time = PhaseAngle
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

// Helper to set output type of time * freq = PhaseAngle
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

// Helper to set output type of PhaseAngle/freq = time
template< class X
        , class Y
        , class System
        >
struct divide_typeof_helper< boost::units::quantity<pss::astrotypes::units::PhaseAngle, X>
                           , boost::units::quantity<unit<frequency_dimension, System>, Y>
                           >
{
    typedef PssValueTypeHelper_t<typename divide_typeof_helper<X, Y>::type>    value_type;
    typedef unit<time_dimension, System>                                       unit_type;
    typedef quantity<unit_type, value_type>                                    type;
};

// Helper to set output type of PhaseAngle/time = freq
template< class X
        , class Y
        , class System
        >
struct divide_typeof_helper< boost::units::quantity<pss::astrotypes::units::PhaseAngle, X>
                           , boost::units::quantity<unit<time_dimension, System>, Y>
                           >
{
    typedef PssValueTypeHelper_t<typename divide_typeof_helper<X, Y>::type>    value_type;
    typedef unit<frequency_dimension, System>                                  unit_type;
    typedef quantity<unit_type, value_type>                                    type;
};

} // namespace units
} // namespace boost
