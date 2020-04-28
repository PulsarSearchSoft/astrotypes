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
#ifndef PSS_ASTROTYPES_SIGPROC_TEST_SIGPROCTESTFILE_H
#define PSS_ASTROTYPES_SIGPROC_TEST_SIGPROCTESTFILE_H
#include "pss/astrotypes/multiarray/DimensionSize.h"
#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Frequency.h"
#include <string>


namespace pss {
namespace astrotypes {
namespace sigproc {
namespace test {

/**
 * @brief Interface for finding test files and the associated meta data used to gerneate them
 */

class SigProcTestFile
{
    public:
        SigProcTestFile(std::string const& file);
        virtual ~SigProcTestFile() {};

        // return the name of the file
        std::string file() const;

        // pamareters used in file generation
        DimensionSize<units::Frequency> number_of_channels() const;
        DimensionSize<units::Time> number_of_spectra() const;
        std::size_t number_of_ifs() const;
        units::ModifiedJulianClock::time_point start_time() const;
        std::size_t number_of_bits() const;

    protected:
        std::string _file;
        DimensionSize<units::Frequency> _nchans;
        DimensionSize<units::Time> _nsamples;
        unsigned _nifs;
        units::ModifiedJulianClock::time_point _start_time;
        unsigned _nbits;
};

template <typename T>
class SigProcFilterBankTestFile : public SigProcTestFile
{
        typedef SigProcTestFile BaseT;

    public:
        SigProcFilterBankTestFile();
};


} // namespace test
} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "src/SigProcTestFile.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_TEST_SIGPROCTESTFILE_H
