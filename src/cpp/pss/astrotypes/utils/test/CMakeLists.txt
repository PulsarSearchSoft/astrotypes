include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(
    gtest_utils_src
    src/OptionalTest.cpp
    src/ModuloOneTest.cpp
)

add_executable(gtest_astrotypes_utils ${gtest_utils_src})
#target_link_libraries(gtest_utils ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(gtest_astrotypes_utils ${ASTROTYPES_TEST_UTILS} ${GTEST_LIBRARIES})
add_test(gtest_astrotypes_utils gtest_astrotypes_utils)
