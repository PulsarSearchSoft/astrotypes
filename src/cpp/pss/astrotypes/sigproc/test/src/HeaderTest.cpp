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
#include "../HeaderTest.h"
#include "../SigProcTestFile.h"
#include "pss/astrotypes/sigproc/Header.h"
#include "pss/astrotypes/types/TimeFrequency.h"
#include <sstream>
#include <fstream>


namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {


HeaderTest::HeaderTest()
    : ::testing::Test()
{
}

HeaderTest::~HeaderTest()
{
}

void HeaderTest::SetUp()
{
}

void HeaderTest::TearDown()
{
}

Header save_restore(Header const& h)
{
    std::stringstream ss;
    //h.write(ss);
    ss << h;
    Header r;
    r.read(ss);
    return r;
}

TEST_F(HeaderTest, test_size)
{
    Header h;
    std::stringstream ss;
    h.write(ss);

    Header r;
    r.read(ss);

    ASSERT_EQ(r.size(), ss.str().size());
    ASSERT_EQ(r.size(), h.size());
}

TEST_F(HeaderTest, test_write_read_default)
{
    Header header;    
    Header read_header = save_restore(header);
    ASSERT_EQ(read_header, header);
}

TEST_F(HeaderTest, test_operator_eq_empty)
{
    Header header1;    
    Header header2;    
    ASSERT_TRUE(header1 == header2);
    ASSERT_FALSE(header1 != header2);
}

TEST_F(HeaderTest, test_telescope_id)
{
    unsigned s(10);

    Header h;
    ASSERT_FALSE(h.telescope_id());
    h.telescope_id(s);
    ASSERT_TRUE(h.telescope_id());
    ASSERT_EQ(h.telescope_id(), s);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.telescope_id(), h.telescope_id());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h == h2);
    ASSERT_FALSE(h.telescope_id());
}

TEST_F(HeaderTest, test_machine_id)
{
    unsigned s(20);

    Header h;
    ASSERT_FALSE(h.machine_id());
    h.machine_id(s);
    ASSERT_TRUE(h.machine_id());
    ASSERT_EQ(h.machine_id(), s);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.machine_id(), h.machine_id());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.machine_id());
}

TEST_F(HeaderTest, test_raw_data_file)
{
    std::string s("abc");

    Header h;
    ASSERT_FALSE(h.raw_data_file());
    h.raw_data_file(s);
    ASSERT_TRUE(h.raw_data_file());
    ASSERT_EQ(h.raw_data_file(), s);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.raw_data_file(), h.raw_data_file());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.raw_data_file());
}

TEST_F(HeaderTest, test_barycentric)
{
    Header h;
    ASSERT_FALSE(h.barycentric());
    h.barycentric(false);
    ASSERT_TRUE(h.barycentric());
    ASSERT_EQ(*h.barycentric(), 0U);
    Header h2 = save_restore(h);
    ASSERT_EQ(*h2.barycentric(), *h.barycentric());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.barycentric());
}

TEST_F(HeaderTest, test_pulsarcentric)
{
    Header h;
    ASSERT_FALSE(h.pulsarcentric());
    h.pulsarcentric(true);
    ASSERT_TRUE(h.pulsarcentric());
    ASSERT_EQ(*h.pulsarcentric(), 1U);
    Header h2 = save_restore(h);
    ASSERT_EQ(*h2.pulsarcentric(), *h.pulsarcentric());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.pulsarcentric());
}

TEST_F(HeaderTest, test_source_name)
{
    std::string s("abc");

    Header h;
    ASSERT_FALSE(h.source_name());
    h.source_name(s);
    ASSERT_TRUE(h.source_name());
    ASSERT_EQ(h.source_name(), s);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.source_name(), h.source_name());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.source_name());
}

TEST_F(HeaderTest, test_az_start)
{
    boost::units::quantity<units::Degree, double> angle(150 * units::degrees);

    Header h;
    ASSERT_FALSE(h.az_start());
    h.az_start(angle);
    ASSERT_TRUE(h.az_start());
    ASSERT_EQ(h.az_start(), angle);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.az_start(), h.az_start());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.az_start());
}

TEST_F(HeaderTest, test_za_start)
{
    boost::units::quantity<units::Degree, double> angle(150 * units::degrees);

    Header h;
    ASSERT_FALSE(h.za_start());
    h.za_start(angle);
    ASSERT_TRUE(h.za_start());
    ASSERT_EQ(h.za_start(), angle);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.za_start(), h.za_start());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.za_start());
}

TEST_F(HeaderTest, number_of_channels)
{
    Header h;
    ASSERT_EQ(0, h.number_of_channels());
    h.number_of_channels(332);
    ASSERT_EQ(332, h.number_of_channels());
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.number_of_channels(), h.number_of_channels());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);

    h.reset();
    ASSERT_EQ(0, h.number_of_channels());
}

TEST_F(HeaderTest, number_of_bits)
{
    Header h;
    ASSERT_EQ(0, h.number_of_bits());
    h.number_of_bits(32);
    ASSERT_EQ(32, h.number_of_bits());
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.number_of_bits(), h.number_of_bits());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_EQ(0, h.number_of_bits());
}

TEST_F(HeaderTest, test_tstart)
{
    units::MJD mjd(100.0 * units::days);

    Header h;
    ASSERT_FALSE(h.tstart());
    h.tstart(mjd);
    ASSERT_TRUE(h.tstart());
    ASSERT_EQ(h.tstart(), mjd);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.tstart(), h.tstart());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.tstart());
}

TEST_F(HeaderTest, test_sample_interval)
{
    boost::units::quantity<units::Seconds, double> zero(0.0 * units::milliseconds);
    boost::units::quantity<units::Seconds, double> t(20.0 * units::milliseconds);
    Header h;
    ASSERT_EQ(zero, h.sample_interval());
    h.sample_interval(t);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.sample_interval(), h.sample_interval());
    ASSERT_EQ(t, h.sample_interval());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_EQ(zero, h.sample_interval());
}

TEST_F(HeaderTest, test_fch1)
{
    boost::units::quantity<units::MegaHertz, double> freq(150 * units::megahertz);

    Header h;
    ASSERT_FALSE(h.fch1());
    h.fch1(freq);
    ASSERT_TRUE(h.fch1());
    ASSERT_EQ(h.fch1(), freq);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.fch1(), h.fch1());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.fch1());
}

TEST_F(HeaderTest, test_foff)
{
    boost::units::quantity<units::MegaHertz, double> freq(150 * units::megahertz);

    Header h;
    ASSERT_FALSE(h.foff());
    h.foff(freq);
    ASSERT_TRUE(h.foff());
    ASSERT_EQ(h.foff(), freq);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.foff(), h.foff());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.foff());
}

TEST_F(HeaderTest, test_frequency_channels)
{
    std::vector<boost::units::quantity<units::MegaHertz, double>> channels;
    for(double i=100.0; i < 500.0; ++i) { 
        channels.push_back( boost::units::quantity<units::MegaHertz, double>(i * units::megahertz) );
    }
    Header h;
    ASSERT_EQ(h.frequency_channels().size(), 0U);
    h.frequency_channels(channels);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.frequency_channels(), h.frequency_channels());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_EQ(h.frequency_channels().size(), 0U);
}

TEST_F(HeaderTest, test_extract_info_from_time_frequency)
{
    Header header;    
    astrotypes::TimeFrequency<uint16_t> data(astrotypes::DimensionSize<units::Time>(2), astrotypes::DimensionSize<units::Frequency>(4));
    header << data;
    ASSERT_EQ(header.number_of_bits(), 16U);
    ASSERT_EQ(4U, header.number_of_channels());
}

TEST_F(HeaderTest, test_ref_dm)
{
    units::DispersionMeasure<double> dm(100 * units::parsecs_per_cube_cm);

    Header h;
    ASSERT_FALSE(h.ref_dm());
    h.ref_dm(dm);
    ASSERT_TRUE(h.ref_dm());
    ASSERT_EQ(h.ref_dm(), dm);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.ref_dm(), h.ref_dm());
    ASSERT_TRUE(h == h2);
    ASSERT_FALSE(h != h2);
    h.reset();
    ASSERT_FALSE(h.ref_dm());
}

TEST_F(HeaderTest, test_period)
{
    boost::units::quantity<units::Seconds, double> p(100 * units::milliseconds);

    Header h;
    ASSERT_FALSE(h.period());
    h.period(p);
    ASSERT_TRUE(h.period());
    ASSERT_EQ(h.period(), p);
    Header h2 = save_restore(h);
    ASSERT_EQ(h2.period(), h.period());
    ASSERT_TRUE(h == h2);
    h.reset();
    ASSERT_FALSE(h.period());
}

TEST_F(HeaderTest, info_adapter_write)
{
    Header h;
    h.number_of_bits(16);
    h.number_of_channels(100);
    std::vector<boost::units::quantity<units::MegaHertz, double>> channels;
    for(double i=100.0; i < 140.0; ++i) { 
        channels.push_back( boost::units::quantity<units::MegaHertz, double>(i * units::megahertz) );
    }
    h.frequency_channels(channels);
    std::stringstream ss1;
    ss1 << Header::Info() << h;
    std::cout << ss1.str() << "\n";
}

TEST_F(HeaderTest, read_filterbank_file_test)
{
    Header header;
    SigProcFilterBankTestFile test_file;
    std::ifstream s(test_file.file(), std::ios::in | std::ios::binary);
    s >> header;
    ASSERT_EQ(header.number_of_bits(), 8U);
    ASSERT_EQ(header.number_of_channels(), test_file.number_of_channels());
    ASSERT_EQ(header.number_of_ifs(), test_file.number_of_ifs());
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
