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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#pragma GCC diagnostic pop

#ifndef PSS_ASTROTYPES_SIGPROC_SIGPROCVARIABLE_H
#define PSS_ASTROTYPES_SIGPROC_SIGPROCVARIABLE_H

#include <iostream>

namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief
 *    Read and write variables to sigproc format  
 *
 * @details
 *    You can specialise for any types that are not yet supported.
 */

template<typename T>
struct SigProcVariable
{
    /**
     * @brief write the provided variable  to the stream
     * @return the number of bytes read
     */
    static unsigned write(std::ostream&, T const& var);

    /**
     * @brief read form the stream into the variable provided
     * @return the number of bytes read
     */
    static unsigned read(std::istream&, T& var);
};

// specialisation for std::string
template<>
struct SigProcVariable<std::string>
{
    static unsigned write(std::ostream&, std::string const& var);
    static unsigned read(std::istream&, std::string& var);
};

// specialisation for boost::quantity types
template<typename Unit, typename T>
struct SigProcVariable<boost::units::quantity<Unit, T>>
{
    static unsigned write(std::ostream&, boost::units::quantity<Unit, T> const& var);
    static unsigned read(std::istream&, boost::units::quantity<Unit, T>& var);
};

// strong typing for the Header labels
struct SigProcLabel : public std::string
{
    SigProcLabel() {}
    SigProcLabel(std::string const& s) : std::string(s) {}
    SigProcLabel(const char * s) : std::string(s) {}
    std::size_t size() const {
        return std::string::size() + sizeof(int32_t);
    }
};


} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/SigProcVariable.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_SIGPROCVARIABLE_H
