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
 *       Stored as a contiguous block af complete spectra.
 */

template<typename T, typename Alloc=std::allocator<T>>
class TimeFrequency : public MultiArray<Alloc, T, Time, Frequency>
{
    private:
        typedef MultiArray<Alloc, T, Time, Frequency> BaseT;

    public:
        typedef typename BaseT::ReducedDimensionSliceType Spectra;
        typedef typename BaseT::ConstReducedDimensionSliceType ConstSpectra;

    public:
        TimeFrequency(DimensionSize<Time>, DimensionSize<Frequency>);
        TimeFrequency(DimensionSize<Frequency>, DimensionSize<Time>);
        ~TimeFrequency();

        /// @brief return a single spectra from the specified offset
        Spectra spectra(std::size_t offset);
        ConstSpectra spectra(std::size_t offset) const;

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
        FrequencyTime(DimensionSize<Frequency>, DimensionSize<Time>);
        FrequencyTime(DimensionSize<Time>, DimensionSize<Frequency>);
        ~FrequencyTime();
};

} // namespace astrotypes
} // namespace pss
#include "detail/TimeFrequency.cpp"

#endif // PSS_ASTROTYPES_TYPES_TIMEFREQUENCY_H
