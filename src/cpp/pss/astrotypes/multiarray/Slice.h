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
#ifndef PSS_ASTROTYPES_MULTIARRAY_SLICE_H
#define PSS_ASTROTYPES_MULTIARRAY_SLICE_H
#include "DimensionSpan.h"
#include "SliceIterator.h"
#include <utility>

namespace pss {
namespace astrotypes {

/**
 * @brief
 *      Representation of a Slice through a Data Array
 * @details
 */

template<bool is_const, typename ParentT, typename Dimension, typename... Dimensions>
class Slice : private Slice<is_const, ParentT, Dimensions...>
{
        typedef Slice<is_const, ParentT, Dimensions...> BaseT;
        typedef Slice<is_const, ParentT, Dimension, Dimensions...> SelfType;
        typedef typename ParentT::value_type value_type;

        typedef typename ParentT::const_iterator parent_const_iterator;
        typedef typename std::conditional<is_const, parent_const_iterator, typename ParentT::iterator>::type parent_iterator;

    public:
        typedef typename std::conditional<is_const, const ParentT, ParentT>::type Parent;
        typedef SliceIterator<SelfType, is_const> iterator;
        typedef SliceIterator<SelfType, true> const_iterator;

    public:
        Slice( Parent& parent
             , DimensionSpan<Dimension> const&
             , DimensionSpan<Dimensions> const&...
        );

        static constexpr std::size_t rank = 1 + sizeof...(Dimensions);

        /**
         * @ brief return the size of the slice in the specified dimension
         * @code
         *      Slice<DimesionA, DimensionB> slice(...some initialisers...);
         *      DimensionSize<DimensionA> size_of_A = slice.size<DimensionA>();
         *      DimensionSize<DimensionB> size_of_B = slice.size<DimensionB>();
         *      // note you can rmeomve the dimension tagging on the DimesionSize
         *      // objects for compatibilty with less strongly types libs
         *      // with a static_cast
         *      std::size_t size_A_as_size_t = static_cast<std::size_t>(size_of_A);
         * @endcode
         * @details
         *      If you intend to inherit from this class you may need to call this method with the 
         *      template keyword.
         *      e,g, 
         *      @code
         *      slice.template size<DimensionA>();
         *      @endcode
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type 
        size() const;

        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type 
        size() const;

        /**
         * @brief Take a slice in the specified dimension one channel thick
         */
        Slice<is_const, ParentT, Dimensions...> operator[](DimensionIndex<Dimension>) const;
        Slice<is_const, ParentT, Dimensions...> operator[](std::size_t) const; // TODO remove

        /**
         * @brief Take a slice from this slice
         */
        Slice<is_const, ParentT, Dimension, Dimensions...> slice(DimensionSpan<Dimension> const& span) const;

        /**
         * @brief iterator pointing to the first element in the slice
         */
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

    protected:
        template<bool, typename P, typename D, typename... Ds> friend class Slice;

        template<typename IteratorT> bool increment_it(IteratorT& current, IteratorT& end, IteratorT& max_end) const;

        // return the size of memory occupied by the lowest dimension
        std::size_t contiguous_span() const;

        // reeturn the span of all lower dimensions than this one (i.e an index of +1 in this dimension)
        std::size_t base_span() const;

        // ptr to the start of the block
        parent_iterator const& base_ptr() const;
        parent_iterator& base_ptr();

        void offset(parent_iterator const&); // init the offset relative to the top parent

        // init for inheriting classes only
        Slice( DimensionSpan<Dimension> const&
             , DimensionSpan<Dimensions> const&...
             , Parent const& parent
        );

        // increment pointer by a n * span length
        SelfType& operator+=(std::size_t n);

        // increment pointer by a n * base span length
        SelfType& operator+=(DimensionSize<Dimension> n);

    private:
        friend typename iterator::BaseT;
        friend typename const_iterator::BaseT;

    private:
        DimensionSpan<Dimension> _span;
        DimensionSize<Dimension> _base_span;
        parent_iterator _ptr;  // start of block
};

// specialisation for 0 dimensional slice
// which should return a single element
template<bool is_const, typename ParentT, typename Dimension>
class Slice<is_const, ParentT, Dimension>
{
        typedef Slice<is_const, ParentT, Dimension> SelfType;
        typedef typename ParentT::const_iterator parent_const_iterator;
        typedef typename std::conditional<is_const, parent_const_iterator, typename ParentT::iterator>::type parent_iterator;
        typedef typename std::iterator_traits<parent_iterator>::reference reference_type;

    public:
        typedef typename std::conditional<is_const, const ParentT, ParentT>::type Parent;
        typedef SliceIterator<SelfType, is_const> iterator;
        typedef SliceIterator<SelfType, true> const_iterator;

    public:
        Slice(Parent& parent, DimensionSpan<Dimension> const&);
        ~Slice();

        static constexpr std::size_t rank = 1;

        /**
         * @ brief return the size of the slice in the specified dimension
         */
        template<typename Dim>
        typename std::enable_if<std::is_same<Dim, Dimension>::value, DimensionSize<Dimension>>::type 
        size() const;

        /**
         * @ brief return the size of the slice in the specified dimension (will always be zero)
         */
        template<typename Dim>
        typename std::enable_if<!std::is_same<Dim, Dimension>::value, DimensionSize<Dim>>::type 
        size() const;

        /**
         * @brief return the value at the position specified in this Dimension
         */
        reference_type operator[](std::size_t index) const;
        reference_type operator[](DimensionIndex<Dimension> const& index) const;

        /**
         * @brief iterator pointing to the first element in the slice
         */
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        /**
         * @brief iterator pointing to just after the last element
         */
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;


    protected:
        template<bool, typename P, typename D, typename... Ds> friend class Slice;

        template<typename IteratorT> static bool increment_it(IteratorT& current, IteratorT& end, IteratorT& max_end);

        // return the size of memory occupied by the lowest dimension
        std::size_t contiguous_span() const;

        // same as size() - to support base_span calls from higher dimensions
        std::size_t base_span() const;

        // ptr to the start of the block
        parent_iterator const& base_ptr() const;
        parent_iterator& base_ptr();

        void offset(parent_iterator const&); // init the offset relative to the top parent
        Slice(DimensionSpan<Dimension> const&, Parent const&);
        SelfType& operator+=(DimensionSize<Dimension> const&);

    private:
        friend typename iterator::BaseT;
        friend typename const_iterator::BaseT;

    private:
        DimensionSpan<Dimension> _span;
        DimensionSize<Dimension> _base_span;
        parent_iterator _ptr;   // start, to extent _ptr + _span
};

} // namespace astrotypes
} // namespace pss

#include "detail/Slice.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_SLICE_H
