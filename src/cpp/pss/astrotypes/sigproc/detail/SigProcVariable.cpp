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

// --- generic implementation ----------
template<typename T>
unsigned SigProcVariable<T>::write(std::ostream& stream, T const& var)
{
    stream.write(reinterpret_cast<const char*>(&var), sizeof(T));
    return sizeof(T);
}

template<typename T>
unsigned SigProcVariable<T>::read(std::istream& stream, T& var)
{
    stream.read(reinterpret_cast<char*>(&var), sizeof(T));
    return stream.gcount();
}

// ------- boost::units::quantity ------------------------
template<typename Unit, typename T>
unsigned SigProcVariable<boost::units::quantity<Unit, T>>::write(std::ostream& stream, boost::units::quantity<Unit, T> const& var)
{
    return SigProcVariable<T>::write(stream, var.value());
}

template<typename Unit, typename NumericalRep>
unsigned SigProcVariable<boost::units::quantity<Unit, NumericalRep>>::read(std::istream& stream, boost::units::quantity<Unit, NumericalRep>& var)
{
    stream.read(reinterpret_cast<char*>(const_cast<NumericalRep*>(&var.value())), sizeof(NumericalRep));
    return stream.gcount();
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss

namespace std {
    template<>
    struct hash<pss::astrotypes::sigproc::SigProcLabel>
    {
        std::size_t operator()(const pss::astrotypes::sigproc::SigProcLabel& quantity) const
        {
            return std::hash<std::string>()(quantity.string());
        }
    };
} // namespace std

