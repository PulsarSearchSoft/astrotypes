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
#include "pss/astrotypes/sigproc/SigProcHeader.h"
#include "pss/astrotypes/sigproc/SigProcVariable.h"

namespace pss {
namespace astrotypes {
namespace sigproc {

SigProcHeader::SigProcHeader()
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
    , _tstart("tstart", *this)
    , _tsamp("tsamp", *this)
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

void SigProcHeader::reset()
{
    for(auto const& header : _headers) {
        header.second->reset();
    }
}

void SigProcHeader::read(std::istream & stream)
{
    reset();

    // try to read in the first line of the header
    SigProcLabel str;
    //stream >> str;
    str.read(stream);
    _size = str.size();

    //if (std::strcmp(str.c_str(),"HEADER_START") !=0) {
    if (str != "HEADER_START"){
        throw parse_error("expecting HEADER_START got ", str);
    }
    /* loop over and read remaining header lines until HEADER_END reached */
    while (1) {
        stream >> str;
        _size += str.size();

        if( str.string().find(std::string("HEADER_END") ) != std::string::npos) {
            break;
        }

        // look for a matching known header
        auto it = _read_only_headers.find(str);
        if( it == _read_only_headers.end()) {
            // look for a matching known header
            it = _headers.find(str);
            if( it == _headers.end()) {
                throw parse_error("SigProcHeader - unknown parameter", str);
            }
        }

        // read in the value
        it->second->read(stream);
    }
}

void SigProcHeader::write(std::ostream& stream) const {

    // Write header
    const static SigProcLabel start("HEADER_START");
    stream << start;
    _size = start.size(); 

    // write out header data
    for(auto const& header : _headers) {
        if(header.second->is_set())
        {
            stream << header.first;
            _size += header.first.size();
            _size += header.second->write(stream);
        }
    }

    const static SigProcLabel end("HEADER_END");
    stream << end;
    _size += end.size(); 
}

std::ostream& operator<<(std::ostream& stream, SigProcHeader const& h)
{
    h.write(stream);
    return stream;
}

std::istream& operator>>(std::istream& stream, SigProcHeader& headers)
{
    headers.read(stream);
    return stream;
}


bool SigProcHeader::operator==(SigProcHeader const& h) const
{
    for(auto const& header : _headers) {
        auto const& field = *h._headers.at(header.first);
        if(header.second->is_set()) {
            if(!field.is_set()) {
                // one is set, the other not so can't be equal
                return false;
            }
/* TODO
            if(field != &header.second) {
                // we test the actual values as they are both set
                return false;
            }
*/
        }
        if(field.is_set()) {
            // one is set, the other not so can't be equal
            return false;
        }
    }
    return true;
}

bool SigProcHeader::operator!=(SigProcHeader const& h) const
{
    return !(h==*this);
}

std::size_t SigProcHeader::size() const {
    return _size;
}

void SigProcHeader::add(std::string const& name, HeaderFieldBase& field)
{
    _headers.insert(std::make_pair(SigProcLabel(name), &field));
}

void SigProcHeader::add_read(std::string const& name, HeaderFieldBase& field)
{
    _read_only_headers.insert(std::make_pair(SigProcLabel(name), &field));
}

// --------- getters and setters --------------------------
boost::optional<unsigned> SigProcHeader::telescope_id() const
{
    return _telescope_id;
}

void SigProcHeader::telescope_id(unsigned id)
{
    _telescope_id = id;
}

boost::optional<unsigned> SigProcHeader::machine_id() const
{
    return _machine_id;
}

void SigProcHeader::machine_id(unsigned id)
{
    _machine_id = id;
}

boost::optional<std::string> const& SigProcHeader::raw_data_file() const 
{
    return _raw_data_file;
}

void SigProcHeader::raw_data_file(std::string const& filename)
{
    _raw_data_file = filename;
}

boost::optional<std::string> const& SigProcHeader::source_name() const
{
    return _source_name;
}

void SigProcHeader::source_name(std::string const& name)
{
    _source_name = name;
}

boost::optional<unsigned> const& SigProcHeader::barycentric() const
{
    return _barycentric;
}

void SigProcHeader::barycentric(bool b)
{
    _barycentric = b;
}

boost::optional<unsigned> const& SigProcHeader::pulsarcentric() const
{
    return _pulsarcentric;
}

void SigProcHeader::pulsarcentric(bool b)
{
    _pulsarcentric = b;
}

boost::optional<boost::units::quantity<units::Degree, double>> SigProcHeader::az_start() const
{
    return _az_start;
}

void SigProcHeader::az_start(boost::units::quantity<units::Degree, double> const& v)
{
    _az_start = v;
}

boost::optional<boost::units::quantity<units::Degree, double>> SigProcHeader::za_start() const
{
    return _za_start;
}

void SigProcHeader::za_start(boost::units::quantity<units::Degree, double> const& v)
{
    _za_start = v;
}

boost::optional<units::ModifiedJulianDate> const& SigProcHeader::tstart() const
{
    return _tstart;
}

void SigProcHeader::tstart(units::ModifiedJulianDate const& mjd)
{
    _tstart = mjd;
}

boost::units::quantity<units::Seconds, double> SigProcHeader::sample_interval() const
{
    if(_tsamp.is_set()) return _tsamp;
    return boost::units::quantity<units::Seconds, double>(0.0 * units::seconds);
}

void SigProcHeader::sample_interval(boost::units::quantity<units::Seconds, double> tsamp)
{
    _tsamp = tsamp;
}

boost::optional<boost::units::quantity<units::MegaHertz, double>> const& SigProcHeader::fch1() const
{
    return _fch1;
}

void SigProcHeader::fch1(boost::units::quantity<units::MegaHertz, double> const& f)
{
    _fch1 = f;
}

boost::optional<boost::units::quantity<units::MegaHertz, double>> const& SigProcHeader::foff() const
{
    return _foff;
}

void SigProcHeader::foff(boost::units::quantity<units::MegaHertz, double> const& f)
{
    _foff = f;
}

std::vector<boost::units::quantity<units::MegaHertz, double>> const& SigProcHeader::frequency_channels() const
{
    return *_freq_channels;
}

void SigProcHeader::frequency_channels(std::vector<boost::units::quantity<units::MegaHertz, double>> const& frequency_channels)
{
    *_freq_channels = frequency_channels;
}

unsigned SigProcHeader::number_of_bits() const
{
    if(_n_bits.is_set()) return _n_bits;
    return 0;
}

void SigProcHeader::number_of_bits(unsigned n)
{
    _n_bits = n;
}

std::size_t SigProcHeader::number_of_channels() const
{
    if(_n_chans.is_set()) return _n_chans;
    return 0;
}

void SigProcHeader::number_of_channels(std::size_t n)
{
    _n_chans = n;
}

unsigned SigProcHeader::number_of_ifs() const
{
    if(_nifs.is_set()) return _nifs;
    return 0;
}

void SigProcHeader::number_of_ifs(unsigned n)
{
    _nifs = n;
}

boost::optional<units::DispersionMeasure<double>> const& SigProcHeader::ref_dm() const
{
    return _refdm;
}

void SigProcHeader::ref_dm(units::DispersionMeasure<double> dm)
{
    _refdm = dm;
}

boost::optional<boost::units::quantity<units::Seconds, double>> const& SigProcHeader::period() const
{
    return _period;
}

void SigProcHeader::period(boost::units::quantity<units::Seconds, double> p)
{
    _period = p;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
