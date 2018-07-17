include(subpackage)

# define the cheetah libraries
set(ASTROTYPES_LIBRARIES ${CMAKE_PROJECT_NAME} ${DEPENDENCY_LIBRARIES})
include_directories(../..)                  # include paths should all be prefixed with "pss/astrotypes/"
include_directories(${PROJECT_BINARY_DIR})  # pick up generated files

SUBPACKAGE(multiarray)
SUBPACKAGE(units)
SUBPACKAGE(types)
SUBPACKAGE(sigproc)

# should come after all SUBPACKAGE directives
include_subpackage_files()
add_library(${CMAKE_PROJECT_NAME} ${lib_src_cpu}) 

#install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${LIBRARY_INSTALL_DIR})
