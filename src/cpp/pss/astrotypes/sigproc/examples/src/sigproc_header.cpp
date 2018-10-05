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
#include <fstream>

void usage(const char* program_name)
{
    std::cout << "Usage:\n" 
              << "\t" << program_name << " [options] input_file\n"
              << "Synopsis:\n"
              << "\tDisplays the header information of the provided input_files to stdout,\n"
              << "Options:\n"
              << "\t--help          : this message\n";
}

int main(int argc, char** argv) {

    std::string file;

    // process command line
    for(int a=1; a < argc; ++a) {
        if((char)argv[a][0] == '-') {
            if(std::string("--help") == argv[a])
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
            file = argv[a];        
        }
    }
    if(file.size() == 0) {
        std::cerr << argv[0] << " error: no input file supplied" << std::endl;
        usage(argv[0]);
        return 1;
    }

    // read in a sigproc file
    std::ifstream input_file_stream(file);
    
    pss::astrotypes::sigproc::Header header;
    input_file_stream >> header;
    
    // dump header information to the screen in a human readable format
    std::cout << pss::astrotypes::sigproc::Header::Info() << header;

}
