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
#include "pss/astrotypes/multiarray/Slice.h"


namespace pss {
namespace astrotypes {

// ----- single dimension specialisation
template<typename Parent, typename Dimension>
Slice<Parent, Dimension>::Slice(Parent& parent, std::pair<DimensionIndex<Dimension>, DimensionIndex<Dimension>> const& d)
    : _parent(parent)
    , _span(d.second - d.first)
    , _ptr(parent.begin() + static_cast<const std::size_t>(d.first))
{
}

template<typename Parent, typename Dimension>
Slice<Parent, Dimension>::~Slice()
{
}

template<typename Parent, typename Dimension>
template<typename Dim>
typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type Slice<Parent, Dimension>::size() const
{
    return _span;
}

template<typename Parent, typename Dimension>
template<typename Dim>
typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type Slice<Parent, Dimension>::size() const
{
    return DimensionSize<Dim>(0);
}

template<typename Parent, typename Dimension>
typename Slice<Parent, Dimension>::reference_type Slice<Parent, Dimension>::operator[](std::size_t p) const
{
    return *(_ptr + p);
}

} // namespace astrotypes
} // namespace pss
