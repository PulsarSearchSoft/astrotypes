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
#include "pss/astrotypes/sigproc/SigProc.h"
#include "pss/astrotypes/types/TimeFrequency.h"
#include <fstream>
void usage(const char* program_name)
{
    std::cout << "Usage:\n" 
              << "\t" << program_name << " [options] input_filterbank_files... output_filterbank_file\n"
              << "Synopsis:\n"
              << "\tCatenates all input_filterbank_files to output_filterbank_file,\n"
              << "\twith appropriatw src_file header field pointing to the first file\n"
              << "Options:\n"
              << "\t--as_time_series:  save as multiple channels (time series)\n"
              << "\t--as_filterbank :  save as multiple spectra (filterbank)\n"
              << "\t--help          : this message\n";
}

// Generates code for each DataType that we might encounter
template<typename SigProcTraits>
struct CatData
{
    static
    bool exec( pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency> number_of_channels
                 , std::ostream& output_file
                 , std::vector<std::string> files
                 , std::ifstream& input_file
                 , bool as_time_series)
    {
        // we choose to load in data 1000 samples at a time to keep down the memory requirements of the app
        // Note that the number of samples in the file does not have to be a multiple of this value, the 
        // streamer adapters will adjust the chunk size as necessary if there are insufficent data to fill a whole
        // data structure.
        typename SigProcTraits::DataType data(number_of_channels, 
                    pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(1000));

        typename SigProcTraits::Adapter sigproc_adapter;

        unsigned file_index = 0;
        while(true) {
            while(!input_file.eof()) {
                input_file >> sigproc_adapter >> data;
                // we output the data as contiguos spectra. To output as a series of channles
                // use the SigProcFormat<pss::astrotypes::units::Frequency, pss::astrotypes::units::Time>() adapter
                if(as_time_series) {
                    output_file << pss::astrotypes::sigproc::SigProcFormat<pss::astrotypes::units::Frequency, pss::astrotypes::units::Time>() << data;
                }
                else
                {
                    output_file << pss::astrotypes::sigproc::SigProcFormat<pss::astrotypes::units::Time, pss::astrotypes::units::Frequency>() << data;
                }
            }

            // read in the next file and check the haader is consistent
            // with what we expect
            if(++file_index < files.size()) {
                input_file.open(files[file_index]);
                pss::astrotypes::sigproc::Header header;
                input_file >> header;
                if(header.number_of_bits() != sizeof(typename decltype(data)::value_type)) {
                    std::cerr << "Error: file " << files[file_index] 
                              << " has " << header.number_of_bits() << " bit data."
                              << " (expecting " << sizeof(typename decltype(data)::value_type) << " bits)";
                    return 1;
                }
                continue;
            }
            return 0;
        }
    }
}; // end CatData struct

int main(int argc, char** argv) {

    bool as_time_series = false;
    bool as_filterbank = false;
    std::vector<std::string> files;

    // process command line
    for(int a=1; a < argc; ++a) {
        if((char)argv[a][0] == '-') {
            if(std::string("--as_time_series") == argv[a])
            {
                 as_time_series = true;
            }
            if(std::string("--as_filterbank") == argv[a])
            {
                 as_filterbank = true;
            }
            else if(std::string("--help") == argv[a])
            {
                usage(argv[0]);
                return 0;
            }
            else {
                std::cerr << "unknown parameter " << argv[a] << std::endl;
                usage(argv[0]);
                return 1;
            }
        }
        files.push_back(argv[a]);
    }
    if(files.size() <  2) {
        std::cerr << "Must specify one or more input files and an output file" << std::endl;
        usage(argv[0]);
        return 1;
    }
    if( as_time_series && as_filterbank ) {
        std::cerr << "--as_filterbank and --as_time_series are mutually incompatible" << std::endl;
    }
    std::string out_file = files.back();
    files.pop_back();

    // read in a sigproc file
    std::ifstream input_file_stream(argv[1]);
    
    pss::astrotypes::sigproc::Header header;
    input_file_stream >> header;
    
    // dump header information to the screen in a human readable format
    std::cout << pss::astrotypes::sigproc::Header::Info() << header;

    // open the output stream and write out a suitable header
    header.raw_data_file(files[0]); // we can set any/all the different parameters of the header

    if(as_time_series) {
        header.data_type(pss::astrotypes::sigproc::Header::DataType::TimeSeries);
    }
    if(as_filterbank) {
        header.data_type(pss::astrotypes::sigproc::Header::DataType::FilterBank);
    }
    std::ofstream output_file(out_file);
    output_file << header;

    // not launch the appropriate CatData::exec function that matches the header information
    pss::astrotypes::sigproc::DataFactory<CatData>::exec(header
                                                        , header.number_of_channels()
                                                        , output_file
                                                        , files
                                                        , input_file_stream
                                                        , as_time_series);

}
