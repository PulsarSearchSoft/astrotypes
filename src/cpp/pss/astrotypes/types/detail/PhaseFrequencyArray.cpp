/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
namespace types {

template<typename SliceT>
PhaseFrequencyArrayInterface<SliceT>::PhaseFrequencyArrayInterface()
{
}

template<typename SliceT>
PhaseFrequencyArrayInterface<SliceT>::PhaseFrequencyArrayInterface(PhaseFrequencyArrayInterface const& t)
    : SliceT(t)
{
}

template<typename SliceT>
PhaseFrequencyArrayInterface<SliceT>::PhaseFrequencyArrayInterface(SliceT const& t)
    : SliceT(t)
{
}

template<typename SliceT>
PhaseFrequencyArrayInterface<SliceT>::PhaseFrequencyArrayInterface(SliceT&& t)
    : SliceT(std::move(t))
{
}

template<typename SliceT>
PhaseFrequencyArrayInterface<SliceT>&
    PhaseFrequencyArrayInterface<SliceT>::operator=(PhaseFrequencyArrayInterface const& t)
{
    static_cast<SliceT&>(*this) = static_cast<SliceT const&>(t);
    return *this;
}

template<typename SliceT>
typename PhaseFrequencyArrayInterface<SliceT>::Channel
    PhaseFrequencyArrayInterface<SliceT>::channel(std::size_t channel_number)
{
    return (*this)[DimensionIndex<units::Frequency>(channel_number)];
}

template<typename SliceT>
typename PhaseFrequencyArrayInterface<SliceT>::ConstChannel
    PhaseFrequencyArrayInterface<SliceT>::channel(std::size_t channel_number) const
{
    return (*this)[DimensionIndex<units::Frequency>(channel_number)];
}

template<typename SliceT>
typename PhaseFrequencyArrayInterface<SliceT>::PhaseBin
    PhaseFrequencyArrayInterface<SliceT>::phase_bin(std::size_t phase_bin_number)
{
    return (*this)[DimensionIndex<units::PhaseAngle>(phase_bin_number)];
}

template<typename SliceT>
typename PhaseFrequencyArrayInterface<SliceT>::ConstPhaseBin
    PhaseFrequencyArrayInterface<SliceT>::phase_bin(std::size_t phase_bin_number) const
{
    return (*this)[DimensionIndex<units::PhaseAngle>(phase_bin_number)];
}

template<typename SliceT>
std::size_t PhaseFrequencyArrayInterface<SliceT>::number_of_channels() const
{
    return this->template dimension<units::Frequency>();
}

template<typename SliceT>
std::size_t PhaseFrequencyArrayInterface<SliceT>::number_of_phase_bins() const
{
    return this->template dimension<units::PhaseAngle>();
}



template<typename NumericalT, typename Alloc>
PhaseFrequencyArray<NumericalT, Alloc>::PhaseFrequencyArray()
    : BaseT(DimensionSize<units::PhaseAngle>(0)
            , DimensionSize<units::Frequency>(0)
           )
{
}

template<typename NumericalT, typename Alloc>
PhaseFrequencyArray<NumericalT, Alloc>::PhaseFrequencyArray(
         DimensionSize<units::PhaseAngle> phase_size
         , DimensionSize<units::Frequency> freq_size
        )
    : BaseT(phase_size, freq_size)
{
}

} // namespace types
} // namespace astrotypes
} // namespace pss
