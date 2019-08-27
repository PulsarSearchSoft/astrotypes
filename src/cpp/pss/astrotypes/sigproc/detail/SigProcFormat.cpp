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

namespace {
    //should work with any iterator
    template<typename IteratorType, typename IteratorTag, typename ValueType>
    struct do_write {
        static void exec(IteratorType begin, IteratorType const end, std::ostream& os) {
            while(begin != end) {
                os.write(reinterpret_cast<const char*>(&(*begin)), sizeof(ValueType));
                ++begin;
            }
        }
    };

    // we can speed things up if the memory is contiguous
    template<typename IteratorType, typename ValueType>
    struct do_write<IteratorType, std::random_access_iterator_tag, ValueType>
    {

        static void exec(IteratorType begin, IteratorType const end, std::ostream& os) {
            os.write(reinterpret_cast<const char*>(&(*begin)), std::distance(begin, end) * sizeof(ValueType));
        }
    };

    template<typename IteratorType, typename IteratorTag, typename ValueType>
    struct do_read {
        static void exec(IteratorType begin, IteratorType const end, std::istream& is) {
            while(begin != end) {
                is.read(reinterpret_cast<char*>(&(*begin)), sizeof(ValueType));
                ++begin;
            }
        }
    };

    template<typename IteratorType, typename ValueType>
    struct do_read<IteratorType, std::random_access_iterator_tag, ValueType>
    {
        static void exec(IteratorType begin, IteratorType const end, std::istream& is) {
            is.read(reinterpret_cast<char*>(&(*begin)), std::distance(begin, end) * sizeof(ValueType));
        }
    };

    // calls the correct specialization
    template<typename IteratorType>
    void write(IteratorType begin, IteratorType const end, std::ostream& os) {
        do_write<IteratorType
                , typename std::iterator_traits<typename std::remove_reference<IteratorType>::type>::iterator_category
                , typename std::iterator_traits<typename std::remove_reference<IteratorType>::type>::value_type
                >::exec(begin, end, os);
    }

    template<typename IteratorType>
    void read(IteratorType begin, IteratorType const end, std::istream& is) {
        do_read<IteratorType
               , typename std::iterator_traits<typename std::remove_reference<IteratorType>::type>::iterator_category
               , typename std::iterator_traits<typename std::remove_reference<IteratorType>::type>::value_type
               >::exec(begin, end, is);
    }
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, SigProcFormat<units::Time, units::Frequency>::ISigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::ISigProcFormat::operator>>(T& d) const
{
    read(d.begin(), d.end(), _is);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time>::value, SigProcFormat<units::Time, units::Frequency>::ISigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::ISigProcFormat::operator>>(T& d) const
{
    read(d.begin(), d.end(), _is);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, SigProcFormat<units::Time, units::Frequency>::OSigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::OSigProcFormat::operator<<(T const& d) const
{
    write(d.cbegin(), d.cend(), _os);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, SigProcFormat<units::Time, units::Frequency>::ISigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::ISigProcFormat::operator>>(T& d) const
{
    for(DimensionIndex<units::Time> spectrum_num(0);  spectrum_num < d.template size<units::Time>(); ++spectrum_num)
    {
        auto spectrum = d[spectrum_num];
        read(spectrum.begin(), spectrum.end(), _is);
    }
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time>::value, SigProcFormat<units::Time, units::Frequency>::OSigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::OSigProcFormat::operator<<(T const& d) const
{
    write(d.cbegin(), d.cend(), _os);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Frequency>::value, SigProcFormat<units::Time, units::Frequency>::OSigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::OSigProcFormat::operator<<(T const& d) const
{
    write(d.cbegin(), d.cend(), _os);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, SigProcFormat<units::Time, units::Frequency>::OSigProcFormat const&>::type
SigProcFormat<units::Time, units::Frequency>::OSigProcFormat::operator<<(T const& d) const
{
    for(DimensionIndex<units::Time> spectrum_num(0);  spectrum_num < d.template size<units::Time>(); ++spectrum_num)
    {
        auto const spectrum = d[spectrum_num];
        write(spectrum.cbegin(), spectrum.cend(), _os);
    }
    return *this;
}

template<typename T, typename Alloc>
typename SigProcFormat<units::Frequency, units::Time>::OSigProcFormat const& SigProcFormat<units::Frequency, units::Time>::OSigProcFormat::operator<<(astrotypes::TimeFrequency<T, Alloc> const& d)
{
    for(DimensionIndex<units::Frequency> channel_num(0);  channel_num < d.template size<units::Frequency>(); ++channel_num)
    {
        auto const& channel = d.channel(channel_num);
        write(channel.cbegin(), channel.cend(), _os);
    }
    return *this;
}

template<typename T, typename Alloc>
typename SigProcFormat<units::Frequency, units::Time>::OSigProcFormat const& SigProcFormat<units::Frequency, units::Time>::OSigProcFormat::operator<<(astrotypes::FrequencyTime<T, Alloc> const& d)
{
    write(d.cbegin(), d.cend(), _os);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time, units::Frequency>::value, SigProcFormat<units::Frequency, units::Time>::ISigProcFormat const&>::type
SigProcFormat<units::Frequency, units::Time>::ISigProcFormat::operator>>(T& d) const
{
    for(DimensionIndex<units::Frequency> channel_num(0);  channel_num < d.template size<units::Frequency>(); ++channel_num)
    {
        auto channel = d.channel(channel_num);
        read(channel.begin(), channel.end(), _is);
    }
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Frequency, units::Time>::value, SigProcFormat<units::Frequency, units::Time>::ISigProcFormat const&>::type
SigProcFormat<units::Frequency, units::Time>::ISigProcFormat::operator>>(T& d) const
{
    read(d.begin(), d.end(), _is);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Time>::value, SigProcFormat<units::Frequency, units::Time>::OSigProcFormat const&>::type
SigProcFormat<units::Frequency, units::Time>::OSigProcFormat::operator<<(T const& d) const
{
    write(d.cbegin(), d.cend(), _os);
    return *this;
}

template<typename T>
typename std::enable_if<has_exact_dimensions<T, units::Frequency>::value, SigProcFormat<units::Frequency, units::Time>::OSigProcFormat const&>::type
SigProcFormat<units::Frequency, units::Time>::OSigProcFormat::operator<<(T const& d) const
{
    write(d.cbegin(), d.cend(), _os);
    return *this;
}

typename SigProcFormat<units::Frequency, units::Time>::OSigProcFormat SigProcFormat<units::Frequency, units::Time>::operator<<(std::ostream& os) const
{
    return OSigProcFormat(os);
}

typename SigProcFormat<units::Frequency, units::Time>::ISigProcFormat SigProcFormat<units::Frequency, units::Time>::operator>>(std::istream& is) const
{
    return ISigProcFormat(is);
}

typename SigProcFormat<units::Time, units::Frequency>::OSigProcFormat SigProcFormat<units::Time, units::Frequency>::operator<<(std::ostream& os) const
{
    return OSigProcFormat(os);
}

typename SigProcFormat<units::Time, units::Frequency>::ISigProcFormat SigProcFormat<units::Time, units::Frequency>::operator>>(std::istream& is) const
{
    return ISigProcFormat(is);
}

template<typename Dimension1, typename Dimension2>
typename SigProcFormat<Dimension1, Dimension2>::OSigProcFormat operator<<(std::ostream& os, SigProcFormat<Dimension1, Dimension2> const& f)
{
    return f << os;
}

template<typename Dimension1, typename Dimension2>
typename SigProcFormat<Dimension1, Dimension2>::ISigProcFormat operator>>(std::istream& is, SigProcFormat<Dimension1, Dimension2> const& f)
{
    return f >> is;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
