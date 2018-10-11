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
#include <iomanip>
#include <sstream>

namespace pss {
namespace astrotypes {
namespace sigproc {

template<typename Derived>
template<typename T>
std::runtime_error HeaderBase<Derived>::parse_error(std::string const& msg, T const& msg2) const
{
    std::stringstream ss(msg);
    try {
        ss << ": " << msg2;
    }
    catch(...) {
        // don't let this method throw
    }

    return std::runtime_error(ss.str());
}

template<typename Derived>
HeaderBase<Derived>::HeaderBase()
{
}

template<typename Derived>
HeaderBase<Derived>::HeaderBase(HeaderBase const& h)
    : _size(h._size)
{
}

template<typename Derived>
void HeaderBase<Derived>::reset()
{
    static_cast<Derived&>(*this).do_reset();
}

template<typename Derived>
void HeaderBase<Derived>::do_reset()
{
    for(auto const& header : _headers) {
        header.second->reset();
    }
}

template<typename Derived>
void HeaderBase<Derived>::remove_compare_field(SigProcLabel const& label)
{
    auto it = _compare_headers.find(label);
    if(it == _compare_headers.end()) return;
    _compare_headers.erase(it);
}

template<typename Derived>
void HeaderBase<Derived>::add_compare_field(SigProcLabel const& label)
{
    auto it = _compare_headers.find(label);
    if(it != _compare_headers.end()) return;
    _compare_headers.insert(std::make_pair(label, _headers.at(label)));
}

template<typename Derived>
Derived& HeaderBase<Derived>::copy_header_values(Derived const& dst)
{
    for(auto const& header : dst._headers) {
        if(header.second->is_set()) {
            auto& field = *_headers.at(header.first);
            field = *header.second;
        }
    }
    return static_cast<Derived&>(*this);
}

template<typename Derived>
void HeaderBase<Derived>::read(std::istream & stream)
{
    static_cast<Derived&>(*this).do_read(stream);
}

template<typename Derived>
void HeaderBase<Derived>::do_read(std::istream & stream)
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
        _size += it->second->read(stream);
    }
}

template<typename Derived>
void HeaderBase<Derived>::write(std::ostream& stream) const {
    static_cast<Derived const&>(*this).do_write(stream);
}

template<typename Derived>
void HeaderBase<Derived>::do_write(std::ostream& stream) const {

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

template<typename Derived>
std::ostream& operator<<(std::ostream& stream, HeaderBase<Derived> const& h)
{
    h.write(stream);
    return stream;
}

template<typename Derived>
std::istream& operator>>(std::istream& stream, HeaderBase<Derived>& headers)
{
    headers.read(stream);
    return stream;
}

template<typename Derived>
bool HeaderBase<Derived>::operator==(HeaderBase<Derived> const& h) const
{
    return static_cast<Derived const&>(*this).do_equal(h);
}

template<typename Derived>
bool HeaderBase<Derived>::do_equal(HeaderBase<Derived> const& h) const
{
    for(auto const& header : _compare_headers) {
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

template<typename Derived>
bool HeaderBase<Derived>::operator!=(HeaderBase<Derived> const& h) const
{
    return !(h==*this);
}

template<typename Derived>
std::size_t HeaderBase<Derived>::size() const {
/*
    std::size_t size = 0U;
    for(auto const& header : _headers) {
        if(header.second->is_set()) {
            size += header.first.size();
            size += header.size_bytes();
        }
    }
    return size;
*/
    return _size;
}

template<typename Derived>
void HeaderBase<Derived>::add(SigProcLabel const& name, HeaderFieldBase& field)
{
    _headers.insert(std::make_pair(name, &field));
    if(name != "tstart")
    {
        _compare_headers.insert(std::make_pair(name, &field));
    }
}

template<typename Derived>
void HeaderBase<Derived>::add_read(SigProcLabel const& name, HeaderFieldBase& field)
{
    _read_only_headers.insert(std::make_pair(name, &field));
}

template<typename Derived>
HeaderBase<Derived>::Info::Info()
{
}

template<typename Derived>
template<typename Stream>
typename HeaderBase<Derived>::template InfoSentry<Stream> HeaderBase<Derived>::Info::sentry(Stream& os) const
{
    return HeaderBase<Derived>::InfoSentry<Stream>(os);
}

template<typename Derived>
template<typename Stream>
HeaderBase<Derived>::InfoSentry<Stream>::InfoSentry(Stream& os)
    : _os(os)
{
}

template<typename Derived>
template<typename Stream>
Stream& HeaderBase<Derived>::InfoSentry<Stream>::operator<<(Derived const& h) const
{
    for(auto const& header : h._headers) {
        if(header.second->is_set()) {
            _os << std::left << std::setw(15) << header.second->header_info(header.first.string()) << " : ";
            header.second->write_info(_os);
            _os << "\n";
        }
    }
    return _os;
}

template<typename Derived, typename Stream>
typename HeaderBase<Derived>::template InfoSentry<Stream> const& operator<<(Stream& stream, typename HeaderBase<Derived>::Info const& adapter)
{
    return adapter.sentry(stream);
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
