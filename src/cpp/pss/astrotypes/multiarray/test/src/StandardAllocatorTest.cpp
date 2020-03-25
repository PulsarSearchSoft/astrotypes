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

#include "StandardAllocatorTest.h"
#include "pss/astrotypes/multiarray/StandardAllocator.h"


namespace pss {
namespace astrotypes {
namespace test {


StandardAllocatorTest::StandardAllocatorTest()
    : ::testing::Test()
{
}

StandardAllocatorTest::~StandardAllocatorTest()
{
}

void StandardAllocatorTest::SetUp()
{
}

void StandardAllocatorTest::TearDown()
{
}

TEST(StandardAllocatorTest, test_std_vector)
{
  const std::size_t size = 47;
  StandardAllocator<int> standard_allocator;
  std::vector<int, StandardAllocator<int>> allocated_vector(standard_allocator);

  for ( std::size_t i = 0; i < size; i++ ) {
    allocated_vector.push_back(i);
  }
  for ( std::size_t i = 0; i < size; i++ ) {
    ASSERT_EQ(i, allocated_vector.at(i));
  }
}

} // namespace test
} // namespace astrotypes
} // namespace pss
