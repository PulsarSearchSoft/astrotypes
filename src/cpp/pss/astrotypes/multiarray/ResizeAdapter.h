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
#ifndef PSS_ASTROTYPES_SIGPROC_RESIZEADAPTER_H
#define PSS_ASTROTYPES_SIGPROC_RESIZEADAPTER_H
#include "DimensionSize.h"

namespace pss {
namespace astrotypes {

template<typename Dimension, typename... Dimensions>
class ResizeAdapter;

template<typename Stream, typename Dim, typename... Dims>
struct ResizeAdapterStreamBase;

template<typename Stream, typename TupleType>
struct ExtendedResizeAdapterStream : public ResizeAdapterStreamBase<Stream, TupleType>
{
        typedef ResizeAdapterStreamBase<Stream, TupleType> BaseT;

    public:
        ExtendedResizeAdapterStream(Stream& is, TupleType const& ra)
            : BaseT(is, _tuple)
            , _tuple(ra) {}

    private:
        TupleType _tuple;
};

template<typename Stream, typename Dim, typename... Dims>
struct ResizeAdapterStreamBase {
        typedef std::tuple<DimensionSize<Dim>, DimensionSize<Dims>...> TupleType;

        ResizeAdapterStreamBase(Stream& is, TupleType const& t) 
            : _stream(is)
            , _sizes(t) {}

        template<typename... ODims>
        ExtendedResizeAdapterStream<Stream, typename merge_tuples_type<std::tuple<DimensionSize<ODims>...>, TupleType>::type>
        operator>>(ResizeAdapter<ODims...>& adapter);

        template<typename T>
        typename std::enable_if<has_dimensions<typename std::decay<T>::type, Dim, Dims...>::value, Stream>::type&
        operator>>(T&);

    protected:
        Stream& _stream;
        TupleType const& _sizes;
};

template<typename Stream, typename Dim, typename... Dims>
struct ResizeAdapterStreamBase<Stream, std::tuple<DimensionSize<Dim>, DimensionSize<Dims>...>>
    : public ResizeAdapterStreamBase<Stream, Dim, Dims...>
{
    using ResizeAdapterStreamBase<Stream, Dim, Dims...>::ResizeAdapterStreamBase;
};

/**
 * @brief
 *      Allows you to define how a MultiArray object should be resized
 *      in a streaming context
 * @details
 * @code
 * MyStream my_stream;
 * MultiArrayDataType data;
 *
 * my_stream >> ResizeAdapter<DimensionA, DimensionB>() >> data;
 * @endcode
 * The stream object must provide the templated function:
 * @code
 * template<typename Dimension>
 * DimensionSize<Dimension< dimension() const;
 * @endcode
 * and should return the new dimension size.
 *
 * ResizeAdapters can be chained, with the latest one taking priority
 * @code
 * // e,g, ensure that the DimesnonB is always 100
 * my_stream >> ResizeAdapter<DimensionA, DimensionB>() >> ResizeAdapter<DimensionB>(DimensionSize<DimensionB>(100) >> data;
 *
 * // equally valid
 * my_stream >> ResizeAdapter<DimensionA>() >> ResizeAdapter<DimensionB>(DimensionSize<DimensionB>(100) >> data;
 * @code
 */

template<typename Dimension, typename... Dimensions>
class ResizeAdapter
{
        typedef std::tuple<DimensionSize<Dimension>, DimensionSize<Dimensions>...> TupleType;

    protected:
        template<typename StreamType>
        struct Stream : public ResizeAdapterStreamBase<StreamType, Dimension, Dimensions...> {
                typedef ResizeAdapterStreamBase<StreamType, Dimension, Dimensions...> BaseT;

            public:
                Stream(StreamType& is, ResizeAdapter& ra);
        };

    public:
        ResizeAdapter();

        template<typename Dim, typename... Dims>
        ResizeAdapter(DimensionSize<Dim>, DimensionSize<Dims>...);

        ~ResizeAdapter() {};

    private:
        template<typename StreamT>
        friend Stream<StreamT> operator>>(StreamT& is, ResizeAdapter ra) {
            return Stream<StreamT>(is, ra);
        }

        template<typename Stream, typename T, typename... Ts>
        friend struct ResizeAdapterStreamBase;

    private:
        TupleType _sizes;
};


} // namespace astrotypes
} // namespace pss
#include "detail/ResizeAdapter.cpp"

#endif // PSS_ASTROTYPES_SIGPROC_RESIZEADAPTER_H
