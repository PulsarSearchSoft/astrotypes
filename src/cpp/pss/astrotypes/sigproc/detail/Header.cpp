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
#include "HeaderField.cpp"
#include <sstream>

namespace pss {
namespace astrotypes {
namespace sigproc {

inline Header::Header()
    : _telescope_id("telescope_id", *this)
    , _machine_id("machine_id", *this)
    , _data_type("data_type", *this)
    , _raw_data_file("raw_data_file", *this)
    , _source_name("source_name", *this)
    , _barycentric("barycentric", *this)
    , _pulsarcentric("pulsarcentric", *this)
    , _az_start("az_start", *this)
    , _za_start("za_start", *this)
    , _src_raj("src_raj", *this)
    , _src_dej("src_dej", *this)
    , _tsamp("tsamp", *this)
    , _tstart("tstart", *this, _tsamp)
    , _n_bits("nbits", *this)
    , _nsamples("nsamples", *this)
    , _fch1("fch1", *this)
    , _foff("foff", *this)
    , _freq_channels("FREQUENCY_START", "fchannel", "FREQUENCY_END", *this)
    , _n_chans("nchans", *this)
    , _nifs("nifs", *this)
    , _refdm("refdm", *this)
    , _period("period", *this)
{
}

inline Header::Header(Header const& h)
    : BaseT(h)
    , _telescope_id("telescope_id", *this)
    , _machine_id("machine_id", *this)
    , _data_type("data_type", *this)
    , _raw_data_file("raw_data_file", *this)
    , _source_name("source_name", *this)
    , _barycentric("barycentric", *this)
    , _pulsarcentric("pulsarcentric", *this)
    , _az_start("az_start", *this)
    , _za_start("za_start", *this)
    , _src_raj("src_raj", *this)
    , _src_dej("src_dej", *this)
    , _tsamp("tsamp", *this)
    , _tstart("tstart", *this, _tsamp)
    , _n_bits("nbits", *this)
    , _nsamples("nsamples", *this)
    , _fch1("fch1", *this)
    , _foff("foff", *this)
    , _freq_channels("FREQUENCY_START", "fchannel", "FREQUENCY_END", *this)
    , _n_chans("nchans", *this)
    , _nifs("nifs", *this)
    , _refdm("refdm", *this)
    , _period("period", *this)
{
    this->copy_header_values(h);
}

inline Header& Header::operator=(Header const& h)
{
    return this->copy_header_values(h);
}

template<typename T, typename Alloc>
Header::OStreamAdapter<astrotypes::TimeFrequency<T, Alloc>> Header::operator<<(astrotypes::TimeFrequency<T, Alloc> const& data)
{
    _n_bits = sizeof(T) * 8;
    _n_chans = data.template size<units::Frequency>();
    return OStreamAdapter<astrotypes::TimeFrequency<T, Alloc>>(*this, data);
}

template<typename T>
Header::OStreamAdapter<T>::OStreamAdapter(Header const& h, T const& data)
    : _h(h)
    , _d(data)
{
}

template<typename T>
std::ostream& Header::OStreamAdapter<T>::operator<<(std::ostream& os) const
{
    os << _h;
    os << _d;
    return os;
}

// --------- getters and setters --------------------------
inline boost::optional<unsigned> Header::telescope_id() const
{
    return _telescope_id;
}

inline void Header::telescope_id(unsigned id)
{
    _telescope_id = id;
}

inline boost::optional<unsigned> Header::machine_id() const
{
    return _machine_id;
}

inline void Header::machine_id(unsigned id)
{
    _machine_id = id;
}

inline boost::optional<std::string> const& Header::raw_data_file() const
{
    return _raw_data_file;
}

inline void Header::data_type(Header::DataType type)
{
    _data_type = static_cast<unsigned>(type);
}

inline Header::DataType Header::data_type() const
{
    if(_data_type.is_set()) return static_cast<DataType>((int)_data_type);
    return DataType::Undefined;
}

inline void Header::raw_data_file(std::string const& filename)
{
    _raw_data_file = filename;
}

inline boost::optional<std::string> const& Header::source_name() const
{
    return _source_name;
}

inline void Header::source_name(std::string const& name)
{
    _source_name = name;
}

inline boost::optional<unsigned> const& Header::barycentric() const
{
    return _barycentric;
}

inline void Header::barycentric(bool b)
{
    _barycentric = b;
}

inline boost::optional<unsigned> const& Header::pulsarcentric() const
{
    return _pulsarcentric;
}

inline void Header::pulsarcentric(bool b)
{
    _pulsarcentric = b;
}

inline boost::optional<boost::units::quantity<units::Degree, double>> Header::az_start() const
{
    return _az_start;
}

inline void Header::az_start(boost::units::quantity<units::Degree, double> const& v)
{
    _az_start = v;
}

inline boost::optional<boost::units::quantity<units::Degree, double>> Header::za_start() const
{
    return _za_start;
}

inline void Header::za_start(boost::units::quantity<units::Degree, double> const& v)
{
    _za_start = v;
}

inline boost::optional<units::ModifiedJulianDate> const& Header::tstart() const
{
    return _tstart;
}

inline void Header::tstart(units::ModifiedJulianDate const& mjd)
{
    _tstart = mjd;
}

inline boost::units::quantity<units::Seconds, double> Header::sample_interval() const
{
    if(_tsamp.is_set()) return _tsamp;
    return boost::units::quantity<units::Seconds, double>(0.0 * units::seconds);
}

inline void Header::sample_interval(boost::units::quantity<units::Seconds, double> tsamp)
{
    _tsamp = tsamp;
}

inline boost::optional<boost::units::quantity<units::MegaHertz, double>> const& Header::fch1() const
{
    return _fch1;
}

inline void Header::fch1(boost::units::quantity<units::MegaHertz, double> const& f)
{
    _fch1 = f;
}

inline boost::optional<boost::units::quantity<units::MegaHertz, double>> const& Header::foff() const
{
    return _foff;
}

inline void Header::foff(boost::units::quantity<units::MegaHertz, double> const& f)
{
    _foff = f;
}

inline std::vector<boost::units::quantity<units::MegaHertz, double>> const& Header::frequency_channels() const
{
    return *_freq_channels;
}

inline void Header::frequency_channels(std::vector<boost::units::quantity<units::MegaHertz, double>> const& frequency_channels)
{
    *_freq_channels = frequency_channels;
}

inline boost::optional<units::DispersionMeasure<double>> const& Header::ref_dm() const
{
    return _refdm;
}

inline void Header::ref_dm(units::DispersionMeasure<double> dm)
{
    _refdm = dm;
}

inline boost::optional<boost::units::quantity<units::Seconds, double>> const& Header::period() const
{
    return _period;
}

inline void Header::period(boost::units::quantity<units::Seconds, double> p)
{
    _period = p;
}

inline DimensionSize<units::Frequency> Header::number_of_channels() const
{
    if(_n_chans.is_set()) return DimensionSize<units::Frequency>(_n_chans);
    return DimensionSize<units::Frequency>(0);
}

inline void Header::number_of_channels(std::size_t n)
{
    _n_chans = n;
}

inline unsigned Header::number_of_bits() const
{
    if(_n_bits.is_set()) return _n_bits;
    return 0;
}

inline void Header::number_of_bits(unsigned n)
{
    _n_bits = n;
}

inline boost::optional<unsigned> const& Header::number_of_samples() const
{
    return _nsamples;
}

inline void Header::number_of_samples(unsigned n)
{
    _nsamples = n;
}

inline void Header::number_of_ifs(unsigned i)
{
    _nifs = i;
}

inline unsigned Header::number_of_ifs() const
{
    if(_nifs) return _nifs;
    return 0;
}

inline Header::Info const& operator<<(std::ostream& os, Header::Info const& adapter)
{
    adapter << os;
    return adapter;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
