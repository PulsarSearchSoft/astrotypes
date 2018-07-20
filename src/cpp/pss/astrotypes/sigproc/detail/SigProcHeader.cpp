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
#include "HeaderField.cpp"
#include <sstream>

namespace pss {
namespace astrotypes {
namespace sigproc {

template<typename T>
std::runtime_error SigProcHeader::parse_error(std::string const& msg, T const& msg2)
{
    std::stringstream ss(msg);
    try {
        ss << ": " << msg2;
    }
    catch(...) {
        // dont't let this method throw
    }

    return std::runtime_error(ss.str());
}

template<typename T, typename Alloc>
SigProcHeader::OStreamAdapter<astrotypes::TimeFrequency<T, Alloc>> SigProcHeader::operator<<(astrotypes::TimeFrequency<T, Alloc> const& data)
{
    _n_bits = sizeof(T);
    _n_chans = data.template size<astrotypes::DimensionSize<units::Frequency>>();
    return OStreamAdapter<astrotypes::TimeFrequency<T, Alloc>>(*this, data);
}

template<typename T>
SigProcHeader::OStreamAdapter<T>::OStreamAdapter(SigProcHeader const& h, T const& data)
    : _h(h)
    , _d(data)
{
}

template<typename T>
std::ostream& SigProcHeader::OStreamAdapter<T>::operator<<(std::ostream& os) const
{
    os << _h;
    os << _d;
    return os;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
