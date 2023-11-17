# Compiler defaults for astrotypes

if(NOT PSS_ASTROTYPES_COMPILER_SETTINGS_GUARD_VAR)
    set(PSS_ASTROTYPES_COMPILER_SETTINGS_GUARD_VAR TRUE)
else()
    return()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "release")
endif()

# Set compiler flags
set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER MATCHES icpc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcheck -wd2259 -wd1125")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror")
    if(CMAKE_BUILD_TYPE MATCHES profile)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0  -fprofile-arcs -ftest-coverage")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    # -Wl, --no-as-needed avoids linker problem with libfftwf3 on ubuntu systems
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align -march=native")
    if(CMAKE_BUILD_TYPE MATCHES profile)
        # -pg for gprof
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0  -fprofile-arcs -ftest-coverage")
    endif()
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -Wno-unused-local-typedefs")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -Wall -Wextra -pedantic -Wno-unused-local-typedefs")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra -pedantic -Wno-unused-local-typedefs")
