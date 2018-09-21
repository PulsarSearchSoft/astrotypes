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

template<typename SliceType>
class TimeFreqCommon : public SliceType
{
    public:
        typedef typename SliceType::template OperatorSliceType<units::Frequency>::type Channel;
        typedef typename SliceType::template ConstOperatorSliceType<units::Frequency>::type ConstChannel;
        typedef typename SliceType::template OperatorSliceType<units::Time>::type Spectra;
        typedef typename SliceType::template ConstOperatorSliceType<units::Time>::type ConstSpectra;

        using SliceType::SliceType;

    public:
        TimeFreqCommon(SliceType const& t);
        TimeFreqCommon(SliceType&& t);

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
};

/**
 * @brief
 *       A template class representing values associated with a time and frequecny
 *       such as Stokes values or voltages.
 *
 * @details
 *       Stored as a contiguous block af complete spectrum.
 */

template<typename T, typename Alloc=std::allocator<T>>
class TimeFrequency : public TimeFreqCommon<MultiArray<Alloc, T, TimeFreqCommon, units::Time, units::Frequency>>
{
    private:
        typedef TimeFreqCommon<MultiArray<Alloc, T, TimeFreqCommon, units::Time, units::Frequency>> BaseT;

    public:
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;
        typedef typename BaseT::Spectra Spectra;
        typedef typename BaseT::ConstSpectra ConstSpectra;

    public:
        TimeFrequency(DimensionSize<units::Time>, DimensionSize<units::Frequency>);
        TimeFrequency(DimensionSize<units::Frequency>, DimensionSize<units::Time>);

        /**
         * @brief The transpose constructor
         * @details copy data from a FrequencyTime object
         */
        template<typename FrequencyTimeType, typename Enable=typename std::enable_if<
                   has_exact_dimensions<FrequencyTimeType, units::Frequency, units::Time>::value>::type>
        TimeFrequency(FrequencyTimeType const&);

        ~TimeFrequency();
};

/**
 * @brief
 *       A template class representing values associated with a time and frequecny
 *       such as Stokes values or voltages.
 *
 * @details
 *       Stored as a multiple contiguous time series. This can be used in exactly the same
 *       calls as the TimeFrequency object. They are designed to be interchangable
 *       without having to rewrite any code that uses this interface.
 */
template<typename T, typename Alloc=std::allocator<T>>
class FrequencyTime : public TimeFreqCommon<MultiArray<Alloc, T, TimeFreqCommon, units::Frequency, units::Time>>
{
    private:
        typedef TimeFreqCommon<MultiArray<Alloc, T, TimeFreqCommon, units::Frequency, units::Time>> BaseT;

    public:
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;
        typedef typename BaseT::Spectra Spectra;
        typedef typename BaseT::ConstSpectra ConstSpectra;

    public:
        FrequencyTime(DimensionSize<units::Frequency>, DimensionSize<units::Time>);
        FrequencyTime(DimensionSize<units::Time>, DimensionSize<units::Frequency>);

        /**
         * @brief The transpose constructor
         * @details copy data from a TimeFrequency object
         */
        template<typename TimeFrequencyType, typename Enable=typename std::enable_if<
                   has_exact_dimensions<TimeFrequencyType, units::Time, units::Frequency>::value>::type>
        FrequencyTime(TimeFrequencyType const&);

        ~FrequencyTime();
};

template<typename Alloc, typename T>
struct has_exact_dimensions<TimeFrequency<T, Alloc>, units::Time, units::Frequency> : public std::true_type
{
};

template<typename Alloc, typename T>
struct has_exact_dimensions<FrequencyTime<T, Alloc>, units::Frequency, units::Time> : public std::true_type
{
};

template<typename Alloc, typename T>
struct has_dimension<TimeFrequency<T, Alloc>, units::Time> : public std::true_type
{
};

template<typename Alloc, typename T>
struct has_dimension<TimeFrequency<T, Alloc>, units::Frequency> : public std::true_type
{
};

template<typename Alloc, typename T>
struct has_dimension<FrequencyTime<T, Alloc>, units::Time> : public std::true_type
{
};

template<typename Alloc, typename T>
struct has_dimension<FrequencyTime<T, Alloc>, units::Frequency> : public std::true_type
{
};

} // namespace astrotypes
} // namespace pss
#include "detail/TimeFrequency.cpp"

#endif // PSS_ASTROTYPES_TYPES_TIMEFREQUENCY_H
