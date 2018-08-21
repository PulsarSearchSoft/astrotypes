include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(
   gtest_sigproc_src
   src/HeaderTest.cpp
   src/SigProcFormatTest.cpp
)

add_executable(gtest_sigproc ${gtest_sigproc_src} )
#target_link_libraries(gtest_sigproc ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(gtest_sigproc ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_sigproc gtest_sigproc)
