@section sigproc The Sigproc file format (FilterBank and TimeSeries)

## Quick Start Example
To read in any sigproc file you can use the FileReader class.
This will autmoatically convert formats from/to TimeFrequecy or FrequencyTime formats
as appropriate.

~~~~{.cpp}
#include "pss/astrotypes/sigproc/SigProc.h"
sigproc::FileReader<> filterbank_file("my_filterbank_file.fil");
TimeFrequency<uint8_t> time_frequency;

// The ResizeAdapter resizes the time_frequency object according to the filterbank files parameters and file size
filterbank >> ResizeAdapter<units::Time, units::Frequency>() >> time_frequency;
~~~~
This will work equally with FrequencyTime types. Note that any corner turns will be done automatically by the streaming
operators.

If you only want to resize the number of channels, then leave out the units::Time template parameter in the ResizeAdapter.
e.g.
~~~~{.cpp}
// read in the first 100 spectra
TimeFrequency<uint8_t> time_frequency(DimensionSize<units::Time>(100));
filterbank >> ResizeAdapter<units::Frequency>() >> time_frequency;
~~~~

### Sigproc Headers
The sigproc format has a header at the beginning of each file that describes
the binary data in the rest of the file.

We can use the FileReader as above to read this in and then you can access it via 
the \m header() method. If you want more control, you can use the headers streaming operators.
e.g.
~~~~.cpp
using astro = pss::astrotypes;

std::ifstream in("my_file_name.fil", std::ios::binary);
astro::sigproc::Header header;

in >> header;

// print human friendly verison of the header
std::cout << astro::sigproc::Header::Info() << header;
~~~~
This is how we do it in the sigproc_header executable in the examples folder.

\include src/sigproc_header.cpp

### Reading in Sigproc Data
Now we have all the information of the header encapsulated in the header object.
Note that many of the headers are optional, and to relflect this they are returned as
utils::Optional types. To use such a variable you must check it is valid first like this:
~~~~.cpp
if(header.telescope_id().is_set()) {
    std::cout << "telescope_id=" << *header.telescope_id() << "\n";
}
~~~~

We can put in some checks to see it is the data we are expecting.
~~~~.cpp
// check header is what we are expecting
// 
if( header.data_type() != astro::sigproc::Header::DataType::FilterBank // FilterBank file
    && header.number_of_bits() != 16                                   // 16 bit data
    && header.number_of_number_of_nifs() != 1                          // Stokes I only
   )
{
    throw std::runtime_error("unexpected file format");
}
~~~~

Our next task is to read in the data into a suitable data structure.
We do this using the SigProcFormat templated Adapter. The template arguments
must be consistent with the header.data_type(). 
The code below will read if FilterBank types. Swap the template parameters 
if this is TimeSeries data.

~~~~.cpp
astro::DimensionSize<astro::units::Frequency> chunk_size(1000); // a thousand samples at a time
astro::TimeFrequency<uint16_t> data(header.number_of_channels(), chunk_size);
while(!in.eof()) {
    in >> SigProcFormat<astro::units::Time, astro::units::Frequency>() >> data;
    do_something_with_the_data(data);
}
~~~~

You can also write out to a std::ostream using the same SigProcFormat class.
~~~~.cpp
// e.g. where header data_type is FilterBank
std::ofstream out("output_data.fil");
out << header;
out << SigProcFormat<astro::units::Time, astro::units::Frequency>() << data;
~~~~

## More Generic Applications
If you are writing an application that needs to handle all types of sigproc files then
you will need to generate code for each different possibilty. This is because our 
TimeFrequency data types do not, for reasons of runtime speed, inherit from a common base class.

Fortunately this is easier than it sounds. astrotypes::sigproc provides helpers to do this for you
and all you need to do is provide a template class for your code. See the sigproc_cat.cpp in the examples
directory.

[sigproc_cat example]("../examples/src/sigproc_cat.cpp")

## Non-Standard Sigproc Headers
The Sigproc format has been much abused, and people have added many non-standard headers.
A limited format such as sigproc cannot handle this in a generic way, and so if you need to
read in such custom files you will need to create your own header class.
To do so just inherit from the pss::astrotypes::sigproc::Header as in the example>

[ExtendeHeader example]("../examples/ExtendedHeader.h")

Using this new header is as simple as passing its type as a template argument to the FileReader.
~~~~{.cpp}
// Use the custom MyHeaderType class to interpret a custom sigproc header
sigproc::FileReader<MyHeaderType> filterbank_file("my_custom_filterbank_file.fil");
~~~~
