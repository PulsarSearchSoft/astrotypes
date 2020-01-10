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
#ifndef PSS_ASTROTYPES_UTILS_TYPETRAITS_H
#define PSS_ASTROTYPES_UTILS_TYPETRAITS_H


namespace pss {
namespace astrotypes {

/**
 * @class is_pointer_wrapper
 * @brief return true is its a std::shared_ptr or std::unique_ptr
 *        false otherwise
 *        @param type is typedefed to either the full object or the wrapped type
 *        @method extract return a ref to the value (by deref if necessary)
 */
template<typename T>
struct is_pointer_wrapper {
    static constexpr bool value=false;
    typedef T type;

    template<typename TT>
    inline static TT& extract(TT& t) {
        return t;
    }
};

template<typename T>
struct is_pointer_wrapper<std::shared_ptr<T>> {
    static constexpr bool value=true;
    typedef T type;

    inline static T& extract(std::shared_ptr<T> const& t) {
        return *t;
    }
};

template<typename T>
struct is_pointer_wrapper<std::unique_ptr<T>> {
    static constexpr bool value=true;
    typedef T type;

    inline static T& extract(std::unique_ptr<T> const& t) {
        return *t;
    }
};

} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_UTILS_TYPETRAITS_H
