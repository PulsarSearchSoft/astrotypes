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
#ifndef PSS_ASTROTYPES_SIGPROC_DATAFACTORY_H
#define PSS_ASTROTYPES_SIGPROC_DATAFACTORY_H

#include "Header.h"
#include "DefaultDataFactoryTraits.h"
#include "SigProcFormat.h"
#include <array>
#include <memory>
#include <exception>

namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief Determines data type from a set of runtime parmmeters
 *        Once determined the user provided template function is called.
 * @param FnTemplate a templated class or struct with a static method
 *        @code
 *        template<typename SigProcInputTraits>
 *        struct MyDataHandler {
 *           static void exec(ArgA a, ArgB b); // you can specify any number of args
 *        };
 *        @endcode
 *        The template paramter will be set to the correct data type
 *
 * @param DataFactoryTraits Allows customisation for setting the mappings between
 *        the number of bits, the numdeb of IF streams, and the types that they
 *        should be represented by. If you create custom types see the DefaultDataFactoryTraits
 *        for more information on how to modify.
 *
 * @details
 *        As the data types have no base classes we cannot follow
 *        the normal factory pattern of returning types on request.
 *        Instead we can determine the type and pass this on to a templated function
 *        to do the work with that data type.
 *        The code below uses the header info to determine the type
 *        @code
 *        ArgA a;
 *        ArgB b;
 *        Header header;
 *        input_stream >> header;
 *        DataFactory<MyDataHandler>::exec(header, a, b);
 *        @endcode
 *
 *        Note on Extending: To support further Header::DataTypes, inherit from this class, otherwise
 *        use the DataFactoryTraits.
 *
 *        See also sigproc_cat example.
 */
template<template<typename> class FnTemplate, typename DataFactoryTraits = DefaultDataFactoryTraits>
class DataFactory
{
    public:
        // type parameter passed to the users templated function
        template<typename DataT, typename AdapterT>
        struct SigProcInputTraits {
            /**
             * @brief the datatype that matched the format of the file
             * @details You can use either TimeFrequency or FrequencyTime data types
             *          but this type will be the one that matches the layout of the
             *          sigproc file stream and therefore likely to be the moist optimal
             *          in read speed.
             */
            typedef DataT DataType;

            /**
             * @brief The type of Stream Adapter required to interpret input streams
             * @details Stream Adapter that is compatible with the sigproc header for both reading and writing
             */
            typedef AdapterT Adapter;
        };

    protected:

        // type generator helpers
        template<Header::DataType DataType, std::size_t nifs, std::size_t nbits>
        struct MultiArrayType;

        template<std::size_t nifs, std::size_t nbits>
        struct MultiArrayType<Header::DataType::FilterBank, nifs, nbits> {
            //typedef TimeFrequency<typename ElementType<nifs, nbits>::type, Allocator> type;
            typedef TimeFrequency<typename DataFactoryTraits::template ElementTypeMap<nifs, nbits>::type> type;
        };

        template<std::size_t nifs, std::size_t nbits>
        struct MultiArrayType<Header::DataType::TimeSeries, nifs, nbits> {
            //typedef FrequencyTime<typename ElementType<nifs, nbits>::type, Allocator> type;
            typedef FrequencyTime<typename DataFactoryTraits::template ElementTypeMap<nifs, nbits>::type> type;
        };

        template<Header::DataType DataType, bool Dummy=true>
        struct SigProcAdapterType {};

        template<bool Dummy>
        struct SigProcAdapterType<Header::DataType::FilterBank, Dummy> {
            typedef SigProcFormat<units::Time, units::Frequency> type;
        };

        template<bool Dummy>
        struct SigProcAdapterType<Header::DataType::TimeSeries, Dummy> {
            typedef SigProcFormat<units::Frequency, units::Time> type;
        };

    private:
        // these maps generate the code to select the correct type from runtime variables
        template<Header::DataType DataType, unsigned Nifs, unsigned Nbits=8>
        struct BitsRuntimeMap {
            template<typename... Args>
            static
            void exec(unsigned number_of_bits, Args&&... args) {
                if(number_of_bits != Nbits) BitsRuntimeMap<DataType, Nifs, BitToUnsignedInt<Nbits>::next>::exec(number_of_bits, std::forward<Args>(args)...);
                FnTemplate<
                    SigProcInputTraits<
                        typename MultiArrayType<DataType, Nifs, Nbits>::type
                       ,typename SigProcAdapterType<DataType>::type
                    >>::exec(std::forward<Args>(args)...);
            }
        };

        template<Header::DataType DataType, unsigned Nifs>
        struct BitsRuntimeMap<DataType, Nifs, 0> {
            template<typename... Args>
            static
            void exec(unsigned number_of_bits, Args&&...) {
                static const std::string err(std::string("number of bits not supported: "));
                throw std::runtime_error(std::string(err + std::to_string(number_of_bits)));
            }
        };

        template<Header::DataType DataType, unsigned N=1>
        struct NifsRuntimeMap {
            template<typename... Args>
            static
            void exec(unsigned nifs, unsigned number_of_bits, Args&&... args) {
                if(nifs != N) NifsRuntimeMap<DataType, N+1>::exec(nifs, number_of_bits, std::forward<Args>(args)...);
                BitsRuntimeMap<DataType, N>::exec(number_of_bits, std::forward<Args>(args)...);
            }
        };

        template<Header::DataType DataType>
        struct NifsRuntimeMap<DataType, DataFactoryTraits::MaxNifs + 1> {
            template<typename... Args>
            static
            void exec(unsigned, unsigned, Args&&...) {
                static const std::string err(std::string("max nifs supported=") + std::to_string(DataFactoryTraits::MaxNifs));
                throw std::runtime_error(err);
            }
        };

    public:
        template<typename... Args>
        static
        void exec(Header const& h, Args&&... args)
        {
            if(h.data_type() == Header::DataType::FilterBank)
            {
                NifsRuntimeMap<Header::DataType::FilterBank>::exec(h.number_of_ifs(), h.number_of_bits(), std::forward<Args>(args)...);
            }
            else if(h.data_type() == Header::DataType::TimeSeries)
            {
                NifsRuntimeMap<Header::DataType::TimeSeries>::exec(h.number_of_ifs(), h.number_of_bits(), std::forward<Args>(args)...);
            }
            else {
                throw std::runtime_error("sigproc: unsupported data type");
            }
        }
};

} // namespace sigproc
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_SIGPROC_DATAFACTORY_H
