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

#include<cmath>

namespace pss {
namespace astrotypes {
namespace utils {

template<typename T>
ModuloOne<T>::ModuloOne(T const& val)
    : _val(std::fmod(val, 1))
{
}

template<typename T>
ModuloOne<T>::operator T const& () const
{
    return _val;
}

template<typename T>
ModuloOne<T>& ModuloOne<T>::operator=(T const& v)
{
    _val = std::fmod(v, 1);
    return *this;
}

template<typename T>
ModuloOne<T>& ModuloOne<T>::operator=(ModuloOne<T> const& v)
{
    _val = v;
    return *this;
}

template<typename T>
ModuloOne<T>& ModuloOne<T>::operator+=(T const& v)
{
    _val = std::fmod((_val + v), 1);
    return *this;
}

template<typename T>
ModuloOne<T>& ModuloOne<T>::operator-=(T const& v)
{
    T v2 = std::fmod(v, 1);
    if (_val >= v2 ) {
        _val -= v2;
    }
    else {
        _val = 1 - (v2 - _val);
    }
    return *this;
}

template<typename T>
ModuloOne<T>& ModuloOne<T>::operator+=(ModuloOne<T> const& v)
{
    _val = std::fmod((_val + static_cast<T>(v)), 1);
    return *this;
}

template<typename T>
ModuloOne<T>& ModuloOne<T>::operator-=(ModuloOne<T> const& v)
{
    return (*this) -= static_cast<T>(v);
}

template<typename T>
bool ModuloOne<T>::operator==(T const& val) const
{
    return _val == val;
}

template<typename T>
bool ModuloOne<T>::operator!=(T const& val) const
{
    return _val != val;
}

template<typename T>
inline bool ModuloOne<T>::operator==(ModuloOne<T> const& val) const
{
    return _val == static_cast<T const&>(val);
}

template<typename T>
inline bool ModuloOne<T>::operator!=(ModuloOne<T> const& val) const
{
    return _val != static_cast<T const&>(val);
}

template<typename T>
inline const ModuloOne<T>& ModuloOne<T>::operator++() const
{
    return *this;
}

template<typename T>
inline const ModuloOne<T>& ModuloOne<T>::operator++(int) const
{
    return *this;
}

template<typename T>
inline const ModuloOne<T>& ModuloOne<T>::operator--() const 
{
    return *this;
}

template<typename T>
inline const ModuloOne<T>& ModuloOne<T>::operator--(int) const
{
    return *this;
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator+(T const& v) const
{
    return ModuloOne<T>(std::fmod((_val + v), 1));
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator-(T const& v) const
{
    T v2 = std::fmod(v, 1);
    return ModuloOne<T>((_val >= v2) ? (_val - v2) : (1 - (v2 - _val)));
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator+(ModuloOne<T> const& v) const
{
    return *this + static_cast<T>(v);
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator-(ModuloOne<T> const& v) const
{
    return *this - static_cast<T>(v);
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator*(T const& v)
{
    return ModuloOne<T>(std::fmod((_val * v), 1));
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator/(T const& v)
{
    return ModuloOne<T>(std::fmod((_val/v), 1));
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator*(ModuloOne<T> const& v)
{
    return *this * static_cast<T>(v);
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator/(ModuloOne<T> const& v)
{
    return std::fmod((*this)/static_cast<T>(v), 1);
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator*=(T const& v)
{
    _val = std::fmod((_val * v), 1);
    return *this;
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator/=(T const& v)
{
    _val = std::fmod((_val/v), 1);
    return *this;
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator*=(ModuloOne<T> const& v)
{
    _val = std::fmod((_val * static_cast<T>(v)), 1);
    return *this;
}

template<typename T>
ModuloOne<T> ModuloOne<T>::operator/=(ModuloOne<T> const& v)
{
    return (*this) /= static_cast<T>(v);
}

template<typename T>
bool ModuloOne<T>::operator<(T const& v) const
{
    T v2 = v - std::trunc(v);
    return (v2 < 0) ? (_val < (1 + v2)) : (_val < v2);
}

template<typename T>
bool ModuloOne<T>::operator<(ModuloOne<T> const& v) const
{
    return operator<(static_cast<T const&>(v));
}

} // namespace utils
} // namespace astrotypes
} // namespace pss
