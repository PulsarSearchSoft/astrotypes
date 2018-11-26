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
#include "pss/astrotypes/utils/test/OptionalTest.h"
#include "pss/astrotypes/utils/Optional.h"


namespace pss {
namespace astrotypes {
namespace utils {
namespace test {


OptionalTest::OptionalTest()
    : ::testing::Test()
{
}

OptionalTest::~OptionalTest()
{
}

void OptionalTest::SetUp()
{
}

void OptionalTest::TearDown()
{
}

TEST_F(OptionalTest, test_int)
{
    Optional<int> opt; 
    if(opt.is_set()) {
        FAIL() << "expected to be marked unset";
    }
    if(opt) {
        FAIL() << "expected to be marked unset";
    }

    opt = 10;
    ASSERT_TRUE(opt.is_set());
    ASSERT_TRUE(opt);
    ASSERT_EQ(opt, 10);
    ASSERT_NE(opt, 11);

    // test operator*
    ASSERT_EQ(*opt, 10);

    // test reset
    opt.reset();
    ASSERT_FALSE(opt.is_set());
}

TEST_F(OptionalTest, test_const_int)
{
    const Optional<int> const_opt(100); 
    ASSERT_TRUE(const_opt.is_set());
    ASSERT_TRUE(const_opt);
    ASSERT_EQ(*const_opt, 100);
    ASSERT_EQ(const_opt, 100);
    ASSERT_NE(const_opt, 11);
}

TEST_F(OptionalTest, test_const_bool)
{
    const Optional<bool> const_opt(true); 
    ASSERT_TRUE(const_opt.is_set());
    ASSERT_TRUE(const_opt);
    ASSERT_EQ(const_opt, true);
    ASSERT_NE(const_opt, false);

    Optional<bool> false_opt(false); 
    ASSERT_TRUE(false_opt.is_set());
    ASSERT_FALSE(false_opt);
    ASSERT_NE(false_opt, true);
    ASSERT_EQ(false_opt, false);
    false_opt.reset();
    ASSERT_FALSE(false_opt.is_set());
}

} // namespace test
} // namespace utils
} // namespace astrotypes
} // namespace pss
