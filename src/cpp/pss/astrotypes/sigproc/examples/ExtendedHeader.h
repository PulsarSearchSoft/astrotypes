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
#ifndef PSS_ASTROTYPES_SIGPROC_EXAMPLES_EXTENDEDHEADER_H
#define PSS_ASTROTYPES_SIGPROC_EXAMPLES_EXTENDEDHEADER_H

#include "pss/astrotypes/sigproc/Header.h"
#include "pss/astrotypes/units/Length.h"

namespace pss {
namespace astrotypes {
namespace sigproc {
namespace examples {

/**
 * @brief An example of a simple extension to the basic sigproc format
 */

class ExtendedHeader : public pss::astrotypes::sigproc::Header
{
        typedef pss::astrotypes::sigproc::Header BaseT;

    public:
        ExtendedHeader();
        ExtendedHeader(ExtendedHeader const& copy);
        ~ExtendedHeader();

        /** 
         * @brief get the calculated distance to the candidate
         * @details note the return type is a utils::Optional and should be tested
         *          before trying to use its value.
         */
        utils::Optional<boost::units::quantity<pss::astrotypes::units::Parsecs, double>> const& distance() const;

        /** 
         * @brief set the distance to the candidate
         * @param the distance to the candidate
         */
        void distance(boost::units::quantity<pss::astrotypes::units::Parsecs, double> const& d);

    private:
        HeaderField<boost::units::quantity<pss::astrotypes::units::Parsecs, double>>       _distance;
};


} // namespace examples
} // namespace sigproc
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_SIGPROC_EXAMPLES_EXTENDEDHEADER_H
