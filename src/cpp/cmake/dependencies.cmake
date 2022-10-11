if(NOT PSS_ASTROTYPES_DEPENDENCIES_GUARD_VAR)
    set(PSS_ASTROTYPES_DEPENDENCIES_GUARD_VAR TRUE)
else()
    return()
endif()

# thirdparty dependencies
include(cmake/googletest.cmake)
include(cmake/boost.cmake)
include(compiler_settings)

include_directories(SYSTEM ${BOOST_INCLUDE_DIRS})

# Common dependencies
#include(doxygen)
