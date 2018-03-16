option(USE_THIRDPARTY_BOOST "Force the use of the included boost, even if a system install is present" false)

# Check if the user wants to use the included boost, regardless of if they have a existing install
if(NOT USE_THIRDPARTY_BOOST)
    message(STATUS "Checking for an installed boost...")
    find_package(Boost QUIET) # Check for an existing boost install. NB we fail quietly here, as we can default to using the included boost.
    if(Boost_FOUND)
        message(STATUS "Boost found!")
    else()
        message(STATUS "Boost not found! Defaulting to thirdparty boost/units...")
    endif()
endif()
if(NOT Boost_FOUND) # If no boost install is found, use the included third-party boost/units
    message(STATUS "Using thirdparty boost.")
    # We do noy use find_package(Boost ...) here, as that calls FindBoost.cmake, which requires libraries to exist, and units is header-only
    set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/boost/1.65 CACHE PATH "" FORCE)
    set(Boost_MAJOR_VERSION 1)
    set(Boost_MINOR_VERSION 65)
endif()
