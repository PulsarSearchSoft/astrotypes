include(subpackage)

# Define the astrotypes libraries
set(ASTROTYPES_LIBRARIES ${CMAKE_PROJECT_NAME} ${DEPENDENCY_LIBRARIES})
include_directories(../..)                  # Include paths should all be prefixed with "pss/astrotypes/"
include_directories(${PROJECT_BINARY_DIR})  # Pick up generated files

subpackage(utils)
subpackage(multiarray)
subpackage(units)
subpackage(types)
subpackage(sigproc)

# Should come after all subpackage() directives
include_subpackage_files()

#add_library(${CMAKE_PROJECT_NAME} ${LIB_SRC_CPU})
#install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${LIBRARY_INSTALL_DIR})
