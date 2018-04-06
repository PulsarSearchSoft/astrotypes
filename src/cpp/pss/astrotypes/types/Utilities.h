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

#include "pss/astrotypes/types/TimeFrequency.h"
#include <algorithm>

#ifndef PSS_ASTROTYPES_TYPES_UTILITIES_H
#define PSS_ASTROTYPES_TYPES_UTILITIES_H

namespace pss {
namespace astrotypes {

/**
 * @brief Transpose a TimeFrequency object into a FrequencyTime object.
 *
 * @tparam T Data type.
 * @param source Object to copy data from.
 * @param destination Object to copy data to.
 */
template<typename T>
void transpose(TimeFrequency<T> & source, FrequencyTime<T> & destination);

/**
 * @brief Transpose a FrequencyTime object into a TimeFrequency object.
 *
 * @tparam T Data type.
 * @param source Object to copy data from.
 * @param destination Object to copy data to.
 */
template<typename T>
void transpose(FrequencyTime<T> & source, TimeFrequency<T> & destination);

} // astrotypes
} // pss
#include "detail/Utilities.cpp"

#endif // PSS_ASTROTYPES_TYPES_UTILITIES_H
