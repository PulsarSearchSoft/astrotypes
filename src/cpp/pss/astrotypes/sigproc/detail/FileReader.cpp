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

namespace pss {
namespace astrotypes {
namespace sigproc {

template<typename HeaderType>
FileReader<HeaderType>::FileReader()
{
}

template<typename HeaderType>
FileReader<HeaderType>::FileReader(std::string const& file_name)
{
    open(file_name);
}

template<typename HeaderType>
FileReader<HeaderType>::~FileReader()
{
}

template<typename HeaderType>
void FileReader<HeaderType>::open(std::string const& file_name)
{
    _file_name = file_name;
    _stream.open(_file_name, std::ios::in | std::ios::binary);
    if(!_stream) throw std::runtime_error(file_name + " failed to open");
    this->new_header(_stream);
}

template<typename HeaderType>
std::size_t FileReader<HeaderType>::number_of_data_points() const
{
    struct stat file_info;
    stat(_file_name.c_str(), &file_info);
    return 8*((file_info.st_size - this->_header.size())/this->_header.number_of_bits());
}

template<typename HeaderType>
template<typename DataType>
typename std::enable_if<has_dimensions<DataType, units::Time, units::Frequency>::value, FileReader<HeaderType>>::type &
FileReader<HeaderType>::operator>>(DataType& data)
{
    BaseT::read(_stream, data);
    return *this;
}

namespace {
// helpers for the dimesion method
template<typename Dimension>
struct DimensionHelper {
    template<typename HeaderType>
    inline static DimensionSize<Dimension> exec(FileReader<HeaderType> const&)
    {
        return DimensionSize<Dimension>(0);
    }
};

template<>
struct DimensionHelper<units::Time> {
    template<typename HeaderType>
    inline static DimensionSize<units::Time> exec(FileReader<HeaderType> const& fr)
    {
        return DimensionSize<units::Time>(fr.number_of_data_points()/ (fr.header().number_of_ifs() * fr.header().number_of_channels()));
    }
};

template<>
struct DimensionHelper<units::Frequency> {
    template<typename HeaderType>
    inline static DimensionSize<units::Frequency> exec(FileReader<HeaderType> const& fr)
    {
        return fr.header().number_of_channels();
    }
};

} // namespace
template<typename HeaderType>
template<typename Dimension>
DimensionSize<Dimension> FileReader<HeaderType>::dimension() const
{
    return DimensionHelper<Dimension>::exec(*this);
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
