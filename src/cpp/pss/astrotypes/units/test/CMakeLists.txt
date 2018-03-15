include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

#set(
#)

# add_executable(gtest_units ${gtest_units_src} )
#target_link_libraries(gtest_units ${ASTROTYPES_TEST_UTILS} ${ASTROTYPES_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(gtest_units ${ASTROTYPES_TEST_UTILS} ${GTEST_LIBRARIES})
add_test(gtest_units gtest_units)
