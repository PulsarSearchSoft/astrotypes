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
