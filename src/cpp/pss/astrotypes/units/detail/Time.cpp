namespace pss {
namespace astrotypes {
namespace units {

template<class BoostUnit>
struct boost_unit_to_std_ratio {
    static_assert(std::is_same<BoostUnit, BoostUnit>::value, "Unkown conversion from boost unit to std::ratio");
};

template<>
struct boost_unit_to_std_ratio<Day> {
    typedef std::ratio<86400, 1> type;
};

template<>
struct boost_unit_to_std_ratio<Second> {
    typedef std::ratio<1,1> type;
};

template<>
struct boost_unit_to_std_ratio<MilliSecond> {
    typedef std::milli type;
};

template<>
struct boost_unit_to_std_ratio<MicroSecond> {
    typedef std::micro type;
};

template<>
struct boost_unit_to_std_ratio<NanoSecond> {
    typedef std::nano type;
};

template<>
struct boost_unit_to_std_ratio<PicoSecond> {
    typedef std::pico type;
};

template<typename BoostType, typename ChronoType>
struct has_equivalent_unit : public std::is_same<typename boost_unit_to_std_ratio<typename BoostType::unit_type>::type
                                               , typename ChronoType::period>::type
{
};

template<typename BoostType, typename ChronoType>
struct has_equivalent_rep : public std::is_same<typename BoostType::value_type, typename ChronoType::rep>::type
{
};

template<typename BoostType, typename ChronoType>
struct is_equivalent : public std::conditional<has_equivalent_unit<BoostType, ChronoType>::value
                                            && has_equivalent_rep<BoostType, ChronoType>::value
                                             , std::true_type, std::false_type
                                              >::type
{
};

} // namespace units
} // namespace astrotypes
} // namespace pss
