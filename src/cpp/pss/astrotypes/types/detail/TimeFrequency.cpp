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

template<typename T, typename Alloc>
TimeFrequency<T, Alloc>::TimeFrequency()
    : BaseT(DimensionSize<units::Time>(0), DimensionSize<units::Frequency>(0))
{
}

template<typename T, typename Alloc>
TimeFrequency<T, Alloc>::TimeFrequency(DimensionSize<units::Time> time_size, DimensionSize<units::Frequency> freq_size)
    : BaseT(time_size, freq_size)
{
}

template<typename T, typename Alloc>
TimeFrequency<T, Alloc>::TimeFrequency(DimensionSize<units::Frequency> freq_size, DimensionSize<units::Time> time_size)
    : BaseT(time_size, freq_size)
{
}

template<typename T, typename Alloc>
template<typename FrequencyTimeType, typename Enable>
TimeFrequency<T, Alloc>::TimeFrequency(FrequencyTimeType const& ft)
    : BaseT(ft)
{
}

template<typename T, typename Alloc>
TimeFrequency<T, Alloc>::~TimeFrequency()
{
}

// ***************************************************************
// --------------    FrequencyTime    ----------------------------
// ***************************************************************
//
template<typename T, typename Alloc>
FrequencyTime<T, Alloc>::FrequencyTime()
    : BaseT(DimensionSize<units::Frequency>(0), DimensionSize<units::Time>(0))
{
}

template<typename T, typename Alloc>
FrequencyTime<T, Alloc>::FrequencyTime(DimensionSize<units::Frequency> freq_size, DimensionSize<units::Time> time_size)
    : BaseT(freq_size, time_size)
{
}

template<typename T, typename Alloc>
FrequencyTime<T, Alloc>::FrequencyTime(DimensionSize<units::Time> time_size, DimensionSize<units::Frequency> freq_size)
    : BaseT(freq_size, time_size)
{
}

template<typename T, typename Alloc>
template<typename TimeFrequencyType, typename Enable>
FrequencyTime<T, Alloc>::FrequencyTime(TimeFrequencyType const& tf)
    : BaseT(tf)
{
}

template<typename T, typename Alloc>
FrequencyTime<T, Alloc>::~FrequencyTime()
{
}

template<typename SliceType>
TimeFreqCommon<SliceType>::TimeFreqCommon()
{
}

template<typename SliceType>
TimeFreqCommon<SliceType>::TimeFreqCommon(TimeFreqCommon const& t)
    : SliceType(t)
{
}

template<typename SliceType>
TimeFreqCommon<SliceType>::TimeFreqCommon(SliceType const& t)
    : SliceType(t)
{
}

template<typename SliceType>
TimeFreqCommon<SliceType>& TimeFreqCommon<SliceType>::operator=(TimeFreqCommon const& t)
{
    static_cast<SliceType&>(*this) = static_cast<SliceType const&>(t);
    return *this;
}

template<typename SliceType>
TimeFreqCommon<SliceType>::TimeFreqCommon(SliceType&& t)
    : SliceType(std::move(t))
{
}

template<typename SliceType>
typename TimeFreqCommon<SliceType>::Channel TimeFreqCommon<SliceType>::channel(std::size_t channel_number)
{
    return (*this)[DimensionIndex<units::Frequency>(channel_number)];
}

template<typename SliceType>
typename TimeFreqCommon<SliceType>::ConstChannel TimeFreqCommon<SliceType>::channel(std::size_t channel_number) const
{
    return (*this)[DimensionIndex<units::Frequency>(channel_number)];
}

template<typename SliceType>
typename TimeFreqCommon<SliceType>::Spectra TimeFreqCommon<SliceType>::spectrum(std::size_t offset)
{
    return (*this)[DimensionIndex<units::Time>(offset)];
}

template<typename SliceType>
typename TimeFreqCommon<SliceType>::ConstSpectra TimeFreqCommon<SliceType>::spectrum(std::size_t offset) const
{
    return (*this)[DimensionIndex<units::Time>(offset)];
}

template<typename SliceType>
std::size_t TimeFreqCommon<SliceType>::number_of_channels() const
{
    return this->template dimension<units::Frequency>();
}

template<typename SliceType>
std::size_t TimeFreqCommon<SliceType>::number_of_spectra() const
{
    return this->template dimension<units::Time>();
}

} // namespace astrotypes
} // namespace pss
