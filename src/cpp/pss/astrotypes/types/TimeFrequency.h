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
class TimeFrequency : public MultiArray<Alloc, T, units::Time, units::Frequency>
{
    private:
        typedef MultiArray<Alloc, T, units::Time, units::Frequency> BaseT;

    public:
        typedef typename BaseT::ReducedDimensionSliceType Spectra;
        typedef typename BaseT::ConstReducedDimensionSliceType ConstSpectra;
        typedef typename BaseT::SliceType Channel;
        typedef typename BaseT::ConstSliceType ConstChannel;

    public:
        TimeFrequency(DimensionSize<units::Time>, DimensionSize<units::Frequency>);
        TimeFrequency(DimensionSize<units::Frequency>, DimensionSize<units::Time>);
        ~TimeFrequency();

        /// @brief return a single spectrum from the specified offset
        //  @detials the type returned is a MultiArray @class Slice with all
        //           the features of that available;
        //  @example
        //  @code
        //  // create a TimeFrequency block
        //  TimeFrequency<uint16_t> tf(DimensionSize<Time>(100), DimensionSize<Frequency>(4096));
        //
        //  // get the first spectrum
        //  typename TimeFrequency<uint16_t>::Spectra spectrum = tf.spectrum(0);
        //
        //  // use the Spectra's iterator interface tp generate an increasing sequence
        //  std::fill(spectrum.begin(), spectra.end(), 100U);
        //
        //  // use the  [] interface to access a particular element
        //  spectrum[DimensionIndex<Frequency>(7)] += 1; // modify the 8th channel value in thie specturm
        //
        //  @endcode
        //
        Spectra spectrum(std::size_t offset);
        ConstSpectra spectrum(std::size_t offset) const;

        /// retrun a single channel across all time samples
        //  @detials the type returned is a MultiArray @class Slice with all
        //           the features of that available;
        //  @example
        //  @code
        //  // create a TimeFrequency block
        //  TimeFrequency<uint16_t> tf(DimensionSize<Time>(100), DimensionSize<Frequency>(4096));
        //
        //  // get the first channel
        //  typename TimeFrequency<uint16_t>::Channel chanel = tf.channel(0);
        //
        //  // use the Chanel's iterator interface tp generate an increasing sequence
        //  // 
        //  uint16_t n = 0;
        //  std::generate(channel.begin(), channel.end(), [&]() { return ++n; });
        //  @endcode
        //
        Channel channel(std::size_t channel_number);
        ConstChannel channel(std::size_t channel_number) const;
};

/**
 * @brief
 *       A template class representing values associated with a time and frequecny
 *       such as Stokes values or voltages.
 *
 * @details
 *       Stored as a contiguous channels in time. This can be used in exactly the same
 *       calls as the TimeFrequency object. They are designed to be interchangable
 *       without having to rewrite any code that uses this interface.
 */

template<typename T, typename Alloc=std::allocator<T>>
class FrequencyTime : public MultiArray<Alloc, T, units::Frequency, units::Time>
{
    private:
        typedef MultiArray<Alloc, T, units::Frequency, units::Time> BaseT;

    public:
        typedef typename BaseT::ReducedDimensionSliceType Channel;
        typedef typename BaseT::ConstReducedDimensionSliceType ConstChannel;
        typedef Slice<false, BaseT, units::Frequency, units::Time> Spectra;
        typedef Slice<true, BaseT, units::Frequency, units::Time> ConstSpectra;

    public:
        FrequencyTime(DimensionSize<units::Frequency>, DimensionSize<units::Time>);
        FrequencyTime(DimensionSize<units::Time>, DimensionSize<units::Frequency>);
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
