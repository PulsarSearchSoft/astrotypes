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

template<typename Dimension>
ResizeAdapter<Dimension>::ResizeAdapter(DimensionSize<Dimension> ts)
    : _samples(ts)
{
}

template<typename Dimension>
template<typename Stream>
ResizeAdapter<Dimension>::ResizeAdapterStream<Stream>::ResizeAdapterStream(Stream& is, ResizeAdapter const& ra)
    : _is(is)
    , _ra(ra)
{
}

template<typename Dimension>
template<typename Stream>
template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, typename ResizeAdapter<Dimension>::template ResizeAdapterStream<Stream>>::type const&
ResizeAdapter<Dimension>::ResizeAdapterStream<Stream>::operator>>(T& tf) const
{
    tf.resize(_ra._samples);
    _is >> tf;
    return *this;
}

template<typename Dimension>
template<typename Stream>
template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, typename ResizeAdapter<Dimension>::template ResizeAdapterStream<Stream>>::type const&
ResizeAdapter<Dimension>::ResizeAdapterStream<Stream>::operator>>(T& tf) const
{
    tf.resize(_ra._samples);
    _is >> tf;
    return *this;
}

template<typename Dimension, typename Stream>
inline
typename ResizeAdapter<Dimension>::template ResizeAdapterStream<Stream> operator>>(Stream& is, ResizeAdapter<Dimension>& ra)
{
    return ResizeAdapter<Dimension>::template ResizeAdapterStream<Stream>(is, ra);
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
