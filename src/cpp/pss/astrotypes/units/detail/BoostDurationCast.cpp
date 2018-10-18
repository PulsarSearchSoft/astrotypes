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

namespace pss {
namespace astrotypes {
namespace units {

template<class BoostUnit>
struct boost_unit_to_std_ratio {
    static_assert(std::is_same<BoostUnit, BoostUnit>::value, "Unkown conversion from boost unit to std::ratio");
};

template<>
struct boost_unit_to_std_ratio<Day> {
    typedef std::ratio<86400, 1> type;
};

template<>
struct boost_unit_to_std_ratio<Second> {
    typedef std::ratio<1,1> type;
};

template<>
struct boost_unit_to_std_ratio<MilliSecond> {
    typedef std::milli type;
};

template<>
struct boost_unit_to_std_ratio<MicroSecond> {
    typedef std::micro type;
};

template<>
struct boost_unit_to_std_ratio<NanoSecond> {
    typedef std::nano type;
};

template<>
struct boost_unit_to_std_ratio<PicoSecond> {
    typedef std::pico type;
};

template<typename BoostType, typename ChronoType>
struct has_equivalent_unit : public std::is_same<typename boost_unit_to_std_ratio<typename BoostType::unit_type>::type
                                               , typename ChronoType::period>::type
{
};

template<typename BoostUnit, typename BoostUnit2, typename NumT, typename Num2T>
struct has_equivalent_unit<boost::units::quantity<BoostUnit, NumT>,
                          boost::units::quantity<BoostUnit2, Num2T>> : public std::is_same<NumT, Num2T>::type
{
};

template<typename BoostType, typename ChronoType>
struct has_equivalent_rep : public std::is_same<typename BoostType::value_type, typename ChronoType::rep>::type
{
};

template<typename BoostUnit, typename BoostUnit2, typename NumT, typename Num2T>
struct has_equivalent_rep<boost::units::quantity<BoostUnit, NumT>,
                          boost::units::quantity<BoostUnit2, Num2T>> : public std::is_same<BoostUnit, BoostUnit2>::type
{
};

template<typename BoostType, typename ChronoType>
struct is_equivalent : public std::conditional<has_equivalent_unit<BoostType, ChronoType>::value
                                            && has_equivalent_rep<BoostType, ChronoType>::value
                                             , std::true_type, std::false_type
                                              >::type
{
};

} // namespace units
} // namespace astrotypes
} // namespace pss
