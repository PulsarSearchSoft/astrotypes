# -- thirdparty dependencies
include(cmake/googletest.cmake)
include(cmake/boost.cmake)
include(compiler_settings)

include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# -- common dependencies
#include(doxygen)
