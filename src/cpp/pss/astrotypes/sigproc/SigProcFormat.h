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
#ifndef PSS_ASTROTYPES_SIGPROC_SIGPROCFORMAT_H
#define PSS_ASTROTYPES_SIGPROC_SIGPROCFORMAT_H

#include "pss/astrotypes/types/TimeFrequency.h"
#include <iostream>

namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief  stream adapter to convert TimeFrequency or FrequencyTime types to filterbank output
 */

template<typename Dimension1, typename Dimension2>
class SigProcFormat {
    SigProcFormat() { static_assert(true, "dimesions must be Time or Frequency for SigProcFormat"); }
};


template<typename Dimension1>
class SigProcFormat<Dimension1, Dimension1> {
    SigProcFormat() { static_assert(true, "dimesions cannot be the same for SigProcFormat"); }
};


template<>
class SigProcFormat<units::Time, units::Frequency>
{
    public:
        class OSigProcFormat {
            friend SigProcFormat;
            protected:
                OSigProcFormat(std::ostream& os) : _os(os) {}

            public:
                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, OSigProcFormat const&>::type
                operator<<(T const&) const;
                
                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time>::value, OSigProcFormat const&>::type
                operator<<(T const&) const;

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency>::value, OSigProcFormat const&>::type
                operator<<(T const&) const;

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, OSigProcFormat const&>::type
                operator<<(T const&) const;

            protected:
                std::ostream& _os;
        };

        class ISigProcFormat {
                friend SigProcFormat;
            protected:
                ISigProcFormat(std::istream& is) : _is(is) {}

            public:
                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, ISigProcFormat const&>::type
                operator>>(T&) const;
                
                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time>::value, ISigProcFormat const&>::type
                operator>>(T&) const;

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, ISigProcFormat const&>::type
                operator>>(T&) const;

            protected:
                std::istream& _is;
        };

    public:
        SigProcFormat() {}
        inline OSigProcFormat operator<<(std::ostream&) const;
        inline ISigProcFormat operator>>(std::istream&) const;
};

template<>
class SigProcFormat<units::Frequency, units::Time>
{
    public:
        class OSigProcFormat {
                friend SigProcFormat;

            protected:
                OSigProcFormat(std::ostream& os) : _os(os) {}

            public:
                template<typename T, typename Alloc>
                OSigProcFormat const& operator<<(astrotypes::TimeFrequency<T, Alloc> const&);
                
                template<typename T, typename Alloc>
                OSigProcFormat const& operator<<(astrotypes::FrequencyTime<T, Alloc> const&);

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time>::value, OSigProcFormat const&>::type
                operator<<(T const&) const;

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency>::value, OSigProcFormat const&>::type
                operator<<(T const&) const;

            protected:
                std::ostream& _os;
        };

        class ISigProcFormat {
                friend SigProcFormat;
            protected:
                ISigProcFormat(std::istream& is) : _is(is) {}

            public:
                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, ISigProcFormat const&>::type
                operator>>(T&) const;
                
                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Frequency>::value, ISigProcFormat const&>::type
                operator>>(T&) const;

                template<typename T>
                typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, ISigProcFormat const&>::type
                operator>>(T&) const;

            protected:
                std::istream& _is;
        };

    public:
        SigProcFormat() {}
        inline OSigProcFormat operator<<(std::ostream&) const;
        inline ISigProcFormat operator>>(std::istream&) const;
};

// forward declaration 
template<typename Dimension1, typename Dimension2>
typename SigProcFormat<Dimension1, Dimension2>::OSigProcFormat operator<<(std::ostream& os, SigProcFormat<Dimension1, Dimension2> const&);

template<typename Dimension1, typename Dimension2>
typename SigProcFormat<Dimension1, Dimension2>::ISigProcFormat operator>>(std::istream& os, SigProcFormat<Dimension1, Dimension2> const&);

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/SigProcFormat.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_SIGPROCFORMAT_H
