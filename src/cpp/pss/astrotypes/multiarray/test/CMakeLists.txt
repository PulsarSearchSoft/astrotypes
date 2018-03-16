include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(
   gtest_multiarray_src
   src/NumericalRepresentationTest.cpp
   src/StandardAllocatorTest.cpp
   src/PointerAllocatorTest.cpp
   src/SliceTest.cpp
#   src/MultiArrayTest.cpp
)

add_executable(gtest_multiarray ${gtest_multiarray_src} )
#target_link_libraries(gtest_multiarray ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(gtest_multiarray ${ASTROTYPES_TEST_UTILS} ${GTEST_LIBRARIES})
add_test(gtest_multiarray gtest_multiarray)
