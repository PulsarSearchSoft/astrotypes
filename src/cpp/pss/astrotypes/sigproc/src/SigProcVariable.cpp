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
#include "pss/astrotypes/sigproc/SigProcVariable.h"


namespace pss {
namespace astrotypes {
namespace sigproc {

// ------- std::string ------------------------

unsigned SigProcVariable<std::string>::write(std::ostream& stream, std::string const& var)
{
    int32_t size=static_cast<int32_t>(var.size());
    if( size > 80 ) {
        throw std::runtime_error("SigProc::write: illegal size of string (max 80): " + std::to_string(size));
    }
    // write out the size of the string
    unsigned s = SigProcVariable<decltype(size)>::write(stream, size);
    stream.write(var.data(), size);
    return size + s;
}

unsigned SigProcVariable<std::string>::read(std::istream& stream, std::string& var)
{
    // read in the size of the string
    int32_t size=0;
    stream.read(reinterpret_cast<char*>(&size), sizeof(size));
    auto s = sizeof(int32_t);

    if( size < 0 || size > 80 ) {
        std::runtime_error e("SigProcVariable<std::string>::read: illegal size of string: " + std::to_string(size));
        throw e;
    }

    if(!stream.fail()) {
        var.resize(size);
        stream.read(&var[0], size);
        s += stream.gcount();
    }
    return s;
}

void SigProcLabel::write(std::ostream& stream) const
{
    SigProcVariable<std::string>::write(stream, _string);
}

void SigProcLabel::read(std::istream& stream)
{
    SigProcVariable<std::string>::read(stream, _string);
}

std::istream& operator>>(std::istream& stream, SigProcLabel& var)
{
    var.read(stream);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, SigProcLabel const& var)
{
    var.write(stream);
    return stream;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss


