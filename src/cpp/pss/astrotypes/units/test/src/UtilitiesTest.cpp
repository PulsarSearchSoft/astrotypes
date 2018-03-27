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

#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Utilities.h"
#include "../UtilitiesTest.h"

namespace pss {
namespace astrotypes {
namespace test {

void UtilitiesTest::SetUp() {}

void UtilitiesTest::TearDown() {}

UtilitiesTest::UtilitiesTest() {}

UtilitiesTest::~UtilitiesTest() {}

TEST(UtilitiesTest, test_hash)
{
    double value = 12345.6;
    pss::astrotypes::TimeQuantity<double> quantity(value * pss::astrotypes::seconds);
    typedef boost::units::quantity<boost::units::si::time, double> QuantityType;
    ASSERT_EQ(std::hash<double>()(value), std::hash<QuantityType>()(quantity));
    // TODO
    //ASSERT_EQ(std::hash<double>()(value), std::hash<pss::astrotypes::Seconds<double>>()(quantity));
}

} // namespace test
} // namespace astrotypes
} // namespace pss

