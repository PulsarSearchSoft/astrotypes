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

namespace pss {
namespace astrotypes {
namespace utils {


template<typename T>
inline Optional<T>::Optional()
    : _is_set(false)
{
}

template<typename T>
inline Optional<T>::Optional(T const& t)
    : _is_set(true)
    , _val(t)
{
}

template<typename T>
inline Optional<T>::Optional(T&& t)
    : _is_set(true)
    , _val(std::move(t))
{
}

template<typename T>
inline Optional<T>::~Optional()
{
}

template<typename T>
inline bool Optional<T>::is_set() const
{
    return _is_set;
}

template<typename T>
inline bool Optional<T>::operator==(Optional const& opt) const
{
    if(!_is_set) return !opt._is_set;
    if(!opt._is_set) return false;
    return _val == opt._val;
}

template<typename T>
inline bool Optional<T>::operator!=(Optional const& opt) const
{
    return !(*this == opt);
}

template<typename T>
inline bool Optional<T>::operator==(T const& opt) const
{
    if(!_is_set) return false;
    return _val == opt;
}

template<typename T>
inline bool Optional<T>::operator!=(T const& opt) const
{
    return !(*this == opt);
}

template<typename T>
T const& Optional<T>::operator*() const
{
    return _val;
}

template<typename T>
inline T& Optional<T>::operator*()
{
    return _val;
}

template<typename T>
inline void Optional<T>::reset()
{
    _is_set = false;
}

// ------- bool specialisation -------------
inline Optional<bool>::Optional()
    : _is_set(false)
{
}

inline Optional<bool>::Optional(bool t)
    : _is_set(true)
    , _val(t)
{
}

inline Optional<bool>::~Optional()
{
}

inline bool Optional<bool>::is_set() const
{
    return _is_set;
}

inline bool Optional<bool>::operator==(Optional const& opt) const
{
    if(!_is_set) return !opt._is_set;
    if(!opt._is_set) return false;
    return _val == opt._val;
}

inline bool Optional<bool>::operator!=(Optional const& opt) const
{
    return !(*this == opt);
}

inline bool Optional<bool>::operator==(bool opt) const
{
    if(!_is_set) return false;
    return _val == opt;
}

inline bool Optional<bool>::operator!=(bool opt) const
{
    return !(*this == opt);
}

inline void Optional<bool>::reset()
{
    _is_set = false;
}

} // namespace utils
} // namespace astrotypes
} // namespace pss
