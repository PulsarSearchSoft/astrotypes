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
#ifndef PSS_ASTROTYPES_MULTIARRAY_EXTENDEDMULTIARRAY_H
#define PSS_ASTROTYPES_MULTIARRAY_EXTENDEDMULTIARRAY_H

#include "pss/astrotypes/utils/TypeTraits.h"
#include <type_traits>

namespace pss {
namespace astrotypes {
namespace multiarray {

/**
 * @brief provides the MultiArray Interface to a wrapped object to allow for east object composition
 * @details
 */

template<typename DerivedType, typename MultiArrayType, typename Enable=void>
class ExtendedMultiArray
{
    protected:
        typedef is_pointer_wrapper<typename std::remove_reference<MultiArrayType>::type> IsWrapped;
        typedef typename IsWrapped::type Type; // unwrapped from any pointer wrapping

    public:
        typedef typename Type::iterator iterator;
        typedef typename Type::const_iterator const_iterator;
        typedef typename Type::value_type value_type;
        typedef typename Type::DimensionTuple DimensionTuple;

        typedef typename Type::SliceType SliceType;
        typedef typename Type::ConstSliceType ConstSliceType;
        typedef typename Type::ReducedDimensionSliceType ReducedDimensionSliceType;
        typedef typename Type::ConstReducedDimensionSliceType ConstReducedDimensionSliceType;

        typedef typename Type::reference_type reference_type;
        typedef typename Type::const_reference_type const_reference_type;

    public:
        ExtendedMultiArray(MultiArrayType const&);
        ~ExtendedMultiArray();

        static constexpr std::size_t rank = Type::rank;

        /**
         * @brief return true if the MultiArray  has dimesion D
         */
        template<typename D>
        static constexpr bool has_dimension() { return MultiArrayType::template has_dimension<D>(); }

        inline const_iterator begin() const;
        inline const_iterator end() const;
        inline const_iterator cbegin() const;
        inline const_iterator cend() const;

        template<typename Dimension>
        ConstReducedDimensionSliceType operator[](DimensionIndex<Dimension> index) const;

        template<typename Dim>
        DimensionSize<Dim> dimension() const;


        template<typename... Dims>
        ConstSliceType slice(DimensionSpan<Dims> const&...) const;

        std::size_t data_size() const;

        bool operator==(Type const&) const;

    protected:
        Type& get();
        Type const& get() const;

    protected:
        MultiArrayType _ma;
};

template<typename T>
using IsConstMultiArray = std::is_const<typename is_pointer_wrapper<typename std::remove_reference<T>::type>::type>;

// specialisation for non-const methods
template<typename DerivedType, typename MultiArrayType>
class ExtendedMultiArray<DerivedType
                       , MultiArrayType
                       , typename std::enable_if<IsConstMultiArray<MultiArrayType>::value, void*>::type>
    : public ExtendedMultiArray<DerivedType, MultiArrayType, void>
{
        using ExtendedMultiArray<DerivedType, MultiArrayType, void>::ExtendedMultiArray;
        typedef ExtendedMultiArray<DerivedType, MultiArrayType, void> BaseT;

   public:
        typedef typename BaseT::iterator iterator;
        typedef typename BaseT::const_iterator const_iterator;
        typedef typename BaseT::value_type value_type;
        typedef typename BaseT::DimensionTuple DimensionTuple;

        typedef typename BaseT::SliceType SliceType;
        typedef typename BaseT::ConstSliceType ConstSliceType;
        typedef typename BaseT::ReducedDimensionSliceType ReducedDimensionSliceType;
        typedef typename BaseT::ConstReducedDimensionSliceType ConstReducedDimensionSliceType;

        typedef typename BaseT::reference_type reference_type;
        typedef typename BaseT::const_reference_type const_reference_type;

    public:
        inline iterator begin();
        inline iterator end();

        template<typename Dimension>
        ReducedDimensionSliceType operator[](DimensionIndex<Dimension> index);

        template<typename... Dims>
        SliceType slice(DimensionSpan<Dims> const&...);
};

} // namespace multiarray
} // namespace astrotypes
} // namespace pss
#include "detail/ExtendedMultiArray.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_EXTENDEDMULTIARRAY_H
