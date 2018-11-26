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
#ifndef PSS_ASTROTYPES_SIGPROC_HEADERBASE_H
#define PSS_ASTROTYPES_SIGPROC_HEADERBASE_H

#include "../HeaderField.h"
#include <iostream>
#include <exception>
#include <map>

namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief
 *    Store SigProc header information
 *
 * @details
 *    Sigproc has a number of mandatory and optional parameters
 *    All stored at the beginning of the sigproc file.
 *
 *    As a consequence the type often returned is a utils::Optional<T>.
 *    This class is designed to allow you to test if the value has been set
 *    or not.
 *    e.g.
 * @code
 *      if(!header.source_name()) {
 *          // oh oh no source name,  better deal with it
 *          ...
 *      }
 *      // once tested you can get the underlying variable with the dereference operator *
 *      std::string const& source = *header.source_name();
 * @endcode
 *
 *    note also that many of the types returned as types with specific units. If your writting new code
 *    its a good idea to keep this type information when you pass this value around, but for compatibility
 *    with less type safe codes and you need just the numerical value you can call the value() mehtod
 * @code
 *    // n.b. this loses type information.
 *    double tsamp_in_secs = header.sample_interval().value();
 * @endcode
 *
 * @subsection Extending for Custom types
 */

template<typename Derived>
class HeaderBase
{
    private:
        friend class HeaderFieldBase;

    public:
        template<typename Stream>
        class InfoSentry {
            public:
                InfoSentry(Stream& os);
                Stream& operator<<(Derived const& os) const;

            private:
                Stream& _os;
        };

    public:
        // Adapter for ouputing debug info about the header
        class Info {
            public:
                Info();
                template<typename Stream>
                InfoSentry<Stream> sentry(Stream& os) const;
        };

    public:
        HeaderBase();
        // maps will not be copied over
        HeaderBase(HeaderBase const&);

        /**
         * @brief read in header data from the provided stream
         * @details will throw if the data stream is not in the expected format or eof
         * @throw if attempt to write out of bounds data (e.g strings > 80 chars)
         */
        void read(std::istream & stream);

        /**
         * @brief write header data to the provided stream
         * @details
         * @throw if attempt to read out of bounds data (e.g strings > 80 chars)
         */
        void write(std::ostream & stream) const;

        /**
         * @brief returns true if the data formats match
         * @details source names/machine names are ignored
         *          this only matches data formatting types and offsets
         *          Equality in this case means that the data will be 
         *          consistent with the data type. e.g. tstart is ignored
         *          Other fields may be ignored depnding on the data tyep being
         *          represented
         */
        bool operator==(HeaderBase const&) const;

        /**
         * @brief returns true if the data format meta data does not match
         * @details see caveats in operator==
         */
        bool operator!=(HeaderBase const&) const;

        /**
         * @brief returns the number of bytes in the header
         */
        std::size_t size() const;

        /**
         * @brief reset all header variables to an undefined state
         */
        void reset();

    protected:
        /**
         * @brief copy values from one header to another
         */
        Derived& copy_header_values(Derived const& dst);

        // default implemention overridable in Derived type
        void do_read(std::istream & stream);
        void do_write(std::ostream & stream) const;
        bool do_equal(HeaderBase const&) const;
        void do_reset();

    protected:

        template<typename T>
        std::runtime_error parse_error(std::string const& msg, T const& msg2) const;

        /**
         * @brief add a field to be parsed for read and write
         */
        void add(SigProcLabel const& label, HeaderFieldBase& field);

        /**
         * @brief add a field to be parsed for read only
         */
        void add_read(SigProcLabel const& label, HeaderFieldBase& field);

        /**
         * @brief add a field to be checked in operator==
         * @details n.b the label must already of been added with the add() method
         */
        void add_compare_field(SigProcLabel const&);

        /**
         * @brief remvoe a field to be checked in operator==
         */
        void remove_compare_field(SigProcLabel const&);

    private:
        mutable unsigned _size; // byte size of the header

        std::map<SigProcLabel, HeaderFieldBase*> _headers;
        std::map<SigProcLabel, HeaderFieldBase*> _compare_headers;
        std::map<SigProcLabel, HeaderFieldBase*> _read_only_headers;
};

template<typename Derived>
std::ostream& operator<<(std::ostream& os, HeaderBase<Derived> const&);

template<typename Derived>
std::istream& operator>>(std::istream& os, HeaderBase<Derived>&);

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "HeaderBase.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_HEADERBASE_H
