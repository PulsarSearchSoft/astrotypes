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


namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {


inline SigProcTestFile::SigProcTestFile(std::string const& file)
    : _file(file)
{
}

inline std::string const& SigProcTestFile::file() const
{
    return _file;
}

inline std::size_t SigProcTestFile::number_of_channels() const
{
    return _nchans;
}

inline std::size_t SigProcTestFile::number_of_spectra() const
{
    return _nsamples;
}

inline std::size_t SigProcTestFile::number_of_ifs() const
{
    return _nifs;
}

inline SigProcFilterBankTestFile::SigProcFilterBankTestFile()
    : BaseT("minimal_filterbank.fil")
    //, BaseT::_nchans(2)
    //, BaseT::_nsamples(4)
    //, BaseT::_nifs(1)
{
}

} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
