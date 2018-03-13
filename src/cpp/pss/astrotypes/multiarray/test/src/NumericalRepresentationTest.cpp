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
#include "NumericalRepresentationTest.h"
#include "pss/astrotypes/multiarray/NumericalRepresentation.h"


namespace pss {
namespace astrotypes {
namespace test {


NumericalRepresentationTest::NumericalRepresentationTest()
    : ::testing::Test()
{
}

NumericalRepresentationTest::~NumericalRepresentationTest()
{
}

void NumericalRepresentationTest::SetUp()
{
}

void NumericalRepresentationTest::TearDown()
{
}

TEST_F(NumericalRepresentationTest, test_c_array)
{
    static_assert(std::is_same<typename NumericalRepresentation<bool[]>::type, bool>::value, "wrong type");
}

TEST_F(NumericalRepresentationTest, test_std_vector)
{
    static_assert(std::is_same<typename NumericalRepresentation<std::vector<bool>>::type, bool>::value, "wrong type");
}

TEST_F(NumericalRepresentationTest, test_std_map)
{
    static_assert(std::is_same<typename NumericalRepresentation<std::map<int, bool>>::type, bool>::value, "wrong type");
}

TEST_F(NumericalRepresentationTest, test_std_list)
{
    static_assert(std::is_same<typename NumericalRepresentation<std::list<bool>>::type, bool>::value, "wrong type");
}

} // namespace test
} // namespace astrotypes
} // namespace pss
