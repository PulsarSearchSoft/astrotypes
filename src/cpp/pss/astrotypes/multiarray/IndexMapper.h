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
#ifndef PSS_ASTROTYPES_MULTIARRAY_INDEXMAPPER_H
#define PSS_ASTROTYPES_MULTIARRAY_INDEXMAPPER_H

#include "NumericalRepresentation.h"

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      Maps indexes from one container onto another
 *
 * @details
 * @code
 * bool   flags[100];
 * MutliContainer<2, int> c1(10, 10);
 * auto im = make_IndexMapper(flags, c1);
 *
 * auto it = c1.begin();
 * im(it) = true;
 *
 * @endcode
 */

template<typename DestinationContainer, typename SrcContainerType>
class IndexMapper
{
        typedef typename NumericalRepresentation<DestinationContainer>::type T;
        typedef typename SrcContainerType::Iterator Iterator;

    public:
        IndexMapper(DestinationContainer& container, SrcContainerType const& view);
        ~IndexMapper();

        T& operator()(IteratorType) const;

    private:
};

template<typename DestinationContainer, typename SrcContainerType>
IndexMapper<DestinationContainer, SrcContainerType> make_IndexMapper(DestinationContainer& dst, SrcContainerType const& src)
{
    return IndexMapper<DestinationContainer, SrcContainerType>(dst, src);
}

} // namespace astrotypes
} // namespace pss
#include "detail/IndexMapper.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_INDEXMAPPER_H
