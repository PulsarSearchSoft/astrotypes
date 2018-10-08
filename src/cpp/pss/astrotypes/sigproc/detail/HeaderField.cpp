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
#include "pss/astrotypes/sigproc/HeaderField.h"
#include <boost/units/quantity.hpp>
#include <boost/units/cmath.hpp>
#include <iostream>
#include <exception>
#include <cmath>

namespace pss {
namespace astrotypes {
namespace sigproc {

template<typename T>
HeaderField<T>::HeaderField(std::string const& header_label, Header& h)
    : BaseT(header_label, h)
{
}

template<typename T>
HeaderField<T>::HeaderField(std::string const& header_label, Header& h, T const& t)
    : BaseT(header_label, h)
    , _var(t)
{
}

template<typename T>
HeaderField<T>& HeaderField<T>::operator=(T const& var )
{
    _var = var;
    return *this;
}

template<typename T>
void HeaderField<T>::operator=(const HeaderFieldBase& h )
{
    _var = reinterpret_cast<HeaderField<T> const&>(h)._var;
}

template<typename T>
unsigned HeaderField<T>::read(std::istream& stream)
{
    T v;
    auto count = SigProcVariable<T>::read(stream, v);
    _var=v;
    return count;
}

template<typename T>
unsigned HeaderField<T>::write(std::ostream& stream) const
{
    return SigProcVariable<T>::write(stream, *_var);
}

template<typename T>
void HeaderField<T>::write_info(std::ostream& stream) const
{
    stream << *_var;
}

template<typename T>
void HeaderField<T>::reset()
{
    _var.reset();
}

template<typename T>
bool HeaderField<T>::is_set() const
{
    if(_var) return true;
    return false;
}

template<typename T>
bool HeaderField<T>::operator==(const HeaderFieldBase& f) const
{
    return *this == reinterpret_cast<HeaderField const&>(f);
}

template<typename T>
bool HeaderField<T>::operator==(const HeaderField& f) const
{
    return *f._var == *_var;
}
// -------------- std::vector specialization
template<typename T>
HeaderField<std::vector<T>>::HeaderField( std::string const& start_label
                                        , std::string const& item_label
                                        , std::string const& end_label
                                        , Header& header)
    : BaseT(start_label, header)
    , _item_label_handler(_var)
    , _end_label(end_label)
    , _item_label(item_label)
{
    // register the read handlers
    BaseT::add_read(end_label, _end_label_handler, header);
    BaseT::add_read(item_label, _item_label_handler, header);
}

template<typename T>
void HeaderField<std::vector<T>>::reset()
{
    _var.clear();
}

template<typename T>
bool HeaderField<std::vector<T>>::is_set() const
{
    return !_var.empty();
}

template<typename T>
unsigned HeaderField<std::vector<T>>::read(std::istream&)
{
    _var.clear();
    return 0;
}

template<typename T>
unsigned HeaderField<std::vector<T>>::ItemField::read(std::istream& stream)
{
    T v;
    unsigned c = SigProcVariable<T>::read(stream, v);
    _vec.emplace_back(v);
    return c;
}

template<typename T>
unsigned HeaderField<std::vector<T>>::write(std::ostream& stream) const
{
    unsigned size=0;
    for(auto const& var : _var) {
        stream << _item_label;
        size += _item_label.size();
        size += SigProcVariable<T>::write(stream, var);
    }
    // write out the end marker
    stream << _end_label;
    size += _end_label.size();

    return size;
}

template<typename T>
std::string const& HeaderField<std::vector<T>>::header_info(std::string const&) const
{
    return _item_label.string();
}

template<typename T>
void HeaderField<std::vector<T>>::write_info(std::ostream& stream) const
{
    // list all the elements if its a reasonable number
    std::string sep="";
    stream << "(";
    if(_var.size() < 4) {
        for( auto const& var : _var )
        {
            stream << sep << var;
            sep = ", ";
        }
    }
    else {
        stream << _var[0] << ", " << _var[1] << ", ..., " << _var.back();
    }
    stream << ") " << _var.size() << " elements";
}

template<typename T>
bool HeaderField<std::vector<T>>::operator==(HeaderFieldBase const& h) const
{
    return *this == reinterpret_cast<HeaderField<std::vector<T>> const&>(h);
}

template<typename T>
bool HeaderField<std::vector<T>>::operator==(const HeaderField& h) const
{
    if(h._var.size() != _var.size()) return false;
    return true;    // only checks if sizes are the same. is this enough?
}

template<typename T>
void HeaderField<std::vector<T>>::operator=(const HeaderFieldBase& h )
{
    _var = reinterpret_cast<HeaderField<std::vector<T>> const&>(h)._var;
}

// ------ HeaderFieldWithTolerance ------
template<typename T, typename ToleranceType>
HeaderFieldWithTolerance<T, ToleranceType>::HeaderFieldWithTolerance(std::string const& header_label, Header& header, ToleranceType const& t)
    : BaseT(header_label, header)
    , _tolerance(t)
{
}

template<typename T, typename ToleranceType>
HeaderFieldWithTolerance<T, ToleranceType>& HeaderFieldWithTolerance<T, ToleranceType>::operator=(T const& h)
{
    static_cast<BaseT&>(*this) = h;
    return *this;
}

template<typename T, typename ToleranceType>
bool HeaderFieldWithTolerance<T, ToleranceType>::operator==(HeaderFieldBase const& h) const
{
    return *this == reinterpret_cast<HeaderFieldWithTolerance const&>(h);
}

namespace {
template<typename T2>
struct compare_tolerance
{
    template<typename T1>
    static inline
    bool exec(T1 const& t1, T2 const& t2) {
        return t1 < t2;
    }
};

template<typename T2>
struct compare_tolerance<HeaderField<T2>>
{
    template<typename T1>
    static inline
    bool exec(T1 const& t1, HeaderField<T2> const& t2) {
        if(t2.is_set())
            return t1 < static_cast<T2 const&>(t2);
        return true;
    }
};

} // namespace

template<typename T, typename ToleranceType>
bool HeaderFieldWithTolerance<T, ToleranceType>::operator==(HeaderFieldWithTolerance const& h) const
{
    // allow for ADL lookups defaulting to std
    using std::abs;
    return compare_tolerance<typename std::decay<ToleranceType>::type>::exec(abs(*this->_var - *h._var) , _tolerance); 
}

inline HeaderFieldBase::HeaderFieldBase(std::string const& header_name, Header& h)
{
    h.add(header_name, *this);
}

inline void HeaderFieldBase::add_read(std::string const& header_label, HeaderFieldBase& field, Header& header)
{
    header.add_read(header_label, field);
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss



