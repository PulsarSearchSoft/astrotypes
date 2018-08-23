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
#ifndef PSS_ASTROTYPES_SIGPROC_RESIZEADAPTER_H
#define PSS_ASTROTYPES_SIGPROC_RESIZEADAPTER_H


namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief
 *      Allows you to define how a TimeFrequency or FrequencyTime object will be resized
 *      when reading from a sigproc input stream
 */

template<typename Dimension>
class ResizeAdapter
{
    protected:
        template<typename Stream>
        struct ResizeAdapterStream {
            public:
                ResizeAdapterStream(Stream& is, ResizeAdapter const& ra);

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, ResizeAdapterStream<Stream>>::type const&
                operator>>(T&) const;

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, ResizeAdapterStream<Stream>>::type const&
                operator>>(T&) const;

            private:
                Stream& _is;
                ResizeAdapter const& _ra;
        };

    public:
        ResizeAdapter(DimensionSize<Dimension> size);
        ~ResizeAdapter() {};

        template<typename Stream>
        friend ResizeAdapterStream<Stream> operator>>(Stream& is, ResizeAdapter& ra);

    private:
        DimensionSize<Dimension> _samples;
};


} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/ResizeAdapter.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_RESIZEADAPTER_H
