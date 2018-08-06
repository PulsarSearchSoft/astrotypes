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
#include "pss/astrotypes/units/test/UnitsTest.h"
#include "pss/astrotypes/units/Units.h"


namespace pss {
namespace astrotypes {
namespace units {
namespace test {


UnitsTest::UnitsTest()
    : ::testing::Test()
{
}

UnitsTest::~UnitsTest()
{
}

void UnitsTest::SetUp()
{
}

void UnitsTest::TearDown()
{
}

TEST_F(UnitsTest, test_include)
{
    // enusre by including Untis we get access to all the functinoality
    Quantity<Seconds, double> t(100 * milliseconds);
    Quantity<Hertz, double> h(100 * hertz);
    Quantity<Parsecs, double> d(100 * parsecs);
    Quantity<Degree, double> a(100 * degrees);

    // hashing functions included?
    auto res = d/t * h * a;
    std::map<Quantity<Hertz, double>, decltype(res)> m;
    m.insert(std::make_pair(h, res));
}

} // namespace test
} // namespace units
} // namespace astrotypes
} // namespace pss
