#===============================================================================
# Building with CMake:
# --------------------
# This project is designed to be built in a separate directory from the src code.
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

# project name and specify the minumum cmake version.
project(astrotypes)
cmake_policy(SET CMP0048 OLD) # don't let cmake control the version variables
cmake_policy(SET CMP0000 OLD) # don't complain about not having a minimum cmake version

# cmake setup.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# project version.
include(git_version)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} ${PROJECT_SOURCE_VERSION}")

# documentation
if(NOT DOC_INSTALL_DIR)
  set(DOC_INSTALL_DIR ${MODULES_INSTALL_DIR}/doc)
endif(NOT DOC_INSTALL_DIR)
include(doxygen)

# === process through the cheetah tree:
add_subdirectory(src)
