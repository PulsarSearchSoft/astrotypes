@section sigproc (FilterBank and TimeSeries) Data File Format

## Quick Start Example
First read in the sigproc header
~~~~.cpp
using astro = pss::astrotypes;

std::ifstream in("my_file_name.fil");
astro::sigproc::Header header;

in >> header;

// print human friendly verison of the header
std::cout << astro::sigproc::Header::Info() << header;
~~~~
Now we have all the information of the header encapsulated in the header object.
We can put in some checks to see it is the data we are expecting.
~~~~.cpp
// check header is what we are expecting
if( header.data_type() != astro::sigproc::Header::DataType::FilerBank
    && header.number_of_bits() != 16
    && header.number_of_number_of_nifs() != 1
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
and all you need to do is provide a template class for your code.

@example sigproc_cat.cpp
