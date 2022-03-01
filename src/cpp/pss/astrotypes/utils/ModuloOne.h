/*
 * MIT License
 * 
 * Copyright (c) 2022 PulsarSearchSoft
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

#ifndef PSS_ASTROTYPES_UTILS_MODULOONE_H
#define PSS_ASTROTYPES_UTILS_MODULOONE_H

#include <cmath>
#include <boost/units/quantity.hpp>

namespace pss {
namespace astrotypes {
namespace utils {

/**
 * @brief Represetation of a number Modulo one (1.0)
 *
 * @details This class will return the result of modding an input value with
 *          one (1.0), i.e. it will return the decimal part of a number minus
 *          the integer part for positive values (1.2 becomes 0.2), or return
 *          (1.0 + value) for negative values (e.g. -0.2 becomes 0.8).
 */

template<typename T>
class ModuloOne
{
    public:
        ModuloOne() = default;
        ModuloOne(T const& value);
        ModuloOne(ModuloOne const&) = default;
        ModuloOne(ModuloOne&&) = default;

        operator T const& () const;

        ModuloOne& operator=(T const&);
        ModuloOne& operator=(ModuloOne<T> const&);
        ModuloOne& operator+=(T const&);
        ModuloOne& operator+=(ModuloOne<T> const&);
        ModuloOne& operator-=(T const&);
        ModuloOne& operator-=(ModuloOne<T> const&);

        bool operator==(T const&) const;
        bool operator!=(T const&) const;
        bool operator==(ModuloOne<T> const&) const;
        bool operator!=(ModuloOne<T> const&) const;

        const ModuloOne& operator++() const;
        const ModuloOne& operator++(int) const;
        const ModuloOne& operator--() const;
        const ModuloOne& operator--(int) const;

        ModuloOne operator+(T const&) const;
        ModuloOne operator-(T const&) const;
        ModuloOne operator+(ModuloOne<T> const&) const;
        ModuloOne operator-(ModuloOne<T> const&) const;

        ModuloOne operator*(T const&);
        ModuloOne operator/(T const&);
        ModuloOne operator*(ModuloOne<T> const&);
        ModuloOne operator/(ModuloOne<T> const&);

        ModuloOne operator*=(T const&);
        ModuloOne operator/=(T const&);
        ModuloOne operator*=(ModuloOne<T> const&);
        ModuloOne operator/=(ModuloOne<T> const&);

        bool operator<(T const&) const;
        bool operator<(ModuloOne<T> const&) const;

    private:
        T _val;
};

} // namespace utils
} // namespace astrotypes
} // namespace pss

#include "detail/ModuloOne.cpp"

#endif // PSS_ASTROTYPES_UTILS_MODULOONE_H
