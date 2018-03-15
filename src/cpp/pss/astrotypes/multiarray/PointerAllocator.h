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

/**
 * @extends StandardAllocator<LocalType>
 * @brief Dummy allocator that stores a pointer to a previously allocated area of memory.
 */
template<typename LocalType>
class PointerAllocator : public StandardAllocator<LocalType>
{
public:
  using value_type = LocalType;

  /**
   * @brief Construct the allocator from a pointer to previously allocated memory.
   * @param pointer Pointer to previously allocated memory
   */
  explicit PointerAllocator(value_type * pointer) noexcept
  {
    _pointer = pointer;
  }

  /**
   * @brief Returns a pointer to the previously allocated memory.
   * @param size Ignored parameter, here for compatibility with allocate interface
   * @return Pointer to the previously allocated memory area
   */
  value_type * allocate(std::size_t size)
  {
    return _pointer;
  }
  /**
   * @brief Empty deallocator.
   * @param pointer Ignored parameter, here for compatibility with allocate interface
   * @param size Ignored parameter, here for compatibility with allocate interface
   */
  void deallocate(value_type * pointer, std::size_t size) noexcept {}

private:
  value_type * _pointer = nullptr;
};

/**
 * @brief Allows to test for equivalence of PointerAllocator objects.
 */
template<typename FirstType, typename SecondType>
bool operator==(PointerAllocator<FirstType> const &, PointerAllocator<SecondType> const &) noexcept
{
  return true;
}

/**
 * @brief Allows to test for diversity of PointerAllocator objects.
 */
template<typename FirstType, typename SecondType>
bool operator!=(PointerAllocator<FirstType> const &, PointerAllocator<SecondType> const &) noexcept
{
  return false;
}

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_POINTERALLOCATOR_H
