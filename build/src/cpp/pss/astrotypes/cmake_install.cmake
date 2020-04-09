# Install script for directory: /home/arun/astrotypes/src/cpp/pss/astrotypes

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/utils" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/utils/Optional.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/utils/TypeTraits.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/utils/detail" TYPE FILE FILES "/home/arun/astrotypes/src/cpp/pss/astrotypes/utils/detail/Optional.cpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/multiarray" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/DataBuffer.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/DimensionIndex.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/DimensionSize.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/DimensionSpan.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/ExtendedMultiArray.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/IndexMapper.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/MultiArray.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/MultiIterator.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/NumericalRepresentation.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/OverlaySliceIterator.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/PointerAllocator.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/ResizeAdapter.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/Slice.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/SliceIterator.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/StandardAllocator.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/TypeTraits.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/View.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/ViewIterator.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/multiarray/detail" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/DataBuffer.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/DimensionIndex.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/DimensionSize.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/DimensionSpan.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/ExtendedMultiArray.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/IndexMapper.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/MultiArray.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/MultiIterator.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/OverlaySliceIterator.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/ReducedRankSlice.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/ReducedRankSlice.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/ResizeAdapter.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/Slice.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/SliceHelpers.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/SliceIterator.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/SlicePosition.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/SlicePosition.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/View.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/multiarray/detail/ViewIterator.cpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/units" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Angle.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/BoostDurationCast.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/DataRate.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/DefineClock.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/DispersionMeasure.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Frequency.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/J2000Clock.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/JulianClock.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Length.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/ModifiedJulianClock.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Quantity.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Time.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/TimePoint.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/TimeUnits.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Units.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/Utilities.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/cmath.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/units/detail" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/detail/BoostDurationCast.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/units/detail/TimePoint.cpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/types" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/types/ExtendedTimeFrequency.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/types/TimeFrequency.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/types/detail" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/types/detail/ExtendedTimeFrequency.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/types/detail/TimeFrequency.cpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/sigproc" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/DataFactory.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/DefaultDataFactoryTraits.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/FileReader.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/Header.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/HeaderField.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/IStream.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/OStream.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/SigProc.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/SigProcFormat.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/SigProcFormatAdapterSelector.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/astrotypes/sigproc/detail" TYPE FILE FILES
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/FileReader.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/Header.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/HeaderBase.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/HeaderBase.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/HeaderField.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/HeaderFieldDataType.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/HeaderFieldDataType.h"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/IStream.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/SigProcFormat.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/SigProcVariable.cpp"
    "/home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/detail/SigProcVariable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/arun/astrotypes/build/src/cpp/pss/astrotypes/utils/cmake_install.cmake")
  include("/home/arun/astrotypes/build/src/cpp/pss/astrotypes/multiarray/cmake_install.cmake")
  include("/home/arun/astrotypes/build/src/cpp/pss/astrotypes/units/cmake_install.cmake")
  include("/home/arun/astrotypes/build/src/cpp/pss/astrotypes/types/cmake_install.cmake")
  include("/home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/cmake_install.cmake")

endif()

