/*
 * MIT License
 * 
 * Copyright (c) 2016 The SKA organisation
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

/// macro to easily define std::chrono compaitble clocks
//  that use utils::TimePoint<> objects as the time_point
//  defined as a template class to work  around OAT rules 
//  (i.e we can have a header only lib)
//
#include "TimePoint.h"
#include <chrono>

#ifndef PSS_UNITS_DEFINE_CLOCK
#define PSS_UNITS_DEFINE_CLOCK(_name_, duration_before_epoch, _symbol_, _is_steady_) \
namespace {                                                 \
    template<typename TimePoint>                            \
    struct _name_##_implementation_t : public std::chrono::system_clock    \
    {                                                       \
        typedef TimePoint time_point;                       \
        typedef decltype(duration_before_epoch) duration;   \
        static const duration diff_from_system_epoch;       \
        static time_point now() noexcept {                  \
               return time_point(std::chrono::system_clock::now());   \
        }                                                   \
    };                                                      \
                                                            \
    template<typename TimePoint>                            \
    const typename _name_##_implementation_t<TimePoint>::duration \
    _name_##_implementation_t<TimePoint>::diff_from_system_epoch(duration_before_epoch); \
};                                                          \
                                                            \
struct _name_ : public _name_##_implementation_t<TimePoint<_name_, decltype(duration_before_epoch)>> \
{                                                           \
    private:                                                \
        typedef _name_##_implementation_t<TimePoint<_name_, decltype(duration_before_epoch)>> BaseT; \
    public:                                                 \
        typedef typename BaseT::duration duration;          \
        typedef typename BaseT::time_point time_point;      \
        typedef duration::rep rep;                          \
        typedef duration::period period;                    \
        static constexpr bool is_steady = _is_steady_;      \
        static constexpr const char* symbol="_symbol_";     \
                                                            \
}

#endif // PSS_UNITS_DEFINE_CLOCK
