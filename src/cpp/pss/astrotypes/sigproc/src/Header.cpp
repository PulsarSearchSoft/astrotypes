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
#include "pss/astrotypes/sigproc/Header.h"
#include <iomanip>

namespace pss {
namespace astrotypes {
namespace sigproc {

Header::Header()
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

void Header::reset()
{
    for(auto const& header : _headers) {
        header.second->reset();
    }
}

void Header::read(std::istream & stream)
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
                throw parse_error("Header - unknown parameter", str);
            }
        }

        // read in the value
        it->second->read(stream);
    }
}

void Header::write(std::ostream& stream) const {

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

std::ostream& operator<<(std::ostream& stream, Header const& h)
{
    h.write(stream);
    return stream;
}

std::istream& operator>>(std::istream& stream, Header& headers)
{
    headers.read(stream);
    return stream;
}


bool Header::operator==(Header const& h) const
{
    for(auto const& header : _headers) {
        auto const& field = *h._headers.at(header.first);
        if(header.second->is_set()) {
            if(!field.is_set()) {
                // one is set, the other not so can't be equal
                return false;
            }
            try {
                if(!(field == *header.second)) {
                    // we test the actual values as they are both set
                    return false;
                }
            }
            catch(std::exception const& e) {
                throw parse_error(std::string("exception comparing values for header ") + header.first.string(), e.what());
            }
            catch(...) {
                throw parse_error("exception comparing values for header ", header.first);
            }
        }
        else if(field.is_set()) {
            // one is set, the other not so can't be equal
            return false;
        }
    }
    return true;
}

bool Header::operator!=(Header const& h) const
{
    return !(h==*this);
}

std::size_t Header::size() const {
    return _size;
}

void Header::add(std::string const& name, HeaderFieldBase& field)
{
    _headers.insert(std::make_pair(SigProcLabel(name), &field));
}

void Header::add_read(std::string const& name, HeaderFieldBase& field)
{
    _read_only_headers.insert(std::make_pair(SigProcLabel(name), &field));
}

// --------- getters and setters --------------------------
boost::optional<unsigned> Header::telescope_id() const
{
    return _telescope_id;
}

void Header::telescope_id(unsigned id)
{
    _telescope_id = id;
}

boost::optional<unsigned> Header::machine_id() const
{
    return _machine_id;
}

void Header::machine_id(unsigned id)
{
    _machine_id = id;
}

boost::optional<std::string> const& Header::raw_data_file() const 
{
    return _raw_data_file;
}

void Header::raw_data_file(std::string const& filename)
{
    _raw_data_file = filename;
}

boost::optional<std::string> const& Header::source_name() const
{
    return _source_name;
}

void Header::source_name(std::string const& name)
{
    _source_name = name;
}

boost::optional<unsigned> const& Header::barycentric() const
{
    return _barycentric;
}

void Header::barycentric(bool b)
{
    _barycentric = b;
}

boost::optional<unsigned> const& Header::pulsarcentric() const
{
    return _pulsarcentric;
}

void Header::pulsarcentric(bool b)
{
    _pulsarcentric = b;
}

boost::optional<boost::units::quantity<units::Degree, double>> Header::az_start() const
{
    return _az_start;
}

void Header::az_start(boost::units::quantity<units::Degree, double> const& v)
{
    _az_start = v;
}

boost::optional<boost::units::quantity<units::Degree, double>> Header::za_start() const
{
    return _za_start;
}

void Header::za_start(boost::units::quantity<units::Degree, double> const& v)
{
    _za_start = v;
}

boost::optional<units::ModifiedJulianDate> const& Header::tstart() const
{
    return _tstart;
}

void Header::tstart(units::ModifiedJulianDate const& mjd)
{
    _tstart = mjd;
}

boost::units::quantity<units::Seconds, double> Header::sample_interval() const
{
    if(_tsamp.is_set()) return _tsamp;
    return boost::units::quantity<units::Seconds, double>(0.0 * units::seconds);
}

void Header::sample_interval(boost::units::quantity<units::Seconds, double> tsamp)
{
    _tsamp = tsamp;
}

boost::optional<boost::units::quantity<units::MegaHertz, double>> const& Header::fch1() const
{
    return _fch1;
}

void Header::fch1(boost::units::quantity<units::MegaHertz, double> const& f)
{
    _fch1 = f;
}

boost::optional<boost::units::quantity<units::MegaHertz, double>> const& Header::foff() const
{
    return _foff;
}

void Header::foff(boost::units::quantity<units::MegaHertz, double> const& f)
{
    _foff = f;
}

std::vector<boost::units::quantity<units::MegaHertz, double>> const& Header::frequency_channels() const
{
    return *_freq_channels;
}

void Header::frequency_channels(std::vector<boost::units::quantity<units::MegaHertz, double>> const& frequency_channels)
{
    *_freq_channels = frequency_channels;
}

unsigned Header::number_of_bits() const
{
    if(_n_bits.is_set()) return _n_bits;
    return 0;
}

void Header::number_of_bits(unsigned n)
{
    _n_bits = n;
}

std::size_t Header::number_of_channels() const
{
    if(_n_chans.is_set()) return _n_chans;
    return 0;
}

void Header::number_of_channels(std::size_t n)
{
    _n_chans = n;
}

unsigned Header::number_of_ifs() const
{
    if(_nifs.is_set()) return _nifs;
    return 0;
}

void Header::number_of_ifs(unsigned n)
{
    _nifs = n;
}

boost::optional<units::DispersionMeasure<double>> const& Header::ref_dm() const
{
    return _refdm;
}

void Header::ref_dm(units::DispersionMeasure<double> dm)
{
    _refdm = dm;
}

boost::optional<boost::units::quantity<units::Seconds, double>> const& Header::period() const
{
    return _period;
}

void Header::period(boost::units::quantity<units::Seconds, double> p)
{
    _period = p;
}

Header::Info::Info()
    : _os(&std::cout)
{
}

Header::Info const& Header::Info::operator<<(std::ostream& os) const
{
    _os = &os;
    return *this;
}

std::ostream& Header::Info::operator<<(Header& h) const
{
    for(auto const& header : h._headers) {
        if(header.second->is_set()) {
            *_os << std::left << std::setw(15) << header.second->header_info(header.first.string()) << " : ";
            header.second->write_info(*_os);
            *_os << "\n";
        }
    }
    return *_os;
}

Header::Info const& operator<<(std::ostream& stream, Header::Info const& adapter)
{
    adapter << stream;
    return adapter;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss