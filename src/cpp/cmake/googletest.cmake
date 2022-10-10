# googletest build - must be built with the same compiler flags

if(NOT GTEST_GUARD_VAR)
    set(GTEST_GUARD_VAR TRUE)
else()
    return()
endif()

if(NOT GTEST_INCLUDE_DIR)
    if(APPLE)
        add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=1)
    else()
        add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=0)
    endif()

    add_subdirectory("thirdparty/googletest")
    set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/include)
    set(GMOCK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googlemock/include)
    set(GTEST_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/googletest)
    set(GTEST_LIBRARIES gtest_main gtest)
endif()
