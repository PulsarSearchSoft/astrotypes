/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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

#include "pss/astrotypes/utils/test/ModuloOneTest.h"
#include "pss/astrotypes/utils/ModuloOne.h"

namespace pss {
namespace astrotypes {
namespace utils {
namespace test {

ModuloOneTest::ModuloOneTest()
    : ::testing::Test()
{
}

ModuloOneTest::~ModuloOneTest()
{
}

void ModuloOneTest::SetUp()
{
}

void ModuloOneTest::TearDown()
{
}

TEST_F(ModuloOneTest, test_constuctor)
{
    double const v1 = 0.5; // floating-point value below modulus
    ModuloOne<double> mv(v1);
    ASSERT_EQ(static_cast<double>(mv), v1);

    int const v2 = 2; // integer value above modulus
    ModuloOne<int> mv2(v2);
    ASSERT_EQ(static_cast<int>(mv2), 0);

    double const v3 = 3.4; // floating-point value above modulus
    ModuloOne<double> mv3(v3);
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(mv3), 0.4);

    ModuloOne<int> mv4(1); // value == modulus
    ASSERT_EQ(static_cast<int>(mv4), 0);
}

TEST_F(ModuloOneTest, test_preincrement)
{
    double const v1 = 0.7;
    ModuloOne<double> mv(v1);
    const ModuloOne<double>& rv = ++mv;
    ASSERT_EQ(static_cast<double>(rv), v1);
    ASSERT_EQ(&rv, &mv);

    ModuloOne<int> mv2(-1);
    ++mv2;
    ASSERT_EQ(static_cast<int>(mv2), 0);
}

TEST_F(ModuloOneTest, test_predecrement)
{
    double const v1 = 0.7;
    ModuloOne<double> mv(v1);
    const ModuloOne<double>& rv = --mv;
    ASSERT_EQ(static_cast<double>(rv), v1);
    ASSERT_EQ(&rv, &mv);

    ModuloOne<int> mv2(0);
    --mv2;
    ASSERT_EQ(static_cast<int>(mv2), 0);
}

TEST_F(ModuloOneTest, test_postincrement)
{
    double const v1 = 0.7;
    ModuloOne<double> mv(v1);
    const ModuloOne<double>& rv = mv++;
    ASSERT_EQ(static_cast<double>(rv), v1);
    ASSERT_EQ(rv, mv);

    ModuloOne<int> mv2(-1);
    mv2++;
    ASSERT_EQ(static_cast<int>(mv2), 0);
}

TEST_F(ModuloOneTest, test_postdecrement)
{
    double const v1 = 0.7;
    ModuloOne<double> mv(v1);
    const ModuloOne<double>& rv = mv--;
    ASSERT_EQ(static_cast<double>(rv), v1);
    ASSERT_EQ(rv, mv);

    ModuloOne<int> mv2(1);
    mv2--;
    ASSERT_EQ(static_cast<int>(mv2), 0);
}

TEST_F(ModuloOneTest, test_equality)
{
    double const v1 = 0.5;
    double const v2 = 0.0;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(v2);
    ASSERT_TRUE(mv2 != mv);
    ASSERT_FALSE(mv2 == mv);

    ModuloOne<double> mv3(v1);
    ASSERT_TRUE(mv == mv3);
    ASSERT_FALSE(mv != mv3);
}

TEST_F(ModuloOneTest, test_plusequal)
{
    double const v1 = 0.5;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(v1);

    mv += v1;
    ASSERT_EQ(static_cast<int>(mv), 0);
    mv += mv2;
    ASSERT_EQ(static_cast<double>(mv), v1);
}

TEST_F(ModuloOneTest, test_minusequal)
{
    double const v1 = 0.5;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(v1);

    mv -= v1;
    ASSERT_EQ(static_cast<int>(mv), 0);
    mv -= mv2;
    ASSERT_EQ(static_cast<double>(mv), v1);

    ModuloOne<int> mv3(1);
    mv3 -= 3;
    ASSERT_EQ(static_cast<int>(mv3), 0);
}

TEST_F(ModuloOneTest, test_plus)
{
    double const v1 = 0.5;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(8);

    ModuloOne<double> result = mv + v1;
    ASSERT_EQ(static_cast<int>(result), 0);
    ModuloOne<double> result2 = mv + mv2;
    ASSERT_EQ(static_cast<double>(result2), v1);
}

TEST_F(ModuloOneTest, test_minus)
{
    double const v1 = 0.5;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(v1);

    ModuloOne<double> result = mv - v1;
    ASSERT_EQ(static_cast<int>(result), 0);
    ModuloOne<double> result2 = mv - mv2;
    ASSERT_EQ(static_cast<int>(result2), 0);

    ModuloOne<double> result3 = mv - 1.0;
    ASSERT_EQ(static_cast<double>(result3), static_cast<double>(mv));
}

TEST_F(ModuloOneTest, test_multiply)
{
    double const v1 = 0.7;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(v1);

    ModuloOne<double> result = mv * 3.0;
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(static_cast<double>(result), 0.1);
    ModuloOne<double> result2 = mv * mv2;
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(result2), 0.49);
}

TEST_F(ModuloOneTest, test_divide)
{
    double const v1 = 0.8;
    ModuloOne<double> const mv(v1);
    double const v2 = 0.4;
    ModuloOne<double> const mv2(v2);
    double const v3 = 0.6;
    ModuloOne<double> const mv3(v3);

    ModuloOne<double> result = mv/v1;
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(result), 0);
    ModuloOne<double> result2 = mv/mv2;
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(result2), 0);
    ModuloOne<double> result3 = mv2/mv;
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(result3), 0.5);
    ModuloOne<double> result4 = mv3/mv2;
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(result4), 0.5);
}

TEST_F(ModuloOneTest, test_multiply_equal)
{
    double const v1 = 0.7;
    ModuloOne<double> mv(v1);
    double const v2 = 0.5;
    ModuloOne<double> mv2(v2);

    mv *= 3.0;
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(static_cast<double>(mv), 0.1);
    mv *= mv2;
    // Use ASSERT_FLOAT_EQ instead of ASSERT_EQ or ASSERT_DOUBLE_EQ, as both
    // compare results to too many decimal places for the double-precision result
    ASSERT_FLOAT_EQ(static_cast<double>(mv), 0.05);
}

TEST_F(ModuloOneTest, test_divide_equal)
{
    double const v1 = 0.8;
    ModuloOne<double> mv(v1);
    ModuloOne<double> mv2(v1);
    ModuloOne<double> mv3(v1);
    double const v2 = 0.4;
    ModuloOne<double> mv4(v2);
    double const v3 = 0.6;
    ModuloOne<double> mv5(v3);
    ModuloOne<double> mv6(v3);
    ModuloOne<double> mv7(v2);

    mv /= v1;   // Phase/double; mv value == v1 value
    ASSERT_EQ(static_cast<double>(mv), 0);
    mv2 /= mv3; // Phase/Phase; mv2 value == mv3 value
    ASSERT_EQ(static_cast<double>(mv2), 0);
    mv4 /= mv3; // Phase/Phase; mv4 value < mv3 value
    ASSERT_EQ(static_cast<double>(mv4), 0.5);
    mv5 /= v2;  // Phase/double; mv5 value > v2 value
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(mv5), 0.5);
    mv6 /= mv7; // Phase/Phase; mv6 value > mv7 value
    // Use ASSERT_DOUBLE_EQ instead of ASSERT_EQ, as ASSERT_EQ compares
    // results to too many decimal places for the double-precision result
    ASSERT_DOUBLE_EQ(static_cast<double>(mv6), 0.5);
}

TEST_F(ModuloOneTest, test_less_than)
{
    // Test operator<() returns false when value and input value are equal
    double const i = 0.4;
    double const j = 1.4;
    ModuloOne<double> mv(i);  // 0.4
    ModuloOne<double> mv2(i); // 0.4
    ModuloOne<double> mv3(j); // 0.4
    ASSERT_FALSE(mv < mv2);   // Test operator<(Modulo<T>) (input value < 1 case)
    ASSERT_FALSE(mv2 < mv);   // Test operator<(Modulo<T>) (input value < 1 case)
    ASSERT_FALSE(mv < i);     // Test operator<(T)         (input value < 1 case)
    ASSERT_FALSE(mv2 < i);    // Test operator<(T)         (input value < 1 case)
    ASSERT_FALSE(mv < mv3);   // Test operator<(Modulo<T>) (input value > 1 case)
    ASSERT_FALSE(mv < j);     // Test operator<(T)         (input value > 1 case)

    // Test operator<() returns true if value is
    // less than input value (input value < 1 case)
    double const k = 0.5;
    double const l = 0.9;
    ModuloOne<double> mv4(k); // 0.5
    ModuloOne<double> mv5(l); // 0.9
    ASSERT_TRUE(mv4 < l);     // Test operator<(T)
    ASSERT_TRUE(mv4 < mv5);   // Test operator<(Modulo<T>)

    // Test operator<() returns true if value is less
    // than modded input value (input value > 1 case)
    double const m = 1.6;
    ModuloOne<double> mv6(k); // 0.5
    ModuloOne<double> mv7(m); // 0.6
    ASSERT_TRUE(mv6 < m);     // Test operator<(T)
    ASSERT_TRUE(mv6 < mv7);   // Test operator<(Modulo<T>)

    // Test operator<() returns false if value is
    // greater than modded input value, even if value
    // is less than input value (input value > 1 case)
    double const n = 1.3;
    ModuloOne<double> mv8(k); // 0.5
    ModuloOne<double> mv9(n); // 0.3
    ASSERT_FALSE(mv8 < n);    // Test operator<(T)
    ASSERT_FALSE(mv8 < mv9);  // Test operator<(Modulo<T>)

    // Test operator<() returns true if value is less
    // than modded input value, even if value is
    // greater than input value (input value < 0 case)
    double const o = -0.1;
    ModuloOne<double>  mv10(k); // 0.5
    ModuloOne<double>  mv11(o); // 0.9
    ASSERT_TRUE(mv10 < o);      // Test operator<(T)
    ASSERT_TRUE(mv10 < mv11);   // Test operator<(Modulo<T>)

    // Test operator<() returns false if value is
    // greater than modded input value, when value is
    // greater than input value (input value < 0 case)
    double const p = -0.6;
    ModuloOne<double>  mv12(k); // 0.5
    ModuloOne<double>  mv13(p); // 0.4
    ASSERT_FALSE(mv12 < p);     // Test operator<(T)
    ASSERT_FALSE(mv12 < mv13);  // Test operator<(Modulo<T>)
}

} // namespace test
} // namespace utils
} // namespace astrotypes
} // namespace pss
