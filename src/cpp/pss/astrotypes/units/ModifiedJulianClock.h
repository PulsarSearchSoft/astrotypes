/*
 * MIT License
 * 
 * Copyright (c) 2016 The SKA Organisation
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
// taken form cheetah class of the same name
#ifndef PSS_ASTROTYPES_UNITS_MODIFIEDJULIANCLOCK_H
#define PSS_ASTROTYPES_UNITS_MODIFIEDJULIANCLOCK_H

#include "JulianClock.h"

namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief
 *    a std::chrono compatible clock representing the NASA Modified Julian Clock
 * @details
 * 
 */

struct ModifiedJulianClock : public std::chrono::system_clock
{
    typedef julian_day duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef TimePoint<ModifiedJulianClock> time_point;
    static constexpr bool is_steady = false;
    static const julian_day diff_from_system_epoch;
    static constexpr const char* symbol = "MJD";

    static time_point now() noexcept;

};

} // namespace units
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_UNITS_MODIFIEDJULIANCLOCK_H
