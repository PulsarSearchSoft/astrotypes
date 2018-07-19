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
#ifndef PSS_ASTROTYPES_SIGPROC_SIGPROCHEADER_H
#define PSS_ASTROTYPES_SIGPROC_SIGPROCHEADER_H

#include "HeaderField.h"
#include "SigProcVariable.h"
#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Frequency.h"
#include "pss/astrotypes/units/Angle.h"
#include "pss/astrotypes/units/DispersionMeasure.h"
#include "pss/astrotypes/types/TimeFrequency.h"
#include <boost/optional.hpp>
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
 *    As a consequence the type often returned is a boost::optional<T>.
 *    This class is designed to allow you to test if the value has been set
 *    or not.
 *    e.g.
 * @code
 *      if(!header.source_name()) {
 *          // oh oh no source name,  better deal with it
 *          ...
 *      }
 *      // once tested you can get the underlying vaiable with the dereference operator *
 *      std::string const& source = *header.source_name();
 * @endcode
 *
 *    note also that many of the types returned as types with specific units. If your writting new code
 *    its a good idea to keep this type information when you pass this value around, but for compatibility
 *    with less type safe codes and you need just the numerical valuem you can call the value() mehtod
 * @code
 *    // n.b. this loses type information.
 *    double tsamp_in_secs = header.sample_interval().value();
 * @endcode
 */

class SigProcHeader
{
    private:
        typedef SigProcHeader SelfType;

        template<typename T>
        class OStreamAdapter {
            public:
                OStreamAdapter(SigProcHeader const& h, T const& data);
                std::ostream& operator<<(std::ostream& os) const;
            private:
                SigProcHeader const& _h;
                T const& _d;
        };

        friend class HeaderFieldBase;

    public:
        SigProcHeader();

        /**
         * @brief return the telescope_id identifier (if any)
         */
        boost::optional<unsigned> telescope_id() const;

        /**
         * @brief set the telescope_id identifier
         */
        void telescope_id(unsigned id);

        /**
         * @brief return the machine_id identifier (if any)
         */
        boost::optional<unsigned> machine_id() const;

        /**
         * @brief set the machine_id identifier
         * @details optional
         */
        void machine_id(unsigned id);

        /**
         * @brief return the value of the raw data filename (if any)
         */
        boost::optional<std::string> const& raw_data_file() const;

        /**
         * @brief set a value for the raw data filename
         * @details optional
         */
        void raw_data_file(std::string const& name);

        /**
         * @brief return the value of the source name (if any)
         */
        boost::optional<std::string> const& source_name() const;

        /**
         * @brief set a value for the source name
         */
        void source_name(std::string const& name);

        /**
         * @brief the value of the barycentric field (if any)
         */
        boost::optional<unsigned> const& barycentric() const;
        void barycentric(bool barycentric);

        /**
         * @brief the value of the pulsarcentric field (if any)
         */
        boost::optional<unsigned> const& pulsarcentric() const;
        void pulsarcentric(bool pulsarcentric);

        /**
         * @brief getters/setters for the telscope azimuth at start of scan (in degrees)
         */
        boost::optional<boost::units::quantity<units::Degree, double>> az_start() const;
        void az_start(boost::units::quantity<units::Degree, double> const&);

        /**
         * @brief getters/setters for the telscope zenith at start of scan (in degrees)
         */
        boost::optional<boost::units::quantity<units::Degree, double>> za_start() const;
        void za_start(boost::units::quantity<units::Degree, double> const&);

        /**
         * @brief return the sample_interval (if defined)
         */
        boost::units::quantity<Seconds, double> sample_interval() const;

        /**
         * @brief set sample_interval , the amount of time between two adjacent spectra
         */
        void sample_interval(boost::units::quantity<Seconds, double>);

        /**
         * @brief get the frequency of the first channel
         * @detials optional
         */
        boost::optional<boost::units::quantity<MegaHertz, double>> const& fch1() const;

        /**
         * @brief set the frequency of the first channel
         * @details optional
         *          if you set this you should also set foff
         */
        void fch1(boost::units::quantity<MegaHertz, double> const&);

        /**
         * @brief get the band width of a channel (assumes homogenous badwidth for each channel)
         * @detials optional
         */
        boost::optional<boost::units::quantity<MegaHertz, double>> const& foff() const;

        /**
         * @brief set the frequency width of the channel (for fixed band width channels)
         * @details optional
         *          if you set this you should also set fch1
         */
        void foff(boost::units::quantity<MegaHertz, double> const&);

        /**
         * @brief return a list of frequency channels (if set)
         * @details optional (alternative if foff() and fch1())
         *          test size() of vector to see if empty
         */
        std::vector<boost::units::quantity<MegaHertz, double>> const& frequency_channels() const;

        /**
         * @brief set a list of frequency channels (optional)
         * @details set this OR fch1() and foff()
         */
        void frequency_channels(std::vector<boost::units::quantity<MegaHertz, double>> const& frequency_channels);

        /**
         * @brief return the number of frequency channels
         */
        std::size_t number_of_channels() const;

        /// set the number of channels
        void number_of_channels(std::size_t);

        /**
         * @brief return the number of vits
         */
        unsigned number_of_bits() const;
        void number_of_bits(unsigned);

        /**
         * @brief return the number of streams (e.g. polarisations)
         */
        unsigned number_of_ifs() const;
        void number_of_ifs(unsigned);

        /**
         * @brief return the reference Dispersion Measure
         * @details optional
         */
        boost::optional<DispersionMeasure<double>> const& ref_dm() const;

        /**
         * @brief set the reference Dispersion Measure
         * @details optional
         */
        void ref_dm(DispersionMeasure<double>);

        /**
         * @brief return the folding period (if defined)
         */
        boost::optional<boost::units::quantity<Seconds, double>> const& period() const;

        /**
         * @brief set folding period period
         * @details optional
         */
        void period(boost::units::quantity<Seconds, double>);

        /**
         * @brief read in header data from the provided stream
         * @details will throw if the data stream is not in the expected format or eof
         * @throw if attempt to write out of bounds data (e.g strings > 80 chars)
         */
        void read(std::istream & stream);

        /**
         * @brief write header data to the provided stream
         * @details
         * @throw if attemp to read out of bounds data (e.g strings > 80 chars)
         */
        void write(std::ostream & stream) const;

        /**
         * @brief write header data to the provided object, resizing appropriately
         */
        template<typename T, typename Alloc>
        SelfType& operator>>(astrotypes::FrequencyTime<T, Alloc> const&) const;

        /**
         * @brief write header data to the provided object, resizing appropriately
         */
        template<typename T, typename Alloc>
        SelfType& operator>>(astrotypes::TimeFrequency<T, Alloc> const&) const;

        /**
         * @brief read header data from the provided object
         */
        template<typename T, typename Alloc>
        SelfType& operator<<(astrotypes::FrequencyTime<T, Alloc> const&) const;

        /**
         * @brief read header data from the provided object
         */
        template<typename T, typename Alloc>
        OStreamAdapter<astrotypes::TimeFrequency<T, Alloc>> operator<<(astrotypes::TimeFrequency<T, Alloc> const&);

        /**
         * @brief returns true if the data formats match
         * @details source names/machine names are ignored
         *          this only matches data formatting types and offsets
         */
        bool operator==(SigProcHeader const&) const;

        /**
         * @brief returns true if the data format meta data does not match
         * @details see caveats in operator==
         */
        bool operator!=(SigProcHeader const&) const;

        /**
         * @brief returns the number of bytes in the header
         */
        std::size_t size() const;

        /**
         * @brief reset all header variables to an undefined state
         */
        void reset();

    protected:
        template<typename T>
        std::runtime_error parse_error(std::string const& msg, T const& msg2);

        /**
         * @brief add a field to be parsed for read and write
         */
        void add(std::string const& label, HeaderFieldBase& field);

        /**
         * @brief add a field to be parsed for read only
         */
        void add_read(std::string const& label, HeaderFieldBase& field);


    private:
        mutable unsigned _size; // byte size of the header

        std::map<SigProcLabel, HeaderFieldBase*> _headers;
        std::map<SigProcLabel, HeaderFieldBase*> _read_only_headers;

        // standard header data fields
        HeaderField<unsigned>           _telescope_id;
        HeaderField<unsigned>           _machine_id;
        HeaderField<unsigned>           _data_type;
        HeaderField<std::string>        _raw_data_file;
        HeaderField<std::string>        _source_name;
        HeaderField<unsigned>           _barycentric;
        HeaderField<unsigned>           _pulsarcentric;
        HeaderField<boost::units::quantity<units::Degree, double>>  _az_start;  // in degrees
        HeaderField<boost::units::quantity<units::Degree, double>>  _za_start;  // in degrees
        HeaderField<double>             _src_raj;
        HeaderField<double>             _src_dej;
        HeaderField<double>             _tstart;    // Modified Julian Date format
        HeaderField<boost::units::quantity<Seconds, double>>        _tsamp;     // sample time (in seconds)
        HeaderField<unsigned>                                       _n_bits;
        HeaderField<unsigned>                                       _nsamples;
        HeaderField<boost::units::quantity<MegaHertz, double>>      _fch1;
        HeaderField<boost::units::quantity<MegaHertz, double>>      _foff;
        HeaderField<std::vector<boost::units::quantity<MegaHertz, double>>>   _freq_channels; // each bin assigned its own value
        HeaderField<unsigned>                                       _n_chans;
        HeaderField<unsigned>                                       _nifs;        // number of seperate IF channels
        HeaderField<DispersionMeasure<double>>                      _refdm;       // parsecs_per_cm_cubed
        HeaderField<boost::units::quantity<Seconds, double>>        _period;      // folding period seconds
};

std::ostream& operator<<(std::ostream& os, SigProcHeader const&);
std::istream& operator>>(std::istream& os, SigProcHeader&);

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/SigProcHeader.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_SIGPROCHEADER_H
