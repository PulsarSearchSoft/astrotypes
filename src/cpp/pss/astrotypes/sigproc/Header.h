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
#ifndef PSS_ASTROTYPES_SIGPROC_HEADER_H
#define PSS_ASTROTYPES_SIGPROC_HEADER_H

#include "HeaderField.h"
#include "detail/HeaderFieldDataType.h"
#include "detail/HeaderBase.h"
#include "pss/astrotypes/utils/Optional.h"
#include "pss/astrotypes/units/Time.h"
#include "pss/astrotypes/units/Frequency.h"
#include "pss/astrotypes/units/Angle.h"
#include "pss/astrotypes/units/DispersionMeasure.h"
#include "pss/astrotypes/types/TimeFrequency.h"

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
 *      if(!header.source_name().is_set()) {
 *          // oh oh no source name,  better deal with it
 *          ...
 *      }
 *      // once tested you can get the underlying variable with the dereference operator *
 *      std::string const& source = *header.source_name();
 * @endcode
 *
 *    note also that many of the types returned as types with specific units. If your writing new code
 *    its a good idea to keep this type information when you pass this value around, but for compatibility
 *    with less type safe codes and you need just the numerical valuem you can call the value() mehtod
 * @code
 *    // n.b. this loses type information.
 *    double tsamp_in_secs = header.sample_interval().value();
 * @endcode
 *
 * @subsection Extending for Custom types
 */

class Header : public HeaderBase<Header>
{
    private:
        typedef HeaderBase<Header> BaseT;
        typedef Header SelfType;

        template<typename T>
        class OStreamAdapter {
            public:
                OStreamAdapter(Header const& h, T const& data);
                std::ostream& operator<<(std::ostream& os) const;
            private:
                Header const& _h;
                T const& _d;
        };

        friend class HeaderFieldBase;

        template<typename T>
        friend class HeaderFieldDataType;

    public:
        /// Adapter for ouputing debug info about the header
        using HeaderBase<Header>::Info;

        enum class DataType {
            Undefined  = 0,
            FilterBank = 1, // stored as a series of spectra
            TimeSeries      // stored as a series of channels
        };

    public:
        Header();
        Header(Header const&);
        virtual ~Header() {};

        Header& operator=(Header const&);

        /**
         * @brief return the telescope_id identifier (if any)
         */
        utils::Optional<unsigned> telescope_id() const;

        /**
         * @brief set the telescope_id identifier
         */
        void telescope_id(unsigned id);

        /**
         * @brief return the machine_id identifier (if any)
         */
        utils::Optional<unsigned> machine_id() const;

        /**
         * @brief set the machine_id identifier
         * @details optional
         */
        void machine_id(unsigned id);

        /**
         * @brief set the data type
         */
        void data_type(DataType type);

        /**
         * @brief set the data type
         */
        DataType data_type() const;

        /**
         * @brief return the value of the raw data filename (if any)
         */
        utils::Optional<std::string> const& raw_data_file() const;

        /**
         * @brief set a value for the raw data filename
         * @details optional
         */
        void raw_data_file(std::string const& name);

        /**
         * @brief return the value of the source name (if any)
         */
        utils::Optional<std::string> const& source_name() const;

        /**
         * @brief set a value for the source name
         */
        void source_name(std::string const& name);

        /**
         * @brief the value of the barycentric field (if any)
         */
        utils::Optional<unsigned> const& barycentric() const;
        void barycentric(bool barycentric);

        /**
         * @brief the value of the pulsarcentric field (if any)
         */
        utils::Optional<unsigned> const& pulsarcentric() const;
        void pulsarcentric(bool pulsarcentric);

        /**
         * @brief getters/setters for the telscope azimuth at start of scan (in degrees)
         */
        utils::Optional<boost::units::quantity<units::Degree, double>> az_start() const;
        void az_start(boost::units::quantity<units::Degree, double> const&);

        /**
         * @brief getters/setters for the telscope zenith at start of scan (in degrees)
         */
        utils::Optional<boost::units::quantity<units::Degree, double>> za_start() const;
        void za_start(boost::units::quantity<units::Degree, double> const&);

        /**
         * @brief return the date of the start of the data block
         */
        utils::Optional<units::ModifiedJulianDate> const& tstart() const;

        /**
         * @brief set the date of the start of the data block
         */
        void tstart(units::ModifiedJulianDate const&);

        /**
         * @brief return the sample_interval (if defined)
         */
        boost::units::quantity<units::Seconds, double> sample_interval() const;

        /**
         * @brief set sample_interval , the amount of time between two adjacent spectra
         */
        void sample_interval(boost::units::quantity<units::Seconds, double>);

        /**
         * @brief get the frequency of the first channel
         * @details optional
         */
        utils::Optional<boost::units::quantity<units::MegaHertz, double>> const& fch1() const;

        /**
         * @brief set the frequency of the first channel
         * @details optional
         *          if you set this you should also set foff
         */
        void fch1(boost::units::quantity<units::MegaHertz, double> const&);

        /**
         * @brief get the band width of a channel (assumes homogenous badwidth for each channel)
         * @details optional
         */
        utils::Optional<boost::units::quantity<units::MegaHertz, double>> const& foff() const;

        /**
         * @brief set the frequency width of the channel (for fixed band width channels)
         * @details optional
         *          if you set this you should also set fch1
         */
        void foff(boost::units::quantity<units::MegaHertz, double> const&);

        /**
         * @brief return a list of frequency channels (if set)
         * @details optional (alternative if foff() and fch1())
         *          test size() of vector to see if empty
         */
        std::vector<boost::units::quantity<units::MegaHertz, double>> const& frequency_channels() const;

        /**
         * @brief set a list of frequency channels (optional)
         * @details set this OR fch1() and foff()
         */
        void frequency_channels(std::vector<boost::units::quantity<units::MegaHertz, double>> const& frequency_channels);

        /**
         * @brief return the number of frequency channels
         */
        DimensionSize<units::Frequency> number_of_channels() const;

        /**
         * @brief set the number of channels
         */
        void number_of_channels(std::size_t);

        /**
         * @brief return the number of bits
         */
        unsigned number_of_bits() const;

        /**
         * @brief set the number of bits
         */
        void number_of_bits(unsigned);

        /**
         * @brief return the number of time samples (optional)
         * @details optional field.
         * @deprecated
         *          This can usually be calculated from the file size instead
         */
        utils::Optional<unsigned> const& number_of_samples() const;

        /**
         * @brief set the number of time samples (optional)
         * @deprecated
         */
        void number_of_samples(unsigned);

        /**
         * @brief return the number of IF streams (e.g. polarisations) in each channel
         * @details if not set will return 1
         */
        unsigned number_of_ifs() const;

        /**
         * @brief set the number of IF streams (e.g. polarisations) in each channel
         */
        void number_of_ifs(unsigned);

        /**
         * @brief return the reference Dispersion Measure
         * @details optional
         */
        utils::Optional<units::DispersionMeasure<double>> const& ref_dm() const;

        /**
         * @brief set the reference Dispersion Measure
         * @details optional
         */
        void ref_dm(units::DispersionMeasure<double>);

        /**
         * @brief return the folding period (if defined)
         */
        utils::Optional<boost::units::quantity<units::Seconds, double>> const& period() const;

        /**
         * @brief set folding period period
         * @details optional
         */
        void period(boost::units::quantity<units::Seconds, double>);

        /**
         * @brief return the beam index number
         * @details This is a common, but non-standard header item
         */
        utils::Optional<unsigned> ibeam() const;

        /**
         * @brief return the number of beams used in an abservation
         * @details This is a common, but non-standard header item
         */
        utils::Optional<unsigned> nbeams() const;

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
         * @brief human readable summary of the header written to the stream
         */
        void info(std::ostream& stream) const;

    protected:
        void updated_data_type();

    private:
        // standard header data fields
        HeaderField<unsigned>           _telescope_id;
        HeaderField<unsigned>           _machine_id;
        HeaderFieldDataType<unsigned>           _data_type;
        HeaderField<std::string>        _raw_data_file;
        HeaderField<std::string>        _source_name;
        HeaderField<unsigned>           _barycentric;
        HeaderField<unsigned>           _pulsarcentric;
        HeaderField<boost::units::quantity<units::Degree, double>>  _az_start;  // in degrees
        HeaderField<boost::units::quantity<units::Degree, double>>  _za_start;  // in degrees
        HeaderField<double>             _src_raj;                               // right ascension (J2000) of source (hhmmss.s)
        HeaderField<double>             _src_dej;                               // declination (J2000) of source (ddmmss.s)
        HeaderField<boost::units::quantity<units::Seconds, double>>        _tsamp;     // sample time (in seconds)
        HeaderFieldWithTolerance<units::ModifiedJulianDate, decltype(_tsamp) const&>   _tstart;    // Modified Julian Date format
        HeaderField<unsigned>                                              _n_bits;
        HeaderField<unsigned>                                              _nsamples;
        HeaderField<boost::units::quantity<units::MegaHertz, double>>      _fch1;
        HeaderField<boost::units::quantity<units::MegaHertz, double>>      _foff;
        HeaderField<std::vector<boost::units::quantity<units::MegaHertz, double>>>   _freq_channels; // each bin assigned its own value
        HeaderField<unsigned>                                              _n_chans;
        HeaderField<unsigned>                                              _nifs;        // number of seperate IF channels
        HeaderField<units::DispersionMeasure<double>>                      _refdm;       // parsecs_per_cm_cubed
        HeaderField<boost::units::quantity<units::Seconds, double>>        _period;      // folding period seconds

        // these fielde are not standard but added here to allow parsing
        HeaderField<unsigned>       _ibeam;      // the beam index
        HeaderField<unsigned>       _nbeams;     // the total number of beams in the observation
};

template<typename Stream>
Header::InfoSentry<Stream> operator<<(Stream& os, Header::Info const&);

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
#include "detail/Header.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_HEADER_H
