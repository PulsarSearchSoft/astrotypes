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


#include "pss/astrotypes/types/test/PhaseFrequencyArrayTest.h"
//#include "cheetah/data/Units.h"
#include <type_traits>
#include <numeric>

namespace pss {
namespace astrotypes {
namespace types {
namespace test {

template<typename TypeParam>
void PhaseFrequencyArrayTest<TypeParam>::SetUp()
{
    // Create a candidate data cube
    _phase_frequency_array = new PhaseFrequencyArray<typename TypeParam::value_type>(
                                                        DimensionSize<units::PhaseAngle>(256),
                                                        DimensionSize<units::Frequency>(4096));
}

template<typename TypeParam>
void PhaseFrequencyArrayTest<TypeParam>::TearDown()
{
    delete _phase_frequency_array;
}



//----------------------------- TEST DATA -----------------------------//
//---------------------------------------------------------------------//

// Helper to run specific tests for non-const data types
template<typename T, typename Enable=void>
struct IteratorAssignmentHelper
{
    template<typename SliceT>
    static inline void exec(SliceT& slice)
    {
        // Create a vector with the same size as the number of elements, with values incrementing up from 1
        std::vector<typename T::value_type> reference_data(slice.data_size());
        std::iota(reference_data.begin(), reference_data.end(), 1);

        // Use the iterator interface to set the elements to values incrementally increasing from 1
        std::copy(reference_data.begin(), reference_data.end(), slice.begin());

        // Verify that each element has been assigned a number increasing from 1
        ASSERT_TRUE(std::equal(slice.begin(), slice.end(), reference_data.begin()));
    }
};



// Test that the number_of_*() operators return
// the correct number of channels and phase bins
TYPED_TEST(PhaseFrequencyArrayTest, test_dimension_accessor_operators)
{
    ASSERT_EQ(this->_phase_frequency_array->number_of_channels(),   4096);
    ASSERT_EQ(this->_phase_frequency_array->number_of_phase_bins(), 256);
}



//------------------------------- SUBBAND -------------------------------//

// Test that the channel() operator returns the correct number of
// frequency channels (should be a single Channel) and phase bins
TYPED_TEST(PhaseFrequencyArrayTest, test_channel_operator)
{
    // Get the first frequency channel
    auto channel = this->_phase_frequency_array->channel(0);

    // Verify the dimensions of this slice are correct (
    // only one channel, i.e. a 1D array of phase bins)
    ASSERT_EQ(channel.number_of_channels(),   1);
    ASSERT_EQ(channel.number_of_phase_bins(), 256);
}

// Test that we can iterate properly over PhaseBins
TYPED_TEST(PhaseFrequencyArrayTest, test_channel_iterator)
{
    // Get the first channel (which is just an array of phase bins)
    auto phase_bin_array = this->_phase_frequency_array->channel(0);

    // Verify that each phase bin is set to a value of zero
    for(auto it = phase_bin_array.begin(); it != phase_bin_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the PhaseBin iterator is const when we specify a const iterator
    auto const_it = phase_bin_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each phase bin is set to a value of zero
    for(; const_it != phase_bin_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run tests for data types
    IteratorAssignmentHelper<TypeParam>::exec(phase_bin_array);
}



//------------------------------- PHASE -------------------------------//

// Test that the phase_bin() operator returns the correct number of
// frequency channels and phase bins (should be a single PhaseBin)
TYPED_TEST(PhaseFrequencyArrayTest, test_phase_operator)
{
    // Get the first phase bin
    auto phase_bin = this->_phase_frequency_array->phase_bin(0);

    // Verify the dimensions of this slice are correct (i.e. only one phase bin)
    ASSERT_EQ(phase_bin.number_of_channels(),   4096);
    ASSERT_EQ(phase_bin.number_of_phase_bins(), 1);
}

// Test that we can iterate properly over Channels
TYPED_TEST(PhaseFrequencyArrayTest, test_phase_bin_iterator)
{
    // Get the first phase bin (which is just an array of channels)
    auto channel_array = this->_phase_frequency_array->phase_bin(0);

    // Verify that each channel is set to a value of zero
    for(auto it = channel_array.begin(); it != channel_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the Channel iterator is const when we specify a const iterator
    auto const_it = channel_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each channel is set to a value of zero
    for(; const_it != channel_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run tests for data types
    IteratorAssignmentHelper<TypeParam>::exec(channel_array);
}

} // namespace test
} // namespace types
} // namespace astrotypes
} // namespace pss
