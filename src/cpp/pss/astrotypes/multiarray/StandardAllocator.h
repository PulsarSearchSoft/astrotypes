/**
 * @file StandardAllocator.h
 * @brief Standard memory allocator.
 */
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

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <memory>
#include <new>
#include <cassert>

#ifndef ASTROTYPES_STANDARDALLOCATOR_H
#define ASTROTYPES_STANDARDALLOCATOR_H

namespace pss {
namespace astrotypes {

/**
 * @class template<typename LocalType> class StandardAllocator
 * @brief Minimal allocator.
 */
template<typename LocalType>
class StandardAllocator
{
public:
  using value_type = LocalType;

  StandardAllocator() noexcept = default;
  template<typename OtherType> StandardAllocator(StandardAllocator<OtherType> const &) noexcept {}

  /**
   * @fn value_type * allocate(std::size_t size)
   * @brief Allocate an area of contiguos memory.
   * @param size Number of elements that the allocated memory area should fit
   * @return Pointer to the allocated memory area
   */
  value_type * allocate(std::size_t size)
  {
    return static_cast<value_type *>(::operator new(size * sizeof(value_type)));
  }
  /**
   * @fn void deallocate(value_type * pointer, std::size_t size) noexcept
   * @brief Free a previously allocated memory area.
   * @param pointer Pointer to the memory area to deallocate
   * @param size Number of elements that the allocated memory area should fit
   */
  void deallocate(value_type * pointer, std::size_t size) noexcept
  {
    ::operator delete(pointer);
  }
};

/**
 * @fn template<typename FirstType, typename SecondType> bool operator==(StandardAllocator<FirstType> const &, StandardAllocator<SecondType> const &) noexcept
 * @brief Allows to test for equivalence of StandardAllocator objects.
 */
template<typename FirstType, typename SecondType>
bool operator==(StandardAllocator<FirstType> const &, StandardAllocator<SecondType> const &) noexcept
{
  return true;
}

/**
 * @fn template<typename FirstType, typename SecondType> bool operator!=(StandardAllocator<FirstType> const &, StandardAllocator<SecondType> const &) noexcept
 * @brief Allows to test for diversity of StandardAllocator objects.
 */
template<typename FirstType, typename SecondType>
bool operator!=(StandardAllocator<FirstType> const &, StandardAllocator<SecondType> const &) noexcept
{
  return false;
}

} // namespace astrotypes
} // namespace pss

#endif // ASTROTYPES_STANDARDALLOCATOR_H
