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
#ifndef PSS_ASTROTYPES_UNITS_QUANTITY_H
#define PSS_ASTROTYPES_UNITS_QUANTITY_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#pragma GCC diagnostic pop

namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief
 * @details
 */

template<typename Unit, typename NumericalRep, class Enabled = void>
class Quantity : public boost::units::quantity<Unit, NumericalRep>
{
        typedef boost::units::quantity<Unit, NumericalRep> BaseT;

    public:
        // export base class constructors
        //using boost::units::quantity<Unit, NumericalRep>::quantity;

        /**
         * @brief Default empty constructor.
         */
        Quantity(BaseT const& b) : BaseT(b) {}
        Quantity(BaseT&& b) : BaseT(std::move(b)) {}

        /**
         * @brief instatiate from alternative unit
         */
        template<typename UnitType, typename OtherDataType
               , typename Enable = typename std::enable_if<!std::is_same<UnitType, Unit>::value, void>::type>
        explicit Quantity(boost::units::quantity<UnitType, OtherDataType> const& o) : BaseT(o) {}

        /**
         * @brief copy ssignment
         */
        template<typename UnitType, typename OtherDataType>
        Quantity& operator=(boost::units::quantity<UnitType, OtherDataType> const& o)
        {
            static_cast<BaseT&>(*this) = BaseT(o);
            return *this; 
        }
};

template<typename Unit, typename Rep, typename Unit2, typename Rep2>
Quantity<Unit, Rep> operator+(Quantity<Unit, Rep> const& a, boost::units::quantity<Unit2, Rep2> const& b)
{
   return Quantity<Unit, Rep>(static_cast<boost::units::quantity<Unit, Rep>>(a) + b); 
}

template<typename Unit, typename Rep, typename Unit2, typename Rep2>
Quantity<Unit, Rep> operator-(Quantity<Unit, Rep> const& a, boost::units::quantity<Unit2, Rep2> const& b)
{
   return Quantity<Unit, Rep>(static_cast<boost::units::quantity<Unit, Rep>>(a) - b); 
}

} // namespace units
} // namespace astrotypes
} // namespace pss

namespace boost {
namespace units {

template<typename T1, typename Unit, typename X>
struct multiply_typeof_helper<T1, pss::astrotypes::units::Quantity<Unit, X>>
{
    typedef typename multiply_typeof_helper<T1, quantity<Unit, X>>::type BoostType;
    typedef pss::astrotypes::units::Quantity<typename BoostType::unit_type, typename BoostType::value_type> type;
};

template<typename T1, typename Unit, typename X>
struct multiply_typeof_helper<pss::astrotypes::units::Quantity<Unit, X>, T1>
{
    typedef typename multiply_typeof_helper<quantity<Unit, X>, T1>::type BoostType;
    typedef pss::astrotypes::units::Quantity<typename BoostType::unit_type, typename BoostType::value_type> type;
};

template<typename T1, typename Unit, typename X>
struct divide_typeof_helper<T1, pss::astrotypes::units::Quantity<Unit, X>>
{
    typedef typename divide_typeof_helper<T1, quantity<Unit, X>>::type BoostType;
    typedef pss::astrotypes::units::Quantity<typename BoostType::unit_type, typename BoostType::value_type> type;
};

template<typename T1, typename Unit, typename X>
struct divide_typeof_helper<pss::astrotypes::units::Quantity<Unit, X>, T1>
{
    typedef typename divide_typeof_helper<quantity<Unit, X>, T1>::type BoostType;
    typedef pss::astrotypes::units::Quantity<typename BoostType::unit_type, typename BoostType::value_type> type;
};

} // namespace units
} // namespace boost
#endif // PSS_ASTROTYPES_UNITS_QUANTITY_H
