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

#include <algorithm>

namespace pss {
namespace astrotypes {
namespace detail {

template<typename T, typename Alloc>
DataBufferImplAllocator<T, Alloc>::AllocatedMemory::AllocatedMemory(Alloc& alloc, std::size_t capacity)
    : _alloc(&alloc)
    , _m_start(alloc.allocate(capacity))
    , _m_end_of_storage(_m_start + capacity)
{
}

template<typename T, typename Alloc>
DataBufferImplAllocator<T, Alloc>::AllocatedMemory::AllocatedMemory(Alloc& alloc)
    : _alloc(&alloc)
    , _m_start(nullptr)
    , _m_end_of_storage(nullptr)
{
}

template<typename T, typename Alloc>
DataBufferImplAllocator<T, Alloc>::AllocatedMemory::AllocatedMemory(AllocatedMemory&& mem)
    : _alloc(mem._alloc)
    , _m_start(mem._m_start)
    , _m_end_of_storage(mem._m_end_of_storage)
{
    mem._m_start = nullptr;
}

template<typename T, typename Alloc>
DataBufferImplAllocator<T, Alloc>::AllocatedMemory::~AllocatedMemory()
{
    this->deallocate();
}

template<typename T, typename Alloc>
typename DataBufferImplAllocator<T, Alloc>::AllocatedMemory& DataBufferImplAllocator<T, Alloc>::AllocatedMemory::operator=(AllocatedMemory&& o)
{
    this->deallocate();

    _m_start=o._m_start;
    _m_end_of_storage=o._m_end_of_storage;

    o._m_start = nullptr;
    return *this;
}

template<typename T, typename Alloc>
void DataBufferImplAllocator<T, Alloc>::AllocatedMemory::deallocate()
{
    if(_m_start)
        _alloc->deallocate(_m_start, _m_end_of_storage - _m_start);
}

template<typename T, typename Alloc>
std::size_t DataBufferImplAllocator<T, Alloc>::AllocatedMemory::capacity() const
{
    return _m_end_of_storage - _m_start;
}

// easily initialise allocator ond its state info
template<typename T, typename Alloc>
DataBufferImplAllocator<T, Alloc>::DataBufferImplAllocator(Alloc const& alloc)
    : Alloc(alloc)
{}

template<typename T, typename Alloc>
DataBufferImplAllocator<T, Alloc>::~DataBufferImplAllocator()
{
}


template<typename T, typename Alloc>
typename DataBufferImplAllocator<T, Alloc>::AllocatedMemory DataBufferImplAllocator<T, Alloc>::allocate(std::size_t s)
{
    return AllocatedMemory(*this, s);
}

// ----------------- DataBufferImpl -------------------------------------------
template<typename T, typename Alloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(std::size_t size, const Alloc& a)
    : _allocator(a)
    , _m_alloc(_allocator.allocate(size))
    , _m_finish(_m_alloc.end())
{
}

template<typename T, typename Alloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(std::size_t size, T const& val, const Alloc& a)
    : _allocator(a)
    , _m_alloc(_allocator.allocate(size))
    , _m_finish(_m_alloc.end())
{
    std::fill(begin(), end(), val);
}

template<typename T, typename Alloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(DataBufferImpl const& vec)
    : _allocator(vec._allocator)
    , _m_alloc(_allocator.allocate(vec.size()))
    , _m_finish(_m_alloc.end())
{
    std::copy(vec.begin(), vec.end(), this->begin());
}

template<typename T, typename Alloc>
template<typename OtherAlloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(DataBufferImpl<T, OtherAlloc, false> const& vec)
    : _allocator(Alloc())
    , _m_alloc(_allocator.allocate(vec.size()))
    , _m_finish(_m_alloc.end())
{
    std::copy(vec.begin(), vec.end(), this->begin());
}

template<typename T, typename Alloc>
template<typename OtherAlloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(std::vector<T, OtherAlloc> const& vec)
    : _allocator(Alloc())
    , _m_alloc(_allocator.allocate(vec.size()))
    , _m_finish(_m_alloc.end())
{
    std::copy(vec.begin(), vec.end(), this->begin());
}

template<typename T, typename Alloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(DataBufferImpl&& vec)
    : _allocator(vec._allocator)
    , _m_alloc(std::move(vec._m_alloc))
    , _m_finish(vec._m_finish)
{
    vec._m_finish = nullptr;
}

template<typename T, typename Alloc>
DataBufferImpl<T, Alloc, false>::DataBufferImpl(Alloc const& a)
    : _allocator(a)
    , _m_alloc(_allocator.allocate(0))
    , _m_finish(_m_alloc.end())
{
}

template<typename T, typename Alloc>
DataBufferImpl<T, Alloc, false>::~DataBufferImpl()
{
}

// assignment
template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::operator=(DataBufferImpl const& other)
{
    this->resize(other.size());
    std::copy(other.begin(), other.end(), this->begin());
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::operator=(DataBufferImpl&& other)
{
    _m_alloc = std::move(other._m_alloc); // must ensure any existing _m_alloc is out of scope before
                                          // replacing the allocator it is associated with
    _allocator = std::move(other._allocator);
    _m_finish = other._m_finish;
    other._m_finish = nullptr;
}

template<typename T, typename Alloc>
template<typename OtherAlloc>
void DataBufferImpl<T, Alloc, false>::operator=(DataBufferImpl<T, OtherAlloc, false> const& other)
{
    this->resize(other.size());
    std::copy(other.begin(), other.end(), this->begin());
}

template<typename T, typename Alloc>
T& DataBufferImpl<T, Alloc, false>::operator[](std::size_t n)
{
    return *(this->_m_alloc.start() + n);
}

template<typename T, typename Alloc>
T const& DataBufferImpl<T, Alloc, false>::operator[](std::size_t n) const
{
    return *(this->_m_alloc.start() + n);
}

template<typename T, typename Alloc>
T* DataBufferImpl<T, Alloc, false>::begin()
{
    return this->_m_alloc.start();
}

template<typename T, typename Alloc>
T* DataBufferImpl<T, Alloc, false>::end()
{
    return this->_m_finish;
}

template<typename T, typename Alloc>
const T* DataBufferImpl<T, Alloc, false>::begin() const
{
    return this->_m_alloc.start();
}

template<typename T, typename Alloc>
const T* DataBufferImpl<T, Alloc, false>::end() const
{
    return this->_m_finish;
}

template<typename T, typename Alloc>
const T* DataBufferImpl<T, Alloc, false>::cbegin() const
{
    return this->_m_alloc.start();
}

template<typename T, typename Alloc>
const T* DataBufferImpl<T, Alloc, false>::cend() const
{
    return this->_m_finish;
}

template<typename T, typename Alloc>
std::size_t DataBufferImpl<T, Alloc, false>::size() const
{
    return this->_m_finish - this->_m_alloc.start();
}

template<typename T, typename Alloc>
std::size_t DataBufferImpl<T, Alloc, false>::max_size()
{
    return this->_allocator.max_size();
}

template<typename T, typename Alloc>
std::size_t DataBufferImpl<T, Alloc, false>::capacity() const
{
    return this->_m_alloc.capacity();
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::resize(std::size_t size, T const& val)
{
    std::size_t last_index = this->size() - 1;
    this->resize(size);
    std::fill(this->begin() + last_index, end(), val);
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::resize(std::size_t size)
{
    if(size < this->size())
        this->_m_finish = this->_m_alloc.start() + size;
    else if( size > this->size())
    {
        this->m_extend(size);
        _m_finish = this->_m_alloc.end();
    }
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::reserve(std::size_t size)
{
    if(size > this->max_size())
        throw std::bad_alloc();
    if(size > this->capacity())
    {
        std::size_t old_size=this->size();
        this->m_extend(size);
        _m_finish = this->begin() + old_size;
    }
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::push_back(const T& x)
{
    if (this->_m_finish == this->_m_alloc.end())
    {
        m_extend();
    }
    (*this->_m_finish) = x;
    ++this->_m_finish;
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::emplace_back(T&& x)
{
    if (this->_m_finish == this->_m_alloc.end())
    {
        m_extend();
    }
    (*this->_m_finish) = std::move(x);
    ++this->_m_finish;
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::pop_back()
{
    --this->_m_finish;
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::clear()
{
    this->_m_finish = this->_m_alloc.start();
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::m_extend(std::size_t len)
{
    AllocatedMemory new_mem = _allocator.allocate(len);
    T* new_start=new_mem.start();
    std::copy(cbegin(), cend(), new_start);
    this->_m_alloc = std::move(new_mem);
}

template<typename T, typename Alloc>
void DataBufferImpl<T, Alloc, false>::m_extend()
{
    std::size_t const old_size = size();
    if(old_size==max_size()) {
        throw std::bad_alloc();
    }
    std::size_t len = old_size != 0 ? 2 * old_size : 2;
    if (len < old_size) // overflow?
        len = this->max_size();
    m_extend(len);
    this->_m_finish = this->begin() + old_size;
}

} // namespace detail

// -------------- Top level ----------------

template<typename T, typename Alloc>
template<typename... Args>
DataBuffer<T, Alloc>::DataBuffer(Args&&... args)
    : BaseT(std::forward<Args>(args)...)
{
}

template<typename T, typename Alloc>
DataBuffer<T, Alloc>::~DataBuffer()
{
}

template<typename T, typename Alloc>
DataBuffer<T, Alloc>& DataBuffer<T, Alloc>::operator=(DataBuffer const& other)
{
    BaseT::operator=(other);
    return *this;
}

template<typename T, typename Alloc>
DataBuffer<T, Alloc>& DataBuffer<T, Alloc>::operator=(DataBuffer&& other)
{
    BaseT::operator=(std::move(other));
    return *this;
}

template<typename T, typename Alloc>
template<typename OtherAlloc>
DataBuffer<T, Alloc>& DataBuffer<T, Alloc>::operator=(DataBuffer<T, OtherAlloc> const& other)
{
    BaseT::operator=(other);
    return *this;
}

} // namespace astrotypes
} // namespace pss
