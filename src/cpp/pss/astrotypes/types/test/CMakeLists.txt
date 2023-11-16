include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_types_src
    src/PhaseFrequencyArrayTest.cpp
    src/TimeFrequencyTest.cpp
    src/ExtendedTimeFrequencyTest.cpp
)

add_executable(gtest_astrotypes_types ${gtest_types_src})
#target_link_libraries(gtest_types ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(gtest_astrotypes_types ${ASTROTYPES_TEST_UTILS} ${GTEST_LIBRARIES})
add_test(gtest_astrotypes_types gtest_astrotypes_types)
