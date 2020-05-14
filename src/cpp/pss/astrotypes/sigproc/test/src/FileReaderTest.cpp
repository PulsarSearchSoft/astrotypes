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
#include "../FileReaderTest.h"
#include "../SigProcTestFile.h"
#include "pss/astrotypes/sigproc/FileReader.h"
#include "pss/astrotypes/types/TimeFrequency.h"


namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {


FileReaderTest::FileReaderTest()
    : ::testing::Test()
{
}

FileReaderTest::~FileReaderTest()
{
}

void FileReaderTest::SetUp()
{
}

void FileReaderTest::TearDown()
{
}

TEST_F(FileReaderTest, test_filterbank_file_tf_data)
{
    SigProcFilterBankTestFile<uint8_t> test_file;
    TimeFrequency<uint8_t> tf_data;
    sigproc::FileReader<> reader(test_file.file());
    ASSERT_EQ(test_file.number_of_ifs(), reader.header().number_of_ifs());
    reader >> ResizeAdapter<units::Time, units::Frequency>() >> tf_data;

    ASSERT_EQ(test_file.number_of_channels(), tf_data.dimension<astrotypes::units::Frequency>());
    ASSERT_EQ(test_file.number_of_spectra(), tf_data.dimension<astrotypes::units::Time>());
}

TEST_F(FileReaderTest, test_filterbank_file_ft_data)
{
    SigProcFilterBankTestFile<uint8_t> test_file;
    FrequencyTime<uint8_t> ft_data;
    sigproc::FileReader<> reader(test_file.file());
    ASSERT_EQ(test_file.number_of_ifs(), reader.header().number_of_ifs());

    reader >> ResizeAdapter<units::Time, units::Frequency>() >> ft_data;
    ASSERT_EQ(test_file.number_of_channels(), ft_data.dimension<astrotypes::units::Frequency>());
    ASSERT_EQ(test_file.number_of_spectra(), ft_data.dimension<astrotypes::units::Time>());
}

TEST_F(FileReaderTest, test_filterbank_file_resize_time_tf_data)
{
    SigProcFilterBankTestFile<uint8_t> test_file;
    TimeFrequency<uint8_t> tf_data(DimensionSize<units::Frequency>(0), DimensionSize<units::Time>(test_file.number_of_spectra()));
    sigproc::FileReader<> reader(test_file.file());
    reader >> ResizeAdapter<units::Time>() >> tf_data;

    ASSERT_EQ(0U, tf_data.dimension<astrotypes::units::Frequency>());
    ASSERT_EQ(test_file.number_of_spectra(), tf_data.dimension<astrotypes::units::Time>());
}

TEST_F(FileReaderTest, test_filterbank_file_resize_frequency_tf_data)
{
    SigProcFilterBankTestFile<uint8_t> test_file;
    TimeFrequency<uint8_t> tf_data(test_file.number_of_spectra(), DimensionSize<units::Frequency>(0));
    sigproc::FileReader<> reader(test_file.file());
    reader >> ResizeAdapter<units::Frequency>() >> tf_data;

    ASSERT_EQ(test_file.number_of_spectra(), tf_data.dimension<astrotypes::units::Time>()); // doesn't get overwritten
    ASSERT_EQ(test_file.number_of_channels(), tf_data.dimension<astrotypes::units::Frequency>());
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
