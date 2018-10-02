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
#ifndef PSS_ASTROTYPES_SIGPROC_ISTREAM_H
#define PSS_ASTROTYPES_SIGPROC_ISTREAM_H
#include "SigProcFormat.h"
#include "SigProcFormatAdapterSelector.h"
#include "pss/astrotypes/multiarray/TypeTraits.h"
#include <memory>

namespace pss {
namespace astrotypes {
namespace sigproc {

template<Header::DataType HeaderDataType>
class AdaptedIStream
{
        typedef typename SigProcFormatAdapterSelector<HeaderDataType>::type AdapterType;

    public:
        template<typename Stream, typename DataType>
        static void read(Stream& s, DataType& data);

    private:
        static AdapterType _adapter;
};

template<typename HeaderT>
class IStream
{
    public:
        typedef HeaderT HeaderType;

    public:
        /**
         * @brief the header object corresponding to the current stream
         */
        HeaderType const& header() const;

        // read into the provided data type using the apporopriate
        // sigproc adapter as specified in the data_types field of the
        // header
        template<typename Stream, typename DataType>
        void read(Stream& s, DataType&);

    protected:
        /// inheriting class should call this every time a new sigproc stream is opened
        //  in order to read the header
        template<typename Stream>
        void new_header(Stream& stream);

    protected:
        HeaderT _header;
};


} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/IStream.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_ISTREAM_H
