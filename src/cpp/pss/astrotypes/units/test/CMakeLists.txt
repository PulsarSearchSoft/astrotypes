include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_units_src
    src/UtilitiesTest.cpp
    src/TimeTest.cpp
    src/JulianClockTest.cpp
    src/ModifiedJulianClockTest.cpp
)

add_executable(gtest_units ${gtest_units_src} )
target_link_libraries(gtest_units ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_units gtest_units)
