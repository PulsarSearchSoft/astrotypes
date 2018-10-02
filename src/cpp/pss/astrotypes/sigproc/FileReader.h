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
        FileReader& operator>>(DataType& data);

        /**
         * @brief setup a resize object with the number of channels
         */
        typename ResizeAdapter<units::Frequency>::Stream<FileReader>& operator>>(ResizeAdapter<units::Frequency>& resizer);

        /**
         * @brief setup a resize object with the number of time samples
         */
        typename ResizeAdapter<units::Time>::Stream<FileReader>& operator>>(ResizeAdapter<units::Time>& resizer);

        /**
         * @brief setup a resize object with the number of time samples and number of channels
         */
        template<typename T1, typename T2>
        typename ResizeAdapter<T1, T2>::template Stream<FileReader>& operator>>(ResizeAdapter<T1, T2>& resizer) const;

    protected:
        DimensionSize<units::Time> number_of_samples() const;

    private:
        std::ifstream _stream;
        std::string _file_name;
};

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/FileReader.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_FILEREADER_H
