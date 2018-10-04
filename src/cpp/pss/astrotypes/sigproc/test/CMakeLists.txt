include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(
   gtest_sigproc_src
   src/HeaderTest.cpp
   src/SigProcFormatTest.cpp
   src/FileReaderTest.cpp
)

# generate a header that hardcodes the location of the test files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/HardCodedDataPath.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/sigproc/HardCodedDataPath.h
              )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(gtest_sigproc ${gtest_sigproc_src})
#target_link_libraries(gtest_sigproc ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(gtest_sigproc ${ASTROTYPES_TEST_UTILS} ${GTEST_LIBRARIES})
add_test(gtest_sigproc gtest_sigproc)
