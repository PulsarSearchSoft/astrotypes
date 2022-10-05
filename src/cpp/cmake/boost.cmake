option(USE_THIRDPARTY_BOOST "Force the use of the included boost, even if a system install is present" false)

if(NOT BOOST_GUARD_VAR)
    set(BOOST_GUARD_VAR TRUE)
else()
    return()
endif()

# Check if the user wants to use the included boost, regardless of if they have a existing install
if(NOT USE_THIRDPARTY_BOOST)
    if(NOT BOOST_FOUND)
        message(STATUS "Checking for an installed boost...")
        # Check for an existing boost install
        find_package(Boost)
        set(BOOST_FOUND ${Boost_FOUND})
        if(BOOST_FOUND)
            set(BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
            set(BOOST_LIBRARIES ${Boost_LIBRARIES})
            set(BOOST_MAJOR_VERSION ${Boost_MAJOR_VERSION})
            set(BOOST_MINOR_VERSION ${Boost_MINOR_VERSION})
            message(STATUS "Boost found (version ${BOOST_MAJOR_VERSION}.${BOOST_MINOR_VERSION})")
        else()
            message(STATUS "Boost not found! Defaulting to thirdparty boost/units...")
        endif()
    endif()
endif()

# If no boost install is found, use the included third-party boost/units
if(NOT BOOST_FOUND)
    message(STATUS "Using thirdparty boost.")
    # We do not use find_package(Boost ...) here, as that calls FindBoost.cmake, which requires libraries to exist, and boost::units is header-only
    set(BOOST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/boost/1.65 CACHE PATH "" FORCE)
    set(BOOST_MAJOR_VERSION 1)
    set(BOOST_MINOR_VERSION 65)
endif()
