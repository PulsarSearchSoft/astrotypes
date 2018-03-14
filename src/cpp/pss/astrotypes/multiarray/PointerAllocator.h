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

#include "StandardAllocator.h"

#ifndef ASTROTYPES_POINTERALLOCATOR_H
#define ASTROTYPES_POINTERALLOCATOR_H

namespace pss {
namespace astrotypes {

template<typename LocalType>
class PointerAllocator : public StandardAllocator<LocalType>
{
public:
  using value_type = LocalType;

  explicit PointerAllocator(LocalType * pointer) noexcept
  {
    _pointer = pointer;
  }

  value_type * allocate(std::size_t size)
  {
    return _pointer;
  }
  void deallocate(value_type * pointer, std::size_t size) noexcept {}

private:
  value_type * _pointer = nullptr;
};

template<typename FirstType, typename SecondType>
bool operator==(PointerAllocator<FirstType> const &, PointerAllocator<SecondType> const &) noexcept
{
  return true;
}


template<typename FirstType, typename SecondType>
bool operator!=(PointerAllocator<FirstType> const &, PointerAllocator<SecondType> const &) noexcept
{
  return false;
}

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_POINTERALLOCATOR_H
