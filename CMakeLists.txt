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

# Specify project name and minumum cmake version
project(astrotypes)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW) # use full paths to libraries on the link line
    cmake_policy(SET CMP0000 OLD) # don't complain about not having a minimum cmake version

    if(${CMAKE_MAJOR_VERSION} GREATER 2)
        cmake_policy(SET CMP0048 OLD) # don't let cmake control the version variables
    endif()
endif()
enable_testing()

# CMake setup
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# Project version
include(git_version)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} ${PROJECT_SOURCE_VERSION}")

# Installations
if(NOT MODULES_INSTALL_DIR)
    set(MODULES_INSTALL_DIR "share/${PROJECT_NAME}")
endif()

if(NOT DOC_INSTALL_DIR)
    set(DOC_INSTALL_DIR ${MODULES_INSTALL_DIR}/doc)
endif()

include(doxygen)

# Process through the astrotypes tree
add_subdirectory(src)
