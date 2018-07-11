@section time_frequency Time Frequency Data

## Quick Start Example
An example where we have 8 bit data stored in spectrum order. 
Each specturm has a 100 channels and there are 10000 spectrum in each block.


~~~~{.cpp}
#include "pss/astrotypes/types/TimeFrequency.h"
#include <algorithm>

// construct a suitable object
TimeFrequency<uint8_t> time_frequency(DimensionSize<Frequency>(100), DimensionSize<Time>(10000));

// fill it with 0 (by default the data is uninitialised)
std::fill(time_frequency.begin(), time_frequency.end(), 0);

// Iterate over each spectrum using the [] bracket style interface
unsigned total_pow=0;
for(DimensionIndex<Time> i(0); i < time_frequency.template size<Time>(); ++i) {
    auto spectrum = time_frequency[i];
    for(DimensionIndex<Frequency> j(0); j < spectrum.template size<Frequency>(); ++j) {
        total_pow += spectrum[j];
    }
}

~~~~

## Would you care for a slice?
You can extract prats of a TimeFrequency chunk as a slice.

~~~~{.cpp}
#include "pss/astrotypes/types/TimeFrequency.h"
#include <algorithm>

// construct a suitable object
TimeFrequency<uint16_t> time_frequency(DimensionSize<Time>(10000), DimensionSize<Frequency>(100));

// take a slice of data for all frequencies from the  10th to the 20th spectra
auto slice = time_frequency.slice(DimesionSpan<Time>(DimensionIndex<Time>(10), DimensionIndex<Time>(20));

// slices have their own iterator interface
// e.g. fill the slice with zeros
std::fill(slice.begin(), slice.end(), 0);

// you can also specify a slice of data using an index and a width, instead of two indicies
auto slice_2 = time_frequency.slice(DimesionSpan<Time>(DimensionIndex<Time>(0), DimensionSize<Time>(10));

// you can further slice of spectra in Frequency to select a subset of the channels
auto reduced_channels_slice = slice_2.slice(DimensionIndex<Frequency>(10), DimensionSize<Frequency>(2));

~~~~

## TimeFrequency or FrequencyTime?
The difference between the two classes is in the way the data is stored. The patterns of memory access 
are often a major factor in the speed of any algorithm you may write and so your choice of class will also
be algorithm dependendent for maximal efficiency.

But you don't have to commit to a specific type. By design the two classes have identical interfaces and
so any code you write for one should work for the other seamlessly (if perhaps not at optimal efficiency).

A simple way to write a function that can work with either type is to use a template.

~~~~{.cpp}
#include "pss/astrotypes/types/TimeFrequency.h"

template<class TimeFrequencyType>
void fill_with_one(TimeFrequencyType const& data)
{
    // use any of the common TimeFrequency/FrequencyTime interface methods here
   std::fill(data.begin(), data.end(), 1U);
}
~~~~
