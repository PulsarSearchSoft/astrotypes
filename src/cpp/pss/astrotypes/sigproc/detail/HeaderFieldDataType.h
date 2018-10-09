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
#ifndef PSS_ASTROTYPES_SIGPROC_HEADERFIELDDATATYPE_H
#define PSS_ASTROTYPES_SIGPROC_HEADERFIELDDATATYPE_H
#include "pss/astrotypes/sigproc/HeaderField.h"


namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief Specilist class for the sigproc data type variable
 * @details
 */

template<typename T>
class HeaderFieldDataType : public HeaderField<T>
{
        typedef HeaderField<T> BaseT;

    public:
        HeaderFieldDataType(SigProcLabel const& header_label, Header& header);
        HeaderFieldDataType(SigProcLabel const& header_label, Header& header, T const& to_copy);

        // Warning HeaderFieldBase must be convertible to HeaderField
        HeaderFieldDataType(SigProcLabel const& header_label, Header& header, HeaderFieldDataType const&);
        ~HeaderFieldDataType();

        HeaderFieldDataType& operator=(T const& var);

        unsigned read(std::istream & is) override;

    private:
        Header& _header;
};


} // namespace sigproc
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_SIGPROC_HEADERFIELDDATATYPE_H
