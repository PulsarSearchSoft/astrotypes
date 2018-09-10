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
#ifndef PSS_ASTROTYPES_SIGPROC_HEADERFIELD_H
#define PSS_ASTROTYPES_SIGPROC_HEADERFIELD_H
#include "pss/astrotypes/sigproc/detail/SigProcVariable.h"
#include <boost/optional.hpp>
#include <string>
#include <vector>


namespace pss {
namespace astrotypes {
namespace sigproc {

class Header;

/**
 * @brief class to provide a virtual lookup table for read/write the varoious types
 *        of SigProcVariables
 * @details
 */

class HeaderFieldBase
{
    protected:
        // allows us to hide HeaderFields from the writer
        HeaderFieldBase() {};

        /// add the provided field as a read_only parameter
        void add_read(std::string const& header_label, HeaderFieldBase& field, Header& header);

    public:
        // registers the Field to be read/written by the Header parser
        HeaderFieldBase(std::string const& header_name, Header& h);
        ~HeaderFieldBase() {}; // virtual not needed, we never expect to pass these around

        /// read into variable
        // @return the number of bytes read
        virtual unsigned read(std::istream &) { return 0; };

        /// serialise a variable to sigproc header format
        // @return the number of bytes written
        virtual unsigned write(std::ostream &) const { return 0; };

        /// an information string about the value in the field (for debugging etc)
        virtual void write_info(std::ostream &) const { };

        /// return the required header string to be used in info output
        virtual std::string const& header_info(std::string const& h) const { return h; };

        /// reset the variable to undefined state
        virtual void reset() = 0;

        /// return true if the variable has been set, false otherwise
        virtual bool is_set() const = 0;

        virtual bool operator==(const HeaderFieldBase&) const { return true; };
};

template<typename T>
class HeaderField : public HeaderFieldBase
{
        typedef HeaderFieldBase BaseT;

    protected:
        // for expansion only, not to be used as a public interface
        HeaderField() {};

    public:
        /// add the provided field as a read/write parameters
        HeaderField(std::string const& header_label, Header& header);
        HeaderField(std::string const& header_label, Header& header, T const& to_copy);

        operator T const&() const { return *_var; }
        operator T&() { return *_var; }
        operator boost::optional<T> const&() const { return _var; }
        HeaderField& operator=(T const& var);

        unsigned read(std::istream &) override;
        unsigned write(std::ostream &) const override;
        void write_info(std::ostream &) const override;
        void reset() override;
        bool is_set() const override;
        bool operator==(const HeaderFieldBase&) const override;
        bool operator==(const HeaderField&) const;

    protected:
        boost::optional<T> _var;
};

/**
 * @brief specialisation to allow the == operator to return true if the values are within specified tolerance
 */
template<typename T, typename ToleranceType>
class HeaderFieldWithTolerance : public HeaderField<T>
{
        typedef HeaderField<T> BaseT;

    public:
        HeaderFieldWithTolerance(std::string const& header_label, Header& header, ToleranceType const&);
        HeaderFieldWithTolerance& operator=(T const& var);
        
        bool operator==(const HeaderFieldBase&) const override;
        bool operator==(const HeaderFieldWithTolerance&) const;

    private:
        ToleranceType _tolerance;
};


/**
 * @brief specialisation for writing and reading the sigproc vector type
 * @details
 *     A start of a vector in the header is marked with a start_label (e.g. FREQUENCY_START)
 *     A end of a vector in the header is marked with an end_label    (e.g. FREQUENCY_END)
 *     In between each element is marked with its own item_tag        (e.g. fchannel)
 */
template<typename T>
class HeaderField<std::vector<T>> : public HeaderFieldBase
{
        typedef HeaderFieldBase BaseT;
        // vectors are marked with start and end tags
        class NullField : public HeaderFieldBase {
            public:
                NullField() {}
                // do content other than the label
                bool is_set() const override { return false; }
                void reset() override {};
        };

        class ItemField : public HeaderFieldBase {
            // only needs to read
            public:
                ItemField(std::vector<T>& vec) : _vec(vec) {}
                bool is_set() const override { return false; }
                unsigned read(std::istream &) override;
                void reset() override {};

            private:
                std::vector<T>& _vec;
        };

    public:
        HeaderField(std::string const& start_label, std::string const& item_label, std::string const& end_label, Header& header);

        operator std::vector<T> const&() const { return _var; }
        operator std::vector<T>&() { return _var; }
        operator boost::optional<std::vector<T>> const&() const { return _var; }
        HeaderField& operator=(std::vector<T> const& var);

        unsigned read(std::istream &) override;
        unsigned write(std::ostream &) const override;
        std::string const& header_info(std::string const& h) const override;
        void write_info(std::ostream &) const override;
        bool is_set() const override;
        void reset() override;

        std::vector<T> const& operator*() const { return _var; };
        std::vector<T>& operator*() { return _var; };

        bool operator==(const HeaderFieldBase&) const override;
        bool operator==(const HeaderField&) const;

    private:
        std::vector<T>  _var;
        ItemField       _item_label_handler;
        NullField       _end_label_handler;
        SigProcLabel    _end_label;
        SigProcLabel    _item_label;
};

} // namespace sigproc
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_SIGPROC_HEADERFIELD_H
