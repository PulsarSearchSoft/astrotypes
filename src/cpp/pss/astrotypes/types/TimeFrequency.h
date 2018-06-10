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
#ifndef PSS_ASTROTYPES_TYPES_TIMEFREQUENCY_H
#define PSS_ASTROTYPES_TYPES_TIMEFREQUENCY_H

#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Frequency.h"
#include "pss/astrotypes/multiarray/MultiArray.h"
#include <memory>

namespace pss {
namespace astrotypes {

/**
 * @brief
 *       A template class representing values associated with a time and frequecny
 *       such as Stokes values or voltages.
 *
 * @details
 *       Stored as a contiguous block af complete spectrum.
 */

template<typename T, typename Alloc=std::allocator<T>>
class TimeFrequency : public MultiArray<Alloc, T, Time, Frequency>
{
    private:
        typedef MultiArray<Alloc, T, Time, Frequency> BaseT;

    public:
        typedef typename BaseT::ReducedDimensionSliceType Spectra;
        typedef typename BaseT::ConstReducedDimensionSliceType ConstSpectra;
        typedef typename BaseT::SliceType Channel;
        typedef typename BaseT::ConstSliceType ConstChannel;

    public:
        TimeFrequency(DimensionSize<Time>, DimensionSize<Frequency>);
        TimeFrequency(DimensionSize<Frequency>, DimensionSize<Time>);
        ~TimeFrequency();

        /// @brief return a single spectrum from the specified offset
        Spectra spectrum(std::size_t offset);
        ConstSpectra spectrum(std::size_t offset) const;

        /// retrun a single channel across all time samples
        Channel channel(std::size_t channel_number);
        ConstChannel channel(std::size_t channel_number) const;
};

/**
 * @brief
 *       A template class representing values associated with a time and frequecny
 *       such as Stokes values or voltages.
 *
 * @details
 *       Stored as a contiguous channels in time
 */

template<typename T, typename Alloc=std::allocator<T>>
class FrequencyTime : public MultiArray<Alloc, T, Frequency, Time>
{
    private:
        typedef MultiArray<Alloc, T, Frequency, Time> BaseT;

    public:
        typedef typename BaseT::ReducedDimensionSliceType Channel;
        typedef typename BaseT::ConstReducedDimensionSliceType ConstChannel;
        typedef Slice<false, BaseT, Frequency, Time> Spectra;
        typedef Slice<true, BaseT, Frequency, Time> ConstSpectra;

    public:
        FrequencyTime(DimensionSize<Frequency>, DimensionSize<Time>);
        FrequencyTime(DimensionSize<Time>, DimensionSize<Frequency>);
        ~FrequencyTime();

        /// retrun a single channel across all time samples
        Channel channel(std::size_t channel_number);
        ConstChannel channel(std::size_t channel_number) const;

        /// @brief return a single spectrum from the specified offset
        Spectra spectrum(std::size_t offset);
        ConstSpectra spectrum(std::size_t offset) const;
};

} // namespace astrotypes
} // namespace pss
#include "detail/TimeFrequency.cpp"

#endif // PSS_ASTROTYPES_TYPES_TIMEFREQUENCY_H
