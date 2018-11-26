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
#ifndef PSS_ASTROTYPES_UTILS_OPTIONAL_H
#define PSS_ASTROTYPES_UTILS_OPTIONAL_H


namespace pss {
namespace astrotypes {
namespace utils {

/**
 * @brief A simplified, faster, but less secure and less versatile version of boost::optional
 * @details only types with a default constructor are supported
 */

template<typename T>
class Optional
{
    public:
        Optional();
        Optional(T const&);
        Optional(T&&);
        ~Optional();

        inline operator bool&() { return _is_set; } 
        inline operator bool const&() const { return _is_set; } 
        inline operator T&() { return _val; }

        /**
         * @brief if the value has been explicitly set then return true
         */
        bool is_set() const;

        /**
         * @brief return the value
         * @details ensure to check is_set() to see if the value is valid
         */
        T const& operator*() const;
        T& operator*();

        /**
         * @brief compare two Optional wrapped parameters
         * @details note that they are considered equal if both parameters are unset
         *          If you wish to make multiple comparision it will be more
         *          efficient to check is_set() once and then call operator*
         */
        bool operator==(Optional const&) const;
        bool operator!=(Optional const&) const;
        bool operator==(T const&) const;
        bool operator!=(T const&) const;

        /**
         * @brief return to unset state
         */
        void reset();

    private:
        bool _is_set;
        T _val;
};

/**
 * @brief specialisation for bool
 * @details bool types will not be able to rely on conversion operator to test
 *          if set, but should use the is_set() method instead. 
 */
template<>
class Optional<bool>
{
    public:
        Optional();
        Optional(bool);
        ~Optional();

        inline operator bool&() { return _val; }
        inline operator bool const&() const { return _val; }

        /**
         * @brief if the value has been explicitly set then return true
         */
        bool is_set() const;

        /**
         * @brief compare two Optional wrapped parameters
         * @details note that they are considered equal if both parameters are unset
         *          these methods will always check is_set().
         *          If you wish to make multiple comparision it will be more
         *          efficient to check this once and then static_cast to bool
         */
        bool operator==(Optional const&) const;
        bool operator!=(Optional const&) const;
        bool operator==(bool) const;
        bool operator!=(bool) const;

        /**
         * @brief return to unset state
         */
        void reset();

    private:
        bool _is_set;
        bool _val;
};

} // namespace utils
} // namespace astrotypes
} // namespace pss
#include "detail/Optional.cpp"

#endif // PSS_ASTROTYPES_UTILS_OPTIONAL_H
