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
#include "pss/astrotypes/types/test/ExtendedTimeFrequencyTest.h"
#include "pss/astrotypes/types/ExtendedTimeFrequency.h"


namespace pss {
namespace astrotypes {
namespace types {
namespace test {


ExtendedTimeFrequencyTest::ExtendedTimeFrequencyTest()
    : ::testing::Test()
{
}

ExtendedTimeFrequencyTest::~ExtendedTimeFrequencyTest()
{
}

void ExtendedTimeFrequencyTest::SetUp()
{
}

void ExtendedTimeFrequencyTest::TearDown()
{
}

template<typename T>
class TestExtendedTimeFrequency : public ExtendedTimeFrequency<T>
{
    public:
        template<typename... Args>
        TestExtendedTimeFrequency(Args&&... args)
            : ExtendedTimeFrequency<T>(std::forward<Args>(args)...)
        {}
};

template<typename TimeFrequencyType, typename T>
static void check_const_interface(TimeFrequencyType const& data, ExtendedTimeFrequency<T> const& ex_tf, bool same_obj=true)
{
    ASSERT_EQ(data.number_of_channels(), ex_tf.number_of_channels());
    ASSERT_EQ(data.number_of_spectra(), ex_tf.number_of_spectra());
    ASSERT_EQ(data.spectrum(1), ex_tf.spectrum(1));
    ASSERT_EQ(data.channel(1), ex_tf.channel(1));
    if(same_obj) {
        ASSERT_EQ(&*data.begin(), &*ex_tf.begin());
        ASSERT_EQ(&*data.end(), &*ex_tf.end());
    }
    else {
        ASSERT_NE(&*data.begin(), &*ex_tf.begin());
        ASSERT_NE(&*data.end(), &*ex_tf.end());
    }
    ASSERT_EQ(data[DimensionIndex<units::Time>(1)], ex_tf[DimensionIndex<units::Time>(1)]);
}

template<typename TimeFrequencyType, typename T>
static void check_interface(TimeFrequencyType& data, ExtendedTimeFrequency<T>& ex_tf, bool same_obj = true)
{
    check_const_interface(data, ex_tf, same_obj);
    ASSERT_EQ(data.spectrum(1), ex_tf.spectrum(1));
    ASSERT_EQ(data.channel(1), ex_tf.channel(1));
    if(same_obj) {
        ASSERT_EQ(&*data.begin(), &*ex_tf.begin());
        ASSERT_EQ(&*data.end(), &*ex_tf.end());
    }
    else {
        ASSERT_NE(&*data.begin(), &*ex_tf.begin());
        ASSERT_NE(&*data.end(), &*ex_tf.end());
    }
    ASSERT_EQ(data[DimensionIndex<units::Time>(1)], ex_tf[DimensionIndex<units::Time>(1)]);
}

TEST_F(ExtendedTimeFrequencyTest, test_type)
{
    typedef TimeFrequency<uint8_t> TimeFrequencyType;
    TimeFrequencyType data(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(12));
    TestExtendedTimeFrequency<TimeFrequencyType> ex_tf(data);
    check_interface(data, ex_tf, false);
}

TEST_F(ExtendedTimeFrequencyTest, test_ref_type)
{
    typedef TimeFrequency<uint8_t> TimeFrequencyType;
    TimeFrequencyType data(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(12));
    TestExtendedTimeFrequency<TimeFrequencyType&> ex_tf(data);
    check_interface(data, ex_tf);
}

TEST_F(ExtendedTimeFrequencyTest, test_shared_ptr_type)
{
    typedef TimeFrequency<uint8_t> TimeFrequencyType;
    typedef std::shared_ptr<TimeFrequencyType> TimeFrequencyPtrType;
    TimeFrequencyPtrType data=std::make_shared<TimeFrequencyType>(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(12));
    TestExtendedTimeFrequency<std::shared_ptr<TimeFrequencyType>> ex_tf(data);
    check_interface(*data, ex_tf);
}

TEST_F(ExtendedTimeFrequencyTest, test_const_shared_ptr_type)
{
    typedef const TimeFrequency<uint8_t> TimeFrequencyType;
    typedef std::shared_ptr<TimeFrequencyType> TimeFrequencyPtrType;
    TimeFrequencyPtrType data=std::make_shared<TimeFrequencyType>(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(12));
    TestExtendedTimeFrequency<std::shared_ptr<TimeFrequencyType>> ex_tf(data);
    check_const_interface(*data, ex_tf);
}

/*
TEST_F(ExtendedTimeFrequencyTest, test_unique_ptr_type)
{
    typedef TimeFrequency<uint8_t> TimeFrequencyType;
    typedef std::unique_ptr<TimeFrequencyType> TimeFrequencyPtrType;
    TimeFrequencyPtrType data(new TimeFrequencyType(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(12)));
    TestExtendedTimeFrequency<std::unique_ptr<TimeFrequencyType>> ex_tf(data);
    check_interface(*data, ex_tf);
    ASSERT_EQ(&*data->begin(), &*ex_tf.begin());
    ASSERT_EQ(&*data->end(), &*ex_tf.end());
}

TEST_F(ExtendedTimeFrequencyTest, test_const_unique_ptr_type)
{
    typedef const TimeFrequency<uint8_t> TimeFrequencyType;
    typedef std::unique_ptr<TimeFrequencyType> TimeFrequencyPtrType;
    TimeFrequencyPtrType data(new TimeFrequencyType(DimensionSize<units::Time>(10), DimensionSize<units::Frequency>(12)));
    TestExtendedTimeFrequency<std::unique_ptr<TimeFrequencyType>> ex_tf(data);
    check_const_interface(*data, ex_tf);
    ASSERT_EQ(&*data.begin(), &*ex_tf.begin());
    ASSERT_EQ(&*data.end(), &*ex_tf.end());
    ASSERT_EQ(&*data->begin(), &*ex_tf.begin());
    ASSERT_EQ(&*data->end(), &*ex_tf.end());
}
*/
} // namespace test
} // namespace types
} // namespace astrotypes
} // namespace pss
