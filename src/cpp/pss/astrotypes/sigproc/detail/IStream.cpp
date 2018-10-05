namespace pss {
namespace astrotypes {
namespace sigproc {

template<Header::DataType HeaderDataType>
typename AdaptedIStream<HeaderDataType>::AdapterType AdaptedIStream<HeaderDataType>::_adapter;

template<Header::DataType HeaderDataType>
template<typename Stream, typename DataType>
void AdaptedIStream<HeaderDataType>::read(Stream& stream, DataType& data)
{
    stream >> _adapter >> data;
}

template<typename HeaderT>
inline HeaderT const& IStream<HeaderT>::header() const
{
    return _header;
}

template<typename HeaderT>
template<typename Stream>
void IStream<HeaderT>::new_header(Stream& stream)
{
    stream >> _header;
}

template<typename HeaderT>
template<typename Stream, typename DataType>
void IStream<HeaderT>::read(Stream& stream, DataType& data)
{
    switch(_header.data_type())
    {
        case HeaderT::DataType::FilterBank:
            AdaptedIStream<HeaderT::DataType::FilterBank>::read(stream, data);
            break;
        case HeaderT::DataType::TimeSeries:
            AdaptedIStream<HeaderT::DataType::TimeSeries>::read(stream, data);
            break;
        default:
            throw std::runtime_error("unkonwn data type setting");
    }
}

} // namespace sigproc
} // namespace astrotypes
} // namespace pss
