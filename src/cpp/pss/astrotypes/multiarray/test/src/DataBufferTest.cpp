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
#include "pss/astrotypes/multiarray/test/DataBufferTest.h"
#include "pss/astrotypes/multiarray/DataBuffer.h"

namespace pss {
namespace astrotypes {
namespace multiarray {
namespace test {


DataBufferTest::DataBufferTest()
    : ::testing::Test()
{
}

DataBufferTest::~DataBufferTest()
{
}

void DataBufferTest::SetUp()
{
}

void DataBufferTest::TearDown()
{
}

TEST_F(DataBufferTest, test_defualt_construct)
{
    DataBuffer<int> buffer;
    ASSERT_EQ(buffer.size(), 0U);
    ASSERT_TRUE(buffer.begin() == buffer.end());
    ASSERT_TRUE(buffer.cbegin() == buffer.cend());
}

TEST_F(DataBufferTest, test_size_constructor)
{
    DataBuffer<long int> buffer(10);
    ASSERT_EQ(buffer.size(), 10U);
    ASSERT_EQ(buffer.capacity(), 10U);
    ASSERT_EQ(std::distance(buffer.begin(), buffer.end()), 10U);
}

TEST_F(DataBufferTest, test_constructor_with_value)
{
    DataBuffer<long int> buffer(10, 99);
    ASSERT_EQ(buffer.size(), 10U);
    ASSERT_EQ(buffer.capacity(), 10U);
    ASSERT_EQ(std::distance(buffer.begin(), buffer.end()), 10U);
    for(std::size_t i=5; i < buffer.size(); ++i) {
        ASSERT_EQ(buffer[i], 99);
    }
}

TEST_F(DataBufferTest, test_size_resize_smaller)
{
    DataBuffer<char> buffer(10);
    buffer.resize(5);
    ASSERT_EQ(buffer.size(), 5U);
    ASSERT_EQ(buffer.capacity(), 10U);
    ASSERT_EQ(std::distance(buffer.begin(), buffer.end()), 5U);
    ASSERT_EQ(std::distance(buffer.cbegin(), buffer.cend()), 5U);
}

TEST_F(DataBufferTest, test_size_resize_greater)
{
    DataBuffer<char> buffer(10);
    buffer.resize(15);
    ASSERT_EQ(buffer.size(), 15U);
    ASSERT_EQ(buffer.capacity(), 15U);
    ASSERT_EQ(std::distance(buffer.begin(), buffer.end()), 15U);
}

TEST_F(DataBufferTest, test_size_resize_with_value)
{
    DataBuffer<char> buffer(10);
    buffer.resize(15, 33);
    ASSERT_EQ(buffer.size(), 15U);
    ASSERT_EQ(buffer.capacity(), 15U);
    ASSERT_EQ(std::distance(buffer.begin(), buffer.end()), 15U);
    ASSERT_EQ(std::distance(buffer.cbegin(), buffer.cend()), 15U);
    for(std::size_t i=10; i < buffer.size(); ++i) {
        ASSERT_EQ(buffer[i], 33) << "i=" << i;
    }
}

TEST_F(DataBufferTest, test_clear)
{
    DataBuffer<int> buffer(10);
    buffer.clear();
    ASSERT_EQ(buffer.size(), 0U);
    ASSERT_EQ(buffer.capacity(), 10U);
    ASSERT_TRUE(buffer.begin() == buffer.end());
}

TEST_F(DataBufferTest, test_push_back)
{
    DataBuffer<int> buffer(10);
    buffer.push_back(199);
    ASSERT_EQ(buffer.size(), 11U);
    ASSERT_GE(buffer.capacity(), 11U);
    ASSERT_EQ(buffer[10], 199);
    ASSERT_EQ(buffer.back(), 199);
}

TEST_F(DataBufferTest, test_pop_back)
{
    DataBuffer<int> buffer(10);
    buffer.pop_back();
    ASSERT_EQ(buffer.size(), 9U);
    ASSERT_EQ(buffer.capacity(), 10U);
}

TEST_F(DataBufferTest, test_emplace_back)
{
    DataBuffer<int> buffer(10);
    buffer.push_back(299);
    ASSERT_EQ(buffer.size(), 11U);
    ASSERT_GE(buffer.capacity(), 11U);
    ASSERT_EQ(buffer[10], 299);
    ASSERT_EQ(buffer.back(), 299);
}

TEST_F(DataBufferTest, test_front)
{
    DataBuffer<int> buffer(10);
    int const& f = buffer.front();
    ASSERT_EQ(&f, &buffer[0]);
    ASSERT_EQ(buffer.size(), 10U);
    ASSERT_GE(buffer.capacity(), 10U);
}

TEST_F(DataBufferTest, test_reserve)
{
    DataBuffer<int> buffer(10);
    ASSERT_EQ(buffer.size(), 10U);
    buffer.reserve(20);
    ASSERT_GE(buffer.capacity(), 20U);
    ASSERT_EQ(buffer.size(), 10U);
}

} // namespace test
} // namespace multiarray
} // namespace astrotypes
} // namespace pss
