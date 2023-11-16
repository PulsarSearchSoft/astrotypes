/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef PSS_ASTROTYPES_TYPES_ACCUMULATOR_H
#define PSS_ASTROTYPES_TYPES_ACCUMULATOR_H

#include "pss/astrotypes/multiarray/MultiArray.h"
#include "pss/astrotypes/units/Phase.h"
//#include "cheetah/data/Candidate.h"
//#include "cheetah/data/Units.h"

namespace pss {
namespace astrotypes {
namespace types {

/**
 * @class PhaseFrequencyArray
 * @brief An array for accumulating time/frequency data into
 *        a folded SubInt during the folding step of Fldo
 *
 * @details A phase/frequency array used to temporarily store data during the
 *          folding process of Fldo. Stores up to one Subint of data, but can
 *          be flushed early if the storage data type is close to overflowing.
 *          This is used as an intermediate step to improve performance of
 *          folding data on a CPU.
 *
 */

template<typename SliceT>
class PhaseFrequencyArrayInterface : public SliceT
{
    protected:
        typedef typename SliceT::SliceType SliceType;

    public:
        typedef typename SliceType::template OperatorSliceType<units::Frequency>::type Channel;
        typedef typename SliceType::template ConstOperatorSliceType<units::Frequency>::type ConstChannel;
        typedef typename SliceType::template OperatorSliceType<units::PhaseAngle>::type PhaseBin;
        typedef typename SliceType::template ConstOperatorSliceType<units::PhaseAngle>::type ConstPhaseBin;


    public:
        // Expose SliceT constructors
        using SliceT::SliceT;

    public:
        PhaseFrequencyArrayInterface();
        PhaseFrequencyArrayInterface(PhaseFrequencyArrayInterface const&);
        PhaseFrequencyArrayInterface(SliceT const& t);
        PhaseFrequencyArrayInterface(SliceT&& t);

        PhaseFrequencyArrayInterface& operator=(PhaseFrequencyArrayInterface const&);

        /** @brief   Return a single frequency channel across all phase bins
         *
         *  @details The type returned is a MultiArray @class Slice
         *           with all the features of that available
         *
         *  @example
         *  @code
         *  // Create an phase_frequency_array array
         *  PhaseFrequencyArray phase_frequency_array(DimensionSize<Phase>(256)
         *                          , DimensionSize<Frequency>(4096)
         *                         );
         *
         *  // Get the first frequency channel
         *  typename PhaseFrequencyArray::Channel channel = phase_frequency_array.channel(0);
         *  @endcode
         */
        Channel channel(std::size_t channel_number);
        ConstChannel channel(std::size_t channel_number) const;

        /** @brief   Return a single phase bin across all frequency channels
         *
         *  @details The type returned is a MultiArray @class Slice
         *           with all the features of that available
         *
         *  @example
         *  @code
         *  // Create an phase_frequency_array array
         *  PhaseFrequencyArray phase_frequency_array(DimensionSize<phase>(256)
         *                          , DimensionSize<Frequency>(4096)
         *                         );
         *
         *  // Get the first phase bin
         *  typename PhaseFrequencyArray::PhaseBin phase_bin = phase_frequency_array.phase_bin(0);
         *  @endcode
         */
        PhaseBin phase_bin(std::size_t phase_bin_number);
        ConstPhaseBin phase_bin(std::size_t phase_bin_number) const;

        /** @brief   Return the number of frequency channels in the data structure
         *  @details A synonym for dimension<Frequency>()
         */
        std::size_t number_of_channels() const;

        /** @brief   Return the number of phase bins in the data structure
         *  @details A synonym for dimension<Phase>()
         */
        std::size_t number_of_phase_bins() const;

};

template<typename NumericalT, typename Alloc=std::allocator<NumericalT>>
class PhaseFrequencyArray : public PhaseFrequencyArrayInterface<
                                          multiarray::MultiArray<
                                          Alloc
                                          , NumericalT
                                          , PhaseFrequencyArrayInterface
                                          , units::PhaseAngle
                                          , units::Frequency
                                      >
                                  >
{

    private:
        typedef PhaseFrequencyArrayInterface<
                    multiarray::MultiArray<
                        Alloc
                        , NumericalT
                        , PhaseFrequencyArrayInterface
                        , units::PhaseAngle
                        , units::Frequency
                    >
                >
                BaseT;

    public:
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;
        typedef typename BaseT::PhaseBin PhaseBin;
        typedef typename BaseT::ConstPhaseBin ConstPhaseBin;

        typedef typename BaseT::SliceType SliceType;

        typedef NumericalT value_type;
        //typedef Candidate<Cpu, float> CandidateType;

    public:
        PhaseFrequencyArray();
        PhaseFrequencyArray(DimensionSize<units::PhaseAngle>
                    , DimensionSize<units::Frequency>
                   );
        ~PhaseFrequencyArray() = default;

};

} // namespace types
} // namespace astrotypes
} // namespace pss

#include "detail/PhaseFrequencyArray.cpp"

#endif // PSS_ASTROTYPES_TYPES_ACCUMULATOR_H
