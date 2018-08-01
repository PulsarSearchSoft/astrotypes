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
#ifndef PSS_ASTROTYPES_UNITS_J2000CLOCK_H
#define PSS_ASTROTYPES_UNITS_J2000CLOCK_H

#include "JulianClock.h"

namespace pss {
namespace astrotypes {
namespace units {

/**
 * @brief
 *    a std::chrono compatible clock representing the J2000 epock
 * @details
 *      epocj is Julian date 2451545.0 TT (Terrestrial Time), or January 1, 2000, noon TT
 */

PSS_UNITS_DEFINE_CLOCK(J2000Clock, julian_day(JulianClock::diff_from_system_epoch.count() - 2451545), J2000, false);

} // namespace units
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_UNITS_J2000CLOCK_H
