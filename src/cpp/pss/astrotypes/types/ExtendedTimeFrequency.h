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
#ifndef PSS_ASTROTYPES_TYPES_EXTENDEDTIMEFREQUENCY_H
#define PSS_ASTROTYPES_TYPES_EXTENDEDTIMEFREQUENCY_H

#include "TimeFrequency.h"
#include "pss/astrotypes/utils/TypeTraits.h"
#include <type_traits>

namespace pss {
namespace astrotypes {
namespace types {

/**
 * @brief A wrapper around the basic TimeFrequency data type, which is stored as a member.
 *        THis allows you to add extra information to the data whilst keeping the expected TimeFrequency interface
 * @details
 */

template<typename TimeFrequencyType>
class ExtendedTimeFrequency
{
    protected:
        typedef is_pointer_wrapper<typename std::remove_reference<TimeFrequencyType>::type> IsWrapped;
        typedef typename IsWrapped::type Type; // unwrapped from any pointer wrapping
        typedef is_pointer_wrapper<const typename std::remove_reference<TimeFrequencyType>::type> ConstIsWrapped;
        template<typename T>
        using IsNotRefType = typename std::enable_if<!std::is_reference<TimeFrequencyType>::value, T>::type;
        template<typename T>
        using HasCopyConstructor = typename std::enable_if<std::is_copy_constructible<TimeFrequencyType>::value, T>::type;

    public:
        typedef typename Type::Channel Channel;
        typedef typename Type::ConstChannel ConstChannel;
        typedef typename Type::Spectra Spectra;
        typedef typename Type::ConstSpectra ConstSpectra;

    protected:
        ExtendedTimeFrequency(TimeFrequencyType const&);

        template<typename Dummy = void, typename = IsNotRefType<Dummy>>
        ExtendedTimeFrequency(TimeFrequencyType&&);
        ~ExtendedTimeFrequency();

    public:
        Channel channel(std::size_t channel_number);
        ConstChannel channel(std::size_t channel_number) const;

        Spectra spectrum(std::size_t offset);
        ConstSpectra spectrum(std::size_t offset) const;

        std::size_t number_of_channels() const;
        std::size_t number_of_spectra() const;

    protected:
        Type& get();
        Type const& get() const;

    protected:
        TimeFrequencyType _tf;
};


} // namespace types
} // namespace astrotypes
} // namespace pss
#include "detail/ExtendedTimeFrequency.cpp"

#endif // PSS_ASTROTYPES_TYPES_EXTENDEDTIMEFREQUENCY_H
