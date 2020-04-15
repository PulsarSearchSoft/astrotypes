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

#include "sigproc/HardCodedDataPath.h"
#include "pss/astrotypes/units/ModifiedJulianClock.h"

namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {


inline SigProcTestFile::SigProcTestFile(std::string const& file)
    : _file(file)
{
}

inline void SigProcTestFile::file(std::string const& filename) 
{
   _file = filename;
}

inline std::string SigProcTestFile::file() const
{
    return test_data_path() + "/" + _file;
}

inline DimensionSize<units::Frequency> SigProcTestFile::number_of_channels() const
{
    return _nchans;
}

inline DimensionSize<units::Time> SigProcTestFile::number_of_spectra() const
{
    return _nsamples;
}

inline std::size_t SigProcTestFile::number_of_ifs() const
{
    return _nifs;
}

inline units::ModifiedJulianClock::time_point SigProcTestFile::start_time() const
{
    return _start_time;
}

inline SigProcFilterBankTestFile::SigProcFilterBankTestFile()
    : BaseT("filterbank_8bit.fil")
{
    this->_nchans=8;
    this->_nsamples=128;
    this->_nifs=1;
    this->_start_time=typename units::ModifiedJulianClock::time_point() + units::julian_day(5000.0);
}

inline void SigProcFilterBankTestFile::operator ++() 
{ 
  i++;
  if(i==1) file("filterbank_16bit.fil");
  if(i==2) file("filterbank_32bit.fil");
  if(i==3) file("filterbank_8bit.fil");
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
