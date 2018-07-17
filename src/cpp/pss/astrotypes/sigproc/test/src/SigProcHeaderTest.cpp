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
#include "../SigProcHeaderTest.h"
#include "pss/astrotypes/sigproc/SigProcHeader.h"
#include "pss/astrotypes/types/TimeFrequency.h"
#include <sstream>


namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {


SigProcHeaderTest::SigProcHeaderTest()
    : ::testing::Test()
{
}

SigProcHeaderTest::~SigProcHeaderTest()
{
}

void SigProcHeaderTest::SetUp()
{
}

void SigProcHeaderTest::TearDown()
{
}

TEST_F(SigProcHeaderTest, test_write_read_default)
{
    SigProcHeader header;    
    std::string out_buffer;
    std::stringstream ss_out(out_buffer);
    ss_out << header;

    SigProcHeader read_header;    
    std::string in_buffer;
    std::stringstream ss_in(in_buffer);
    ss_in >> read_header;
    ASSERT_EQ(read_header, header);
}

TEST_F(SigProcHeaderTest, test_extract_info_from_time_frequency)
{
    SigProcHeader header;    
    astrotypes::TimeFrequency<uint16_t> data(astrotypes::DimensionSize<Time>(2), astrotypes::DimensionSize<Frequency>(4));
    header << data;
    ASSERT_EQ(header.number_of_channels(), 4U);
    ASSERT_EQ(header.number_of_bits(), 16U);
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
