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

SigProcHeader save_restore(SigProcHeader const& h)
{
    std::stringstream ss;
    h.write(ss);
    SigProcHeader r;
    r.read(ss);
    return r;
}

TEST_F(SigProcHeaderTest, test_size)
{
    SigProcHeader h;
    std::stringstream ss;
    h.write(ss);

    SigProcHeader r;
    r.read(ss);

    ASSERT_EQ(r.size(), ss.str().size());
    ASSERT_EQ(r.size(), h.size());
}

TEST_F(SigProcHeaderTest, test_write_read_default)
{
    SigProcHeader header;    
    SigProcHeader read_header = save_restore(header);
    ASSERT_EQ(read_header, header);
}

TEST_F(SigProcHeaderTest, test_telescope_id)
{
    unsigned s(10);

    SigProcHeader h;
    ASSERT_FALSE(h.telescope_id());
    h.telescope_id(s);
    ASSERT_TRUE(h.telescope_id());
    ASSERT_EQ(h.telescope_id(), s);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.telescope_id(), h.telescope_id());
    h.reset();
    ASSERT_FALSE(h.telescope_id());
}

TEST_F(SigProcHeaderTest, test_machine_id)
{
    unsigned s(20);

    SigProcHeader h;
    ASSERT_FALSE(h.machine_id());
    h.machine_id(s);
    ASSERT_TRUE(h.machine_id());
    ASSERT_EQ(h.machine_id(), s);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.machine_id(), h.machine_id());
    h.reset();
    ASSERT_FALSE(h.machine_id());
}

TEST_F(SigProcHeaderTest, test_raw_data_file)
{
    std::string s("abc");

    SigProcHeader h;
    ASSERT_FALSE(h.raw_data_file());
    h.raw_data_file(s);
    ASSERT_TRUE(h.raw_data_file());
    ASSERT_EQ(h.raw_data_file(), s);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.raw_data_file(), h.raw_data_file());
    h.reset();
    ASSERT_FALSE(h.raw_data_file());
}

TEST_F(SigProcHeaderTest, test_barycentric)
{
    SigProcHeader h;
    ASSERT_FALSE(h.barycentric());
    h.barycentric(false);
    ASSERT_TRUE(h.barycentric());
    ASSERT_EQ(*h.barycentric(), 0U);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(*h2.barycentric(), *h.barycentric());
    h.reset();
    ASSERT_FALSE(h.barycentric());
}

TEST_F(SigProcHeaderTest, test_pulsarcentric)
{
    SigProcHeader h;
    ASSERT_FALSE(h.pulsarcentric());
    h.pulsarcentric(true);
    ASSERT_TRUE(h.pulsarcentric());
    ASSERT_EQ(*h.pulsarcentric(), 1U);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(*h2.pulsarcentric(), *h.pulsarcentric());
    h.reset();
    ASSERT_FALSE(h.pulsarcentric());
}

TEST_F(SigProcHeaderTest, test_source_name)
{
    std::string s("abc");

    SigProcHeader h;
    ASSERT_FALSE(h.source_name());
    h.source_name(s);
    ASSERT_TRUE(h.source_name());
    ASSERT_EQ(h.source_name(), s);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.source_name(), h.source_name());
    h.reset();
    ASSERT_FALSE(h.source_name());
}

TEST_F(SigProcHeaderTest, number_of_channels)
{
    SigProcHeader h;
    ASSERT_EQ(0, h.number_of_channels());
    h.number_of_channels(332);
    ASSERT_EQ(332, h.number_of_channels());
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.number_of_channels(), h.number_of_channels());

    h.reset();
    ASSERT_EQ(0, h.number_of_channels());
}

TEST_F(SigProcHeaderTest, number_of_bits)
{
    SigProcHeader h;
    ASSERT_EQ(0, h.number_of_bits());
    h.number_of_bits(32);
    ASSERT_EQ(32, h.number_of_bits());
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.number_of_bits(), h.number_of_bits());
    h.reset();
    ASSERT_EQ(0, h.number_of_bits());
}

TEST_F(SigProcHeaderTest, test_start_time)
{
/*
    utils::TimePoint<utils::ModifiedJulianClock> epoch(utils::julian_day(0));
    utils::TimePoint<utils::ModifiedJulianClock> t(utils::julian_day(12));
    SigProcHeader h;
    ASSERT_EQ(epoch, h.start_time());
    h.set_start_time(t);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.start_time(), h.start_time());
    ASSERT_EQ(t, h.start_time());
    h.reset();
    ASSERT_EQ(epoch, h.start_time());
*/
}

TEST_F(SigProcHeaderTest, test_sample_interval)
{
    boost::units::quantity<Seconds, double> zero(0.0 * milliseconds);
    boost::units::quantity<Seconds, double> t(20.0 * milliseconds);
    SigProcHeader h;
    ASSERT_EQ(zero, h.sample_interval());
    h.sample_interval(t);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.sample_interval(), h.sample_interval());
    ASSERT_EQ(t, h.sample_interval());
    h.reset();
    ASSERT_EQ(zero, h.sample_interval());
}


TEST_F(SigProcHeaderTest, test_extract_info_from_time_frequency)
{
    SigProcHeader header;    
    astrotypes::TimeFrequency<uint16_t> data(astrotypes::DimensionSize<Time>(2), astrotypes::DimensionSize<Frequency>(4));
    header << data;
    ASSERT_EQ(header.number_of_channels(), 4U);
    ASSERT_EQ(header.number_of_bits(), 16U);
}

TEST_F(SigProcHeaderTest, test_ref_dm)
{
    DispersionMeasure<double> dm(100 * parsecs_per_cube_cm);

    SigProcHeader h;
    ASSERT_FALSE(h.ref_dm());
    h.ref_dm(dm);
    ASSERT_TRUE(h.ref_dm());
    ASSERT_EQ(h.ref_dm(), dm);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.ref_dm(), h.ref_dm());
    h.reset();
    ASSERT_FALSE(h.ref_dm());
}

TEST_F(SigProcHeaderTest, test_period)
{
    boost::units::quantity<Seconds, double> p(100 * milliseconds);

    SigProcHeader h;
    ASSERT_FALSE(h.period());
    h.period(p);
    ASSERT_TRUE(h.period());
    ASSERT_EQ(h.period(), p);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.period(), h.period());
    h.reset();
    ASSERT_FALSE(h.period());
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
