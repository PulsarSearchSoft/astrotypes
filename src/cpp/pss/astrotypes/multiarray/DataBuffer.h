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
#ifndef PSS_ASTROTYPES_MULTIARRAY_DATABUFFER_H
#define PSS_ASTROTYPES_MULTIARRAY_DATABUFFER_H
#include <vector>
#include <type_traits>

namespace pss {
namespace astrotypes {

namespace detail {
    template<typename T, typename Alloc, bool use_std_vec=!std::is_trivial<T>::value>
    class DataBufferImpl {
    };

    template<typename T, typename Alloc>
    class DataBufferImpl<T, Alloc, true> : public std::vector<T, Alloc>
    {
        public:
            using std::vector<T,Alloc>::vector;
    };

    // easily initialise allocator ond its state info
    template<typename T, typename Alloc>
    class DataBufferImplAllocator : public Alloc
    {
        public:
            struct AllocatedMemory
            {
                AllocatedMemory(Alloc& alloc, std::size_t capacity);
                AllocatedMemory(Alloc& alloc);
                AllocatedMemory(AllocatedMemory&& mem);
                ~AllocatedMemory();
                AllocatedMemory& operator=(AllocatedMemory&&);

                std::size_t capacity() const;
                T* start() const { return _m_start; }
                T* end() const   { return _m_end_of_storage; }

                private:
                    void deallocate();

                private:
                    Alloc*       _alloc;
                    T*           _m_start;
                    T*           _m_end_of_storage;

            };

        public:
            DataBufferImplAllocator(Alloc const& alloc);
            ~DataBufferImplAllocator();

            AllocatedMemory allocate(std::size_t size);
    };

    template<typename T, typename Alloc>
    class DataBufferImpl<T, Alloc, false>
    {
        private:
            typedef DataBufferImplAllocator<T, Alloc>  Allocator;
            typedef typename Allocator::AllocatedMemory AllocatedMemory;

        public:
            typedef T* iterator;
            typedef T const* const_iterator;
            typedef T value_type;
            typedef T& reference;
            typedef const T& const_reference;

        public:
            // constructors
            DataBufferImpl(std::size_t n, const Alloc& = Alloc());
            DataBufferImpl(std::size_t n, T const& val, const Alloc& = Alloc());
            DataBufferImpl(const Alloc& = Alloc());

            DataBufferImpl(DataBufferImpl const&);
            template<typename OtherAlloc>
            DataBufferImpl(DataBufferImpl<T, OtherAlloc, false> const&);

            template<typename OtherAlloc>
            DataBufferImpl(std::vector<T, OtherAlloc> const&);

            DataBufferImpl(DataBufferImpl&&);
            ~DataBufferImpl();

            // assignment
            void operator=(DataBufferImpl const&);
            void operator=(DataBufferImpl&&);
            template<typename OtherAlloc>
            void operator=(DataBufferImpl<T, OtherAlloc, false> const&);

            // sizing
            std::size_t capacity() const;
            std::size_t size() const;

            std::size_t max_size();

            void resize(std::size_t size);
            void resize(std::size_t size, T const& value);
            void reserve(std::size_t s);

            // data access
            T& operator[](std::size_t n);
            T const& operator[](std::size_t n) const;

            T* begin();
            T* end();
            const T* begin() const;
            const T* end() const;
            const T* cbegin() const;
            const T* cend() const;

            T& front() { return *begin(); }
            T const& front() const { return *begin(); }
            T& back() { return *(end() - 1); }
            T const& back() const { return *(end() - 1); }

            void push_back(const T& x);
            void pop_back();
            void emplace_back(T&& x);
            void clear();

        protected:
            void m_extend(std::size_t len);
            void m_extend();

        private:
            Allocator        _allocator;
            AllocatedMemory  _m_alloc;
            T*               _m_finish;
    };


} // namespace detail

/**
 * @brief
 *      a std::vector that does not initialise its members when they are plain old data (i.e. float, int,...)
 *
 * @details
 *      If they are of a type that needs initialisation then the DataBuffer is a conventional std::vector
 *      Memory is guaranteed to be contiguous
 */

template<typename T, typename Alloc=std::allocator<T>>
class DataBuffer : public detail::DataBufferImpl<T, Alloc>
{
        typedef detail::DataBufferImpl<T, Alloc> BaseT;

    public:
        typedef typename BaseT::iterator iterator;
        typedef typename BaseT::const_iterator const_iterator;
        typedef typename BaseT::value_type value_type;
        typedef typename BaseT::reference reference;

        DataBuffer& operator=(DataBuffer const&);
        DataBuffer& operator=(DataBuffer&&);
        template<typename OtherAlloc>
        DataBuffer& operator=(DataBuffer<T, OtherAlloc> const&);

    public:
        template<typename... Args>
        DataBuffer(Args&&...);
        ~DataBuffer();
};


} // namespace astrotypes
} // namespace pss
#include "detail/DataBuffer.cpp"

#endif // PSS_ASTROTYPES_MULTIARRAY_DATABUFFER_H
