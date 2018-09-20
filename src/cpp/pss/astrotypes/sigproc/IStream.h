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
#include "pss/astrotypes/multiarray/TypeTraits.h"
#include "pss/astrotypes/units/Time.h"

/*
template<typename T>
typename std::enable_if<pss::astrotypes::has_dimensions<T, pss::astrotypes::units::Time, pss::astrotypes::units::Frequency>::value,
std::istream>::type& operator>>(std::istream&, T&)
{
    // improve compile time erro messaging - we should never instantiate this function
    // as we need a SigProcFormat Adapter to know how to interpret the stream 
    static_assert(sizeof(T)==0, "attempt to use operator>> on an istream without a SigProcFormat Adapter.");
}
*/

#endif // PSS_ASTROTYPES_SIGPROC_ISTREAM_H
