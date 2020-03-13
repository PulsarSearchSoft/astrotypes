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
#include <iostream>
#include <string>

void usage(const char* program_name)
{
    std::cout << "Usage:\n"
              << "\t" << program_name << " [options] input_filterbank_files...\n"
              << "Synopsis:\n"
              << "\tSearch through the filterbank for zeroed channels\n"
              << "Options:\n"
              << "\t--help,-h        : this message\n";
}

struct ResultsWriter
{
        typedef pss::astrotypes::units::Time Time;
        typedef pss::astrotypes::DimensionSpan<Time> SpanType;

    public:
        ResultsWriter()
            : _null_span(pss::astrotypes::DimensionSize<Time>(0))
        {
        }

        ~ResultsWriter()
        {
            if(_null_span.span() != pss::astrotypes::DimensionSize<Time>(0))
            {
                write();
            }
        }

        void mark(pss::astrotypes::DimensionIndex<Time> const& s_num) {
            if(_null_span.span() == pss::astrotypes::DimensionSize<Time>(0))
            {
                _null_span=SpanType(s_num, pss::astrotypes::DimensionSize<Time>(1));
            }
            else if(_null_span.end() + 1 == s_num)
            {
                _null_span.span(_null_span.span() + pss::astrotypes::DimensionSize<Time>(1));
            }
            else {
                write();
                _null_span = SpanType(pss::astrotypes::DimensionSize<Time>(0));
            }
        }

        void write() const
        {
            std::cout << "Null channel(s): ";
            if(_null_span.span() == pss::astrotypes::DimensionSize<Time>(1))
            {
                std::cout << _null_span.start() << "\n";
            } else
            {
                std::cout << _null_span.start() << "-" << _null_span.end() << "\n";
            }
        }

    private:
        SpanType _null_span;
};

template<typename SigProcTraits>
struct SearchData
{
    template<typename HeaderType>
    static
    void exec(HeaderType const& header, std::istream& input_file)
    {
        typedef pss::astrotypes::units::Time Time;

        // read in data in 1024 spectral chunks
        pss::astrotypes::DimensionSize<Time> number_of_spectra(1024);
        typename SigProcTraits::DataType data(header.number_of_channels(), number_of_spectra);
        typename SigProcTraits::Adapter sigproc_adapter;

        pss::astrotypes::DimensionIndex<Time> s_num(0);
        while(!input_file.eof()) {
            input_file >> sigproc_adapter >> data;

            ResultsWriter results;
            for(pss::astrotypes::DimensionIndex<Time> spectrum_number(0);
                spectrum_number < data.template dimension<Time>();
                ++spectrum_number)
            {
                auto spectrum = data[spectrum_number];
                typename SigProcTraits::DataType::value_type sum = 0.0;
                for(auto const& sample : spectrum) {
                    sum += sample;
                }
                if(sum == 0.0) {
                    results.mark(s_num);
                }
                ++s_num;
            }
        }
    }
};

int main(int argc, char** argv) {

    std::vector<std::string> files;

    // process command line
    for(int a=1; a < argc; ++a) {
        if((char)argv[a][0] == '-') {
            if(std::string("--help") == argv[a] || std::string("-h") == argv[a])
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
        else {
            files.push_back(argv[a]);
        }
    }
    if(files.empty()) {
        std::cerr << "Must specify one or more input files and an output file" << std::endl;
        usage(argv[0]);
        return 1;
    }

    for(auto const& filename : files) {
        // read in a sigproc file
        std::ifstream input_file_stream(filename, std::ios::binary);

        pss::astrotypes::sigproc::Header header;
        input_file_stream >> header;
        if(header.data_type() == pss::astrotypes::sigproc::Header::DataType::Undefined)
        {
            // assumed undefined data types are filterbank files
            header.data_type(pss::astrotypes::sigproc::Header::DataType::FilterBank);
        }

        pss::astrotypes::sigproc::DataFactory<SearchData>::exec(header, header, input_file_stream);
    }
}
