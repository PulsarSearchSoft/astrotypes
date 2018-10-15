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
#ifndef PSS_ASTROTYPES_SIGPROC_FILEREADER_H
#define PSS_ASTROTYPES_SIGPROC_FILEREADER_H
#include "Header.h"
#include "pss/astrotypes/multiarray/ResizeAdapter.h"
#include "IStream.h"
#include <string>
#include <fstream>

namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief Read in a sigproc file
 * @details
 */
template<typename HeaderType=Header>
class FileReader : public IStream<HeaderType>
{
        typedef IStream<HeaderType> BaseT;

    public:
        FileReader();

        /**
         * @brief constructor with the file
         */
        FileReader(std::string const& file_name);
        ~FileReader();

        /**
         * @brief set the filename to read
         */
        void open(std::string const& file_name);

        /**
         * @brief read in to the provided data object
         */
        template<typename DataType>
        typename std::enable_if<has_dimensions<DataType, units::Time, units::Frequency>::value, FileReader>::type &
        operator>>(DataType& data);

        /**
         * @brief return the expected dimension of the data in the file
         */
        template<typename Dimension>
        DimensionSize<Dimension> dimension() const;

        /**
         * @brief the number of data points in the file (i.e number of data points of nbits)
         */
        std::size_t number_of_data_points() const;

    protected:
        void do_open(std::string const& file_name);

    private:
        std::ifstream _stream;
        std::string _file_name;
};

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/FileReader.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_FILEREADER_H
