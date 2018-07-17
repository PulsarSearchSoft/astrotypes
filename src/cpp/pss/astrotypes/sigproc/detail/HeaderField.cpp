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
#include <iostream>
#include <exception>


namespace pss {
namespace astrotypes {
namespace sigproc {

template<typename T>
HeaderField<T>::HeaderField(std::string const& header_label, SigProcHeader& h)
    : BaseT(header_label, h)
{
}

template<typename T>
HeaderField<T>::HeaderField(std::string const& header_label, SigProcHeader& h, T const& t)
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
unsigned HeaderField<T>::read(std::istream& stream)
{
    return SigProcVariable<T>::read(stream, *_var);
}

template<typename T>
unsigned HeaderField<T>::write(std::ostream& stream)
{
    return SigProcVariable<T>::write(stream, *_var);
}

template<typename T>
bool HeaderField<T>::is_set() const
{
    if(_var) return true;
    return false;
}

// -------------- std::vector specialization
template<typename T>
HeaderField<std::vector<T>>::HeaderField( std::string const& start_label
                                        , std::string const& item_label
                                        , std::string const& end_label
                                        , SigProcHeader& header)
    : BaseT(start_label, header)
    , _item_label_handler(_var)
    , _end_label(item_label)
    , _item_label(item_label)
{
    // register the read handlers
    BaseT::add_read(end_label, _end_label_handler, header);
    BaseT::add_read(item_label, _item_label_handler, header);
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
unsigned HeaderField<std::vector<T>>::write(std::ostream& stream)
{
    unsigned size=0;
    for(auto const& var : _var) {
        stream << _item_label;
        size += _item_label.size();
        size += SigProcVariable<T>::write(stream, var);
    }
    // write out the end marker
    stream << SigProcLabel(_end_label);
    size += _end_label.size();

    return size;
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
