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
#include "pss/astrotypes/units/test/PhaseTest.h"
#include "pss/astrotypes/units/Phase.h"
#include "pss/astrotypes/units/Frequency.h"
#include "pss/astrotypes/units/Time.h"
#include <boost/math/constants/constants.hpp>
#include <boost/units/systems/si/dimensionless.hpp>
#include <type_traits>


namespace pss {
namespace astrotypes {
namespace units {
namespace test {


PhaseTest::PhaseTest()
    : ::testing::Test()
{
}

PhaseTest::~PhaseTest()
{
}

void PhaseTest::SetUp()
{
}

void PhaseTest::TearDown()
{
}

// Test if the modulus ability of this class works internally,
// i.e. setting a Phase value of 1.0 wraps around to 0.0
TEST_F(PhaseTest, test_modulo)
{
    Phase<double> p1(1.0 * revolution);
    Phase<double> p2(0.0 * revolution);
    // Test if Phase 1.0 == 0.0
    ASSERT_EQ(p1.value(), 0.0);
    // Test if Phase 1.0 == Phase 0.0
    ASSERT_EQ(p1, p2);
}

// Test if multiplying {Time * Frequency} results in a unit with the same type as Phase
TEST_F(PhaseTest, test_construction_from_time_freq)
{
    auto f = 5.5 * hertz;
    auto t = 2.1 * seconds;
    auto phase1 = f * t;
    static_assert(std::is_same<decltype(phase1), Revolutions<double>>::value, "Expecting a Revolutions<double> type");
    auto phase2 = t * f;
    static_assert(std::is_same<decltype(phase2), Revolutions<double>>::value, "Expecting a Revolutions<double> type");
    ASSERT_EQ(phase1, phase2);
}

// Test if dividing {Phase/Frequency} results in a unit with the same type as Time
TEST_F(PhaseTest, test_phase_decay_time)
{
    auto f = 1.0 * hertz;
    Phase<double> phase(1.0 * revolution);
    auto t = phase/f;
    static_assert(std::is_same<decltype(t), boost::units::quantity<boost::units::unit<boost::units::time_dimension, boost::units::si::system>, double>>::value, "Expecting a time type");
    ASSERT_EQ(phase.value(), t.value());
}

// Test if dividing {Phase/Time} results in a unit with the same type as Frequency
TEST_F(PhaseTest, test_phase_decay_freq)
{
    auto t = 1.0 * seconds;
    Phase<double> phase(1.0 * revolution);
    auto f = phase/t;
    static_assert(std::is_same<decltype(f), boost::units::quantity<boost::units::unit<boost::units::frequency_dimension, boost::units::si::system>, double>>::value, "Expecting a frequency type");
    ASSERT_EQ(phase.value(), f.value());
}

// Test if the conversion from radians to Phase works properly
TEST_F(PhaseTest, test_conversion_from_radians)
{
    // Test radians < 1
    boost::units::quantity<boost::units::si::plane_angle, utils::ModuloOne<double>> radians(boost::math::constants::pi<double>()/4 * boost::units::si::radians); // 0.785 radians
    Phase<double> phase(radians);
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(phase.value(), Phase<double>(0.125 * revolution).value()); // 0.785 radians == 45 degrees == 0.125 revolutions

    // Test radians > 1 (modulo 1)
    boost::units::quantity<boost::units::si::plane_angle, utils::ModuloOne<double>> radians2(boost::math::constants::pi<double>()/2 * boost::units::si::radians); // 1.57 radians -> 0.57 radians
    Phase<double> phase2(radians2);
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(phase2.value(), Phase<double>(0.090845056 * revolution).value()); // 0.57 radians == 32.7 degrees == 0.090845 revolutions

    // Test radians > 1 (no modulo)
    boost::units::quantity<boost::units::si::plane_angle, double> radians3(boost::math::constants::pi<double>()/2 * boost::units::si::radians); // 1.57 radians
    Phase<double> phase3(radians3);
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(phase3.value(), Phase<double>(0.25 * revolution).value()); // 1.57 radians == 90 degrees == 0.25 revolutions

    // Test radians > 1 (no modulo), greater than one revolution
    boost::units::quantity<boost::units::si::plane_angle, double> radians4(boost::math::constants::pi<double>() * 3 * boost::units::si::radians); // 9.42 radians
    Phase<double> phase4(radians4);
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(phase4.value(), Phase<double>(0.5 * revolution).value()); // 9.42 radians == 540 degrees == 1.5 revolutions -> 0.5 revolutions
}

// Test if the conversion from Phase to radians works properly
TEST_F(PhaseTest, test_conversion_to_radians)
{
    typedef boost::units::quantity<boost::units::si::plane_angle, double> Radians;
    Radians expected_radians(boost::math::constants::pi<double>()/4 * boost::units::si::radians);
    Phase<double> phase(0.125 * revolution);
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(Radians(phase).value(), expected_radians.value());
}

// Test if multiplying {Phase * double} and {double * Phase} return a Phase type
// with the proper value (modulo 1.0 if the answer is greater than or equal to 1.0)
TEST_F(PhaseTest, test_operator_scalar_multiply)
{
    // Test result > 1
    Phase<double> phase1(0.5 * revolution);
    auto res = phase1 * 3.0;
    static_assert(std::is_same<decltype(res), Phase<double>>::value, "Expecting a Phase<double> type");
    ASSERT_EQ(res, phase1);
    auto res2 = 3.0 * phase1;
    static_assert(std::is_same<decltype(res2), Phase<double>>::value, "Expecting a Phase<double> type");
    ASSERT_EQ(res2, phase1);

    // Test result < 1
    Phase<double> phase2(0.25 * revolution);
    auto res3 = phase2 * 3.0;
    static_assert(std::is_same<decltype(res3), Phase<double>>::value, "Expecting a Phase<double> type");
    Phase<double> expected(0.75 * revolution);
    ASSERT_EQ(res3, expected);
    auto res4 = 3.0 * phase2;
    static_assert(std::is_same<decltype(res4), Phase<double>>::value, "Expecting a Phase<double> type");
    ASSERT_EQ(res4, expected);
}

// Test if dividing {Phase/double} returns a Phase type with the proper
// value (modulo 1.0 if the answer is greater than or equal to 1.0)
TEST_F(PhaseTest, test_operator_scalar_divide)
{
    // Result < 1
    Phase<double> phase1(0.8 * revolution);
    auto res1 = phase1/2.0;
    static_assert(std::is_same<decltype(res1), Phase<double>>::value, "Expecting a Phase<double> type");
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(res1.value(), Phase<double>(0.4 * revolution).value());

    // Result > 1
    auto res2 = phase1/0.5;
    static_assert(std::is_same<decltype(res2), Phase<double>>::value, "Expecting a Phase<double> type");
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(res2.value(), Phase<double>(0.6 * revolution).value());
}

// Test if adding {Phase + Phase} returns a Phase type with the proper
// value (modulo 1.0 if the answer is greater than or equal to 1.0)
TEST_F(PhaseTest, test_operator_add)
{
    Phase<double> phase1(0.5 * revolution);
    Phase<double> phase2(0.7 * revolution);
    auto res1 = phase1 + phase2;
    static_assert(std::is_same<Phase<double>, decltype(res1)>::value, "Expecting a Phase<double> type");
    auto res2 = phase2 + phase1;
    static_assert(std::is_same<Phase<double>, decltype(res2)>::value, "Expecting a Phase<double> type");
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(res1.value(), Phase<double>(0.2 * revolution).value());
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(res2.value(), Phase<double>(0.2 * revolution).value());
}

// Test if subtracting {Phase - Phase} returns a Phase type with the proper
// value (modulo 1.0 if the answer is greater than or equal to 1.0)
TEST_F(PhaseTest, test_operator_subtract)
{
    Phase<double> phase1(0.5 * revolution);
    Phase<double> phase2(0.7 * revolution);
    auto res1 = phase1 - phase2;
    static_assert(std::is_same<Phase<double>, decltype(res1)>::value, "Expecting a Phase<double> type");
    auto res2 = phase2 - phase1;
    static_assert(std::is_same<Phase<double>, decltype(res2)>::value, "Expecting a Phase<double> type");
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(res1.value(), Phase<double>(0.8 * revolution).value());
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(res2.value(), Phase<double>(0.2 * revolution).value());
}

} // namespace test
} // namespace units
} // namespace astrotypes
} // namespace pss
