include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(
   gtest_multiarray_src
   src/NumericalRepresentationTest.cpp
}

add_executable(gtest_mutlivector ${gtest_utils_src} )
target_link_libraries(gtest_utils ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_multiarray gtest_multiarray)
