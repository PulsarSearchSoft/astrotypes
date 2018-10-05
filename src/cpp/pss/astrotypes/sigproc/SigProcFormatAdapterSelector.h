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
#ifndef PSS_ASTROTYPES_SIGPROC_SIGPROCFORMATADAPTERSELECTOR_H
#define PSS_ASTROTYPES_SIGPROC_SIGPROCFORMATADAPTERSELECTOR_H
#include "Header.h"
#include "SigProcFormat.h"

namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief static helper class to determine the type of SigProcFormat stream adapter
 *        given a DataaType from the Header::DataType
 * @code
 *      typedef typename SigProcFormatAdapterSelector<Header::FilterBank>::type AdapterType;
 * @endcode
 */
template<Header::DataType HeaderDataType>
struct SigProcFormatAdapterSelector
{};

template<>
struct SigProcFormatAdapterSelector<Header::DataType::FilterBank>
{
    typedef SigProcFormat<units::Time, units::Frequency> type;
};

template<>
struct SigProcFormatAdapterSelector<Header::DataType::TimeSeries>
{
    typedef SigProcFormat<units::Frequency, units::Time> type;
};

} // namespace sigproc
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_SIGPROC_SIGPROCFORMATADAPTERSELECTOR_H
