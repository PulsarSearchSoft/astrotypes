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
#ifndef PSS_ASTROTYPES_SIGPROC_DEFAULTDATAFACTORYTRAITS_H
#define PSS_ASTROTYPES_SIGPROC_DEFAULTDATAFACTORYTRAITS_H


namespace pss {
namespace astrotypes {
namespace sigproc {

/**
 * @brief Maps number_of_bits to unsigned integer types
 * @param type  : the mapped type
 * @param next  : the next nbits value in the sequence of BitToUnsignedInt
 *                the sequence should be terminated by setting this to 0
 */
template<std::size_t nbits>
struct BitToUnsignedInt {
    static constexpr unsigned next = 0; // last one
};

template<>
struct BitToUnsignedInt<64> {
    typedef uint32_t type;
    static constexpr unsigned next = 0; // last one
};

template<>
struct BitToUnsignedInt<32> {
    typedef uint32_t type;
    static constexpr unsigned next = 64;
};

template<>
struct BitToUnsignedInt<16> {
    typedef uint16_t type;
    static constexpr unsigned next = 32;
};

template<>
struct BitToUnsignedInt<8> {
    typedef uint8_t type;
    static constexpr unsigned next = 16;
};

/**
 * @brief Maps number_of_if_streams to the container or raw type 
 * @param type        : the mapped type
 * @tparam nifs       : The number of if streams
 * @tparam nifs       : the number of bits (to be passed to BinTypeMap)
 * @tparam BitTypeMap : the map of nbits to type (e.g BitToUnsignedInt)
 *                      typcially the elements inside the container @param type
 *
 * @details           : uses std::arrays of nifs rank where nifs>1, otherwise
 *                      the type provided by BitTypeMap<nbits>::type
 */
template<unsigned nifs, unsigned nbits, template<unsigned> class BitTypeMap>
struct ElementType {
    typedef std::array<typename BitTypeMap<nbits>::type, nifs> type;
};

template<unsigned nbits, template<unsigned> class BitTypeMap>
struct ElementType<1, nbits, BitTypeMap> {
    typedef typename BitTypeMap<nbits>::type type;
};

template<unsigned nbits, template<unsigned> class BitTypeMap>
struct ElementType<0, nbits, BitTypeMap> {
    typedef typename BitTypeMap<nbits>::type type;
};

/**
 * @brief Default settings for iuse with the @class DataFactory
 */
struct DefaultDataFactoryTraits {

    static constexpr unsigned MaxNifs = 1; // maximum number of data for each channel (e.g 4 polarisations)

    /// The mapping of th number of bits to the NumericalReapresentation of those bits
    template<unsigned nbits>
    using BitsTypeMap = BitToUnsignedInt<nbits>;

    /// The allocator type of the data types that will be generated
    template<typename T>
    using AllocatorMap = std::allocator<T>;

    /**
     * @brief Specifies the mapping from the type that is associated with the 
     *        different values of the number of seperate IF streams
     *        for each channel.
     */
    template<unsigned nifs, unsigned nbits>
    using ElementTypeMap = ElementType<nifs, nbits, BitsTypeMap>; 
    
};


} // namespace sigproc
} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_SIGPROC_DEFAULTDATAFACTORYTRAITS_H
