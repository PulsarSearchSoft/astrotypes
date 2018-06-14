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

template<typename T>
void transpose(TimeFrequency<T> & source, FrequencyTime<T> & destination)
{
    for ( DimensionIndex<Time> index_time(0); index_time < source.template size<Time>(); index_time++ ) {
        for ( DimensionIndex<Frequency> index_frequency(0); index_frequency < source.template size<Frequency>(); index_frequency++ ) {
            destination[index_frequency][index_time] = source[index_time][index_frequency];
        }
    }
}

template<typename T>
void transpose(FrequencyTime<T> & source, TimeFrequency<T> & destination)
{
    for ( DimensionIndex<Frequency> index_frequency(0); index_frequency < source.template size<Frequency>(); index_frequency++ ) {
        for ( DimensionIndex<Time> index_time(0); index_time < source.template size<Time>(); index_time++ ) {
            destination[index_time][index_frequency] = source[index_frequency][index_time];
        }
    }
}

} // namespace astrotypes
} // namespace pss
