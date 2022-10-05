#===============================================================================
# C++ Library/Application specific CMake template.
# As a general rule add extensions to files in the cmake directory
#===============================================================================
# Building with CMake:
# --------------------
#
# This project is designed to be built in a separate directory from the source code.
#
#   1) create a build directory
#   2) chdir <build_dir>
#   3) cmake <cmake_options> <path_to_src>
#   4) make
#   5) make doc (optional - only if -DENABLE_DOC=true is set)
#   6) make install
#
# Some Common Build options:
# --------------------------
#
#   -DCMAKE_BUILD_TYPE={debug|release|profile):
#       Build in debug, release or release with debug info mode.
#
#   -DCMAKE_CXX_COMPILER={compiler} (default: c++)
#       Sets the C++ compiler.
#
#   -DCMAKE_INSTALL_PREFIX
#       Root directory used to install files when calling 'make install'
#       The default for this is usually /usr/local.
#
#===============================================================================

# Specify project name and programming language
project(astrotypes CXX)

# CMake setup
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message("SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

enable_testing()

# Dev tool integration
include(code_coverage)  # Add code_coverage targets
include(valgrind)       # Add valgrind targets

# Default installation locations (note these will be prefixed by ${CMAKE_INSTALL_PREFIX})
set(INCLUDE_DIR_STRUCTURE "pss/${PROJECT_NAME}")

if(NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR "lib")
endif()

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/${INCLUDE_DIR_STRUCTURE}")
endif()

if(NOT MODULES_INSTALL_DIR)
    set(MODULES_INSTALL_DIR "share/${INCLUDE_DIR_STRUCTURE}")
endif()

if(NOT DOC_INSTALL_DIR)
    set(DOC_INSTALL_DIR ${MODULES_INSTALL_DIR}/doc)
endif()

if(NOT BINARY_INSTALL_DIR)
    set(BINARY_INSTALL_DIR "bin")
endif()

# Get project dependencies and compiler settings
include(dependencies)

# Create a header file with the project version
configure_file(${PROJECT_SOURCE_DIR}/cmake/version.h.in
               ${PROJECT_BINARY_DIR}/${INCLUDE_DIR_STRUCTURE}/version.h
)
install(FILES ${PROJECT_BINARY_DIR}/${INCLUDE_DIR_STRUCTURE}/version.h
        DESTINATION ${INCLUDE_INSTALL_DIR}
)

# Process through the astrotypes tree
add_subdirectory(${INCLUDE_DIR_STRUCTURE})

# Print build options summary
set(DEBUG_PRINT ON)
if(DEBUG_PRINT)
    message(STATUS "")
    message(STATUS "****************************************************************************")
    message(STATUS " name: ${PROJECT_NAME} version: ${PROJECT_VERSION}")
    message(STATUS "")
    message(STATUS "")
    message(STATUS " Compiler Options:")
    message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "  C++ Compiler: ${CMAKE_CXX_COMPILER}")
    if(${CMAKE_BUILD_TYPE} MATCHES release)
        message(STATUS "  C++ flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
    elseif(${CMAKE_BUILD_TYPE} MATCHES debug)
        message(STATUS "  C++ flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
    elseif(${CMAKE_BUILD_TYPE} MATCHES relwithdebinfo)
        message(STATUS "  C++ flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    elseif(${CMAKE_BUILD_TYPE} MATCHES minsizerel)
        message(STATUS "  C++ flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_MINSIZEREL}")
    endif()
    message(STATUS "")
    message(STATUS "Install locations: (make install)")
    message(STATUS "  Libraries: ${CMAKE_INSTALL_PREFIX}/lib")
    message(STATUS "  Inclues:   ${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}")
    message(STATUS "  Binaries:  ${CMAKE_INSTALL_PREFIX}/${BINARY_INSTALL_DIR}")
    if(ENABLE_DOC)
        message(STATUS "  Doc:       ${CMAKE_INSTALL_PREFIX}/${DOC_INSTALL_DIR}")
    else()
        message(STATUS "  Doc:       Disabled (-DENABLE_DOC=true to enable)")
    endif()
    message(STATUS "  Other:     ${CMAKE_INSTALL_PREFIX}/${MODULES_INSTALL_DIR}")
    message(STATUS "****************************************************************************")
    message(STATUS "")
endif()
