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
for(DimensionIndex<Time> i(0); i < time_frequency.dimension<Time>(); ++i) {
    auto spectrum = time_frequency[i];
    for(DimensionIndex<Frequency> j(0); j < spectrum.dimension<Frequency>(); ++j) {
        total_pow += spectrum[j];
    }
}

~~~~
C++ Note: If you are writing this as a template function, where the data type is one of the template paramters, you 
will need to insert the template keyword when you call the dimension method. e.g.
~~~~{.cpp}
i < time_frequency.template dimension<Time>()
~~~~

## Loading in FilterBank files
You can easily load in sigproc (Filterbank or TimeSeries) files
e.g.
~~~~{.cpp}
#include "pss/astrotypes/types/SigProc.h"
sigproc::FileReader<> filterbank_file("my_filterbank_file.fil");
TimeFrequency<uint8_t> time_frequency;

// The ResizeAdapter resizes the time_frequency object according to the filterbank files parameters and file size
filterbank >> ResizeAdapter<units::Time, units::Frequency>() >> time_frequency;
~~~~
See the \ref sigproc "sigproc" section for many more options.

## Would you care for a slice\?
You can extract parts of a TimeFrequency chunk as a slice. The Slice provides a view of the underlying
data and does not make any copies.

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

## Overlay slice iterators on other objects
The slice iterator allows you to smoothly iterate across a slice, automatically calculating the position
in the data structure. You can use the same iterator to access elements in alternative data structures
(with the same dimensionality) using the slice iterator overlays.

~~~~{.cpp}
    uint16_t& first = *slice.begin();                     // the first element of the slice
    uint8_t& uint8_first = *slice.begin()(my_uint8_data); // the first element of a corresponding slice in my_uint8_data
~~~~

## TimeFrequency or FrequencyTime?
The difference between the two classes is in the way the data is stored. The patterns of memory access 
are often a major factor in the speed of any algorithm you may write and so your choice of class will also
be algorithm dependendent for maximal efficiency.

But you don't have to commit to a specific type. By design the two classes have identical interfaces and
so any code you write for one should work for the other seamlessly (if perhaps not at optimal efficiency).

You can convert between the two types using the tranpose constructor. e.g.
~~~~{.cpp}
// example conversion from TimeFrequency ordering to FrequencyTime ordering of 8bit data
TimeFrequency<uint8_t> time_frequency(DimensionSize<Frequency>(100), DimensionSize<Time>(10000));
FrequencyTime<uint8_t> frequency_time(time_frequency); // all the data will be copied from the time_frequency object
~~~~

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
