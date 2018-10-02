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
    this->new_header(_stream);
}

template<typename HeaderType>
DimensionSize<units::Time> FileReader<HeaderType>::number_of_samples() const
{
    return DimensionSize<units::Time>( (stat(_file_name) - this->_header.size())
                                                / (this->_header.number_of_bits() * this->_header.number_of_ifs()));
}

template<typename HeaderType>
template<typename T1, typename T2>
typename ResizeAdapter<T1, T2>::template Stream<FileReader<HeaderType>>& FileReader<HeaderType>::operator>>(ResizeAdapter<T1, T2>& resizer) const
{
    return resizer.resize(*this, this->number_of_samples(), this->_header.number_of_channels());
}

template<typename HeaderType>
typename ResizeAdapter<units::Time>::Stream<FileReader<HeaderType>>& FileReader<HeaderType>::operator>>(ResizeAdapter<units::Time>& resizer)
{
    return resizer.resize(*this, number_of_samples());
}

template<typename HeaderType>
typename ResizeAdapter<units::Frequency>::Stream<FileReader<HeaderType>>& FileReader<HeaderType>::operator>>(ResizeAdapter<units::Frequency>& resizer)
{
    return resizer.resize(this->_header.number_of_channels());
}

template<typename HeaderType>
template<typename DataType>
FileReader<HeaderType>& FileReader<HeaderType>::operator>>(DataType& data)
{
    BaseT::read(_stream, data);
    return *this;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
