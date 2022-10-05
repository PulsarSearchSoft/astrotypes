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
#ifndef PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSIZE_H
#define PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSIZE_H

#include <string>
#include <cstddef>

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      A compile time dimesion tagging of size_t
 *
 * @details
 */

template<typename Dimension>
class DimensionSize
{
    public:
        typedef Dimension dimension;

    public:
        explicit DimensionSize(std::size_t size = 0);
        ~DimensionSize();

        operator std::size_t();
        operator std::size_t() const;

        bool operator<(DimensionSize<Dimension> const&) const;
        template<typename OtherDimension>
        bool operator==(DimensionSize<OtherDimension> const& s) const;
        template<typename OtherDimension>
        bool operator!=(DimensionSize<OtherDimension> const& s) const;

        DimensionSize& operator=(std::size_t s);

        template<typename OtherDimension>
        std::size_t operator*(DimensionSize<OtherDimension> const& s) const;
        DimensionSize operator*(std::size_t s) const;
        DimensionSize operator/(std::size_t s) const;
        DimensionSize operator+(DimensionSize<Dimension> const& s) const;
        DimensionSize operator+(std::size_t s) const;
        DimensionSize operator-(DimensionSize<Dimension> const& s) const;
        DimensionSize operator-(std::size_t s) const;
        DimensionSize& operator+=(DimensionSize<Dimension> const& s);
        DimensionSize& operator++();
        DimensionSize operator++(int);

    private:
        std::size_t _size;
};


} // namespace astrotypes
} // namespace pss

namespace std {
    template<typename Dimension>
    std::string to_string(pss::astrotypes::DimensionSize<Dimension> const& size) {
        return std::to_string(static_cast<std::size_t>(size));
    }
}

template<typename T, typename Dimension>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
operator*(T s, pss::astrotypes::DimensionSize<Dimension> const& size)
{
    return s * static_cast<std::size_t>(size);
}

template<typename T, typename Dimension>
typename std::enable_if<std::is_arithmetic<T>::value, bool>::type
operator==(pss::astrotypes::DimensionSize<Dimension> const& size, T const& s)
{
    return s == static_cast<T const&>(static_cast<std::size_t>(size));
}

template<typename T, typename Dimension>
typename std::enable_if<std::is_arithmetic<T>::value, bool>::type
operator<(T s, pss::astrotypes::DimensionSize<Dimension> const& size)
{
    return s < static_cast<T const&>(static_cast<std::size_t>(size));
}

template<typename T, typename Dimension>
typename std::enable_if<std::is_arithmetic<T>::value, pss::astrotypes::DimensionSize<Dimension>>::type
operator*(pss::astrotypes::DimensionSize<Dimension> const& size, T s)
{
    return pss::astrotypes::DimensionSize<Dimension>(s * static_cast<std::size_t>(size));
}

template<typename T, typename Dimension>
typename std::enable_if<std::is_arithmetic<T>::value, pss::astrotypes::DimensionSize<Dimension>>::type
operator/(pss::astrotypes::DimensionSize<Dimension> const& size, T s)
{
    return pss::astrotypes::DimensionSize<Dimension>(static_cast<std::size_t>(size)/s);
}

template<typename T, typename Dimension>
T operator/(T s, pss::astrotypes::DimensionSize<Dimension> const& size)
{
    return s/static_cast<T const&>(static_cast<std::size_t>(size));
}

template<typename T, typename Dimension>
T operator%(T s, pss::astrotypes::DimensionSize<Dimension> const& size)
{
    return s%static_cast<T const&>(static_cast<std::size_t>(size));
}

template<typename T, typename Dimension>
pss::astrotypes::DimensionSize<Dimension> operator-(pss::astrotypes::DimensionSize<Dimension> const& size, T s)
{
    return size.pss::astrotypes::template DimensionSize<Dimension>::operator-(s);
}

template<typename T, typename Dimension>
pss::astrotypes::DimensionSize<Dimension> operator+(pss::astrotypes::DimensionSize<Dimension> const& size, T s)
{
    return size.pss::astrotypes::template DimensionSize<Dimension>::operator+(s);
}

#include "detail/DimensionSize.cpp"
#endif // PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSIZE_H
