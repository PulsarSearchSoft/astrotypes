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
#include "pss/astrotypes/types/ExtendedTimeFrequency.h"


namespace pss {
namespace astrotypes {
namespace types {

template<typename TimeFrequencyType>
ExtendedTimeFrequency<TimeFrequencyType>::ExtendedTimeFrequency(TimeFrequencyType const& tf)
    : BaseT(tf)
{
}

template<typename TimeFrequencyType>
template<typename, typename>
ExtendedTimeFrequency<TimeFrequencyType>::ExtendedTimeFrequency(TimeFrequencyType&& tf)
    : BaseT(std::move(tf))
{
}

template<typename TimeFrequencyType>
ExtendedTimeFrequency<TimeFrequencyType>::~ExtendedTimeFrequency()
{
}

template<typename TimeFrequencyType>
inline
typename ExtendedTimeFrequency<TimeFrequencyType>::Channel ExtendedTimeFrequency<TimeFrequencyType>::channel(std::size_t channel_number)
{
    return this->get().channel(channel_number);
}

template<typename TimeFrequencyType>
inline
typename ExtendedTimeFrequency<TimeFrequencyType>::ConstChannel ExtendedTimeFrequency<TimeFrequencyType>::channel(std::size_t channel_number) const
{
    return this->get().channel(channel_number);
}

template<typename TimeFrequencyType>
inline
typename ExtendedTimeFrequency<TimeFrequencyType>::Spectra ExtendedTimeFrequency<TimeFrequencyType>::spectrum(std::size_t offset)
{
    return this->get().spectrum(offset);
}

template<typename TimeFrequencyType>
inline
typename ExtendedTimeFrequency<TimeFrequencyType>::ConstSpectra ExtendedTimeFrequency<TimeFrequencyType>::spectrum(std::size_t offset) const
{
    return this->get().spectrum(offset);
}

template<typename TimeFrequencyType>
inline
std::size_t ExtendedTimeFrequency<TimeFrequencyType>::number_of_channels() const
{
    return this->get().number_of_channels();
}

template<typename TimeFrequencyType>
inline
std::size_t ExtendedTimeFrequency<TimeFrequencyType>::number_of_spectra() const
{
    return this->get().number_of_spectra();
}

} // namespace types
} // namespace astrotypes
} // namespace pss
