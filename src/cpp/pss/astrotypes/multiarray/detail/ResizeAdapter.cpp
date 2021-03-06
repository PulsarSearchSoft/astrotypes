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

namespace pss {
namespace astrotypes {

namespace {
// unpack tuple in to arguments
template <typename TupleType, std::size_t Index=std::tuple_size<TupleType>::value>
struct ResizeHelper
{
    template<typename DataType, typename... Ts>
    static inline void exec(DataType& data, TupleType const& tuple, Ts&&... args)
    {
        ResizeHelper<TupleType, Index-1>::exec(data, tuple, std::forward<Ts>(args)..., std::get<Index-1>(tuple));
    }

    template<typename DataType>
    static inline void exec(DataType& data, TupleType const& tuple)
    {
        ResizeHelper<TupleType, Index-1>::exec(data, tuple, std::get<Index-1>(tuple));
    }
};

template<typename TupleType>
struct ResizeHelper<TupleType, 0>
{
    template<typename DataType, typename... Ts>
    static inline void exec(DataType& data, TupleType const&, Ts&&... args) {
        data.resize(std::forward<Ts>(args)...);
    }
};

} // namespace

template<typename Dimension, typename... Dimensions>
ResizeAdapter<Dimension, Dimensions...>::ResizeAdapter()
{
}

template<typename Dimension, typename... Dimensions>
template<typename Dim, typename... Dims>
ResizeAdapter<Dimension, Dimensions...>::ResizeAdapter(DimensionSize<Dim> dim, DimensionSize<Dims>... dims)
{
    tuple_insert_type(_sizes, dim, dims...);
}

// helper funtion for Stream construction
namespace {
template<typename... Dimensions>
struct DimensionCopy;

template<typename Dimension>
struct DimensionCopy<Dimension>
{
    template<typename StreamType, typename TupleType>
    inline static void exec(StreamType const& stream, TupleType& tuple)
    {
        tuple_insert_type(tuple, stream.template dimension<Dimension>());
    }
};

template<typename Dimension, typename... Dimensions>
struct DimensionCopy<Dimension, Dimensions...>
{
    template<typename StreamType, typename TupleType>
    inline static void exec(StreamType const& stream, TupleType& tuple)
    {
        tuple_insert_type(tuple, stream.template dimension<Dimension>());
        DimensionCopy<Dimensions...>::exec(stream, tuple);
    }
};

} // namespace
template<typename Dimension, typename... Dimensions>
template<typename StreamType>
ResizeAdapter<Dimension, Dimensions...>::Stream<StreamType>::Stream(StreamType& is, ResizeAdapter& ra)
    : BaseT(is, ra._sizes)
{
    // transfer dimension info fromt the stream into the _sizes tuple
    DimensionCopy<Dimension, Dimensions...>::exec(is, ra._sizes);
}

template<typename Stream, typename Dim, typename... Dims>
template<typename T>
typename std::enable_if<has_dimensions<typename std::decay<T>::type, Dim, Dims...>::value, Stream>::type&
ResizeAdapterStreamBase<Stream, Dim, Dims...>::operator>>(T& tf)
{
    ResizeHelper<typename std::remove_reference<decltype(this->_sizes)>::type>::exec(tf, this->_sizes);
    this->_stream >> tf;
    return _stream;
}

template<typename Stream, typename Dim, typename... Dims>
template<typename... ODims>
ExtendedResizeAdapterStream<Stream, typename merge_tuples_type<std::tuple<DimensionSize<ODims>...>, typename ResizeAdapterStreamBase<Stream, Dim, Dims...>::TupleType>::type>
ResizeAdapterStreamBase<Stream, Dim, Dims...>::operator>>(ResizeAdapter<ODims...>& adapter)
{
    return ExtendedResizeAdapterStream<Stream, typename merge_tuples_type<std::tuple<DimensionSize<ODims>...>, TupleType>::type>(this->_stream, merge_tuples(adapter._sizes, this->_sizes));
}

} // namespace astrotypes
} // namespace pss
