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

static const SigProcLabel telescope_id_label("telescope_id");
static const SigProcLabel machine_id_label("machine_id");
static const SigProcLabel data_type_label("data_type");
static const SigProcLabel raw_data_file_label("raw_data_file");
static const SigProcLabel source_name_label("source_name");
static const SigProcLabel barycentric_label("barycentric");
static const SigProcLabel pulsarcentric_label("pulsarcentric");
static const SigProcLabel az_start_label("az_start");
static const SigProcLabel za_start_label("za_start");
static const SigProcLabel src_raj_label("src_raj");
static const SigProcLabel src_dej_label("src_dej");
static const SigProcLabel tsamp_label("tsamp");
static const SigProcLabel tstart_label("tstart");
static const SigProcLabel nbits_label("nbits");
static const SigProcLabel nsamples_label("nsamples");
static const SigProcLabel fch1_label("fch1");
static const SigProcLabel foff_label("foff");
static const SigProcLabel fchannel_label("fchannel");
static const SigProcLabel fchannel_start_label("FREQUENCY_START");
static const SigProcLabel fchannel_end_label("FREQUENCY_END");
static const SigProcLabel nchans_label("nchans");
static const SigProcLabel nifs_label("nifs");
static const SigProcLabel refdm_label("refdm");
static const SigProcLabel period_label("period");

inline Header::Header()
    : _telescope_id(telescope_id_label, *this)
    , _machine_id(machine_id_label, *this)
    , _data_type(data_type_label, *this)
    , _raw_data_file(raw_data_file_label, *this)
    , _source_name(source_name_label, *this)
    , _barycentric(barycentric_label, *this)
    , _pulsarcentric(pulsarcentric_label, *this)
    , _az_start(az_start_label, *this)
    , _za_start(za_start_label, *this)
    , _src_raj(src_raj_label, *this)
    , _src_dej(src_dej_label, *this)
    , _tsamp(tsamp_label, *this)
    , _tstart(tstart_label, *this, _tsamp)
    , _n_bits(nbits_label, *this)
    , _nsamples(nsamples_label, *this)
    , _fch1(fch1_label, *this)
    , _foff(foff_label, *this)
    , _freq_channels(fchannel_start_label, fchannel_label, fchannel_end_label, *this)
    , _n_chans(nchans_label, *this)
    , _nifs(nifs_label, *this)
    , _refdm(refdm_label, *this)
    , _period(period_label, *this)
{
}

inline Header::Header(Header const& h)
    : BaseT(h)
    , _telescope_id(telescope_id_label, *this, h._telescope_id)
    , _machine_id(machine_id_label, *this, h._machine_id)
    , _data_type(data_type_label, *this, h._data_type)
    , _raw_data_file(raw_data_file_label, *this, h._raw_data_file)
    , _source_name(source_name_label, *this, h._source_name)
    , _barycentric(barycentric_label, *this, h._barycentric)
    , _pulsarcentric(pulsarcentric_label, *this, h._pulsarcentric)
    , _az_start(az_start_label, *this, h._az_start)
    , _za_start(za_start_label, *this, h._za_start)
    , _src_raj(src_raj_label, *this, h._src_raj)
    , _src_dej(src_dej_label, *this, h._src_dej)
    , _tsamp(tsamp_label, *this, h._tsamp)
    , _tstart(tstart_label, *this, _tsamp, h._tstart)
    , _n_bits(nbits_label, *this, h._n_bits)
    , _nsamples(nsamples_label, *this, h._nsamples)
    , _fch1(fch1_label, *this, h._fch1)
    , _foff(foff_label, *this, h._foff)
    , _freq_channels(fchannel_start_label, fchannel_label, fchannel_end_label, *this, h._freq_channels)
    , _n_chans(nchans_label, *this, h._n_chans)
    , _nifs(nifs_label, *this, h._nifs)
    , _refdm(refdm_label, *this, h._refdm)
    , _period(period_label, *this, h._period)
{
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

template<typename Stream>
inline Header::InfoSentry<Stream> operator<<(Stream& os, Header::Info const& adapter)
{
    return adapter.sentry(os);
}


} // namespace sigproc
} // namespace astrotypes
} // namespace pss
