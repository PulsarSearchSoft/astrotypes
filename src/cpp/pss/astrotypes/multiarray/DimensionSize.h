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
        DimensionSize(std::size_t size = 0);
        ~DimensionSize();

        operator std::size_t();
        operator std::size_t() const;

        bool operator<(DimensionSize<Dimension> const&) const;
        bool operator==(DimensionSize<Dimension> const& s) const;
        bool operator!=(DimensionSize<Dimension> const& s) const;
        DimensionSize& operator+=(DimensionSize<Dimension> const& s);
        DimensionSize& operator++();
        DimensionSize operator++(int);

    private:
        std::size_t _size;
};


} // namespace astrotypes
} // namespace pss
#include "detail/DimensionSize.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_DIMENSIONSIZE_H
