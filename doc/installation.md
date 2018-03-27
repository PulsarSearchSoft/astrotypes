@section Installation Installation
## cmake
Cmake is a widely used, platform and toolchain agnostic build tool.
One of the features of cmake is that it provides out of source builds
so you don't mix up build products (libraries, binaries, etc) from the source code.
This is very powerful in other ways, you may want to build the same source using
different compilers and compare the results.

Configuring your build with cmake:
  - create a directory for your build products
    @code
    mkdir build
    @endcode
  - point cmake to the top of the astrotypes repository (along with any flags)
    and it will generate the appropriate build files for your host system 
    (gnu make files on linux, nmake or .proj files for windows, etc)

    e.g to build a debug debug 
    @code
    cmake -DCMAKE_BUILD_TYPE=debug <path_to_astrotypes_repository>
    @endcode
    e.g to build a fully optimised version
    @code
    cmake -DCMAKE_BUILD_TYPE=release <path_to_astrotypes_repository>
    @endcode

    Note that cmake will do its best to determine where dependencies are
    on your system. You can override this behaviour with special flags.
    See the dependencies section for more details and options.

  - run make
    what you actually run will depend on your platform, but on linux this
    will be, by default, make.

    @code
    make
    @endcode

    If you want to see whats going on under the hood you can run

    @code
    make VERBOSE=1
    @endcode

## Dependencies 
### Boost
This project requires the boost_units library.
You can use your local, or system installation of boost libs for this, however for convenience we also provide a
packaged version of boost_units (and its dependencies) in the thirdparty directory of this repository.

By default cmake will look for boost in the following order:
    - User defined variables: 
        @code
        -DBoost_ROOT=<path_to_boost_root> 
        @endcode
        or
        @code
        -DBoost_INCLUDE_DIRS=<path_to_boost_includes> & Boost_LIBRARY_DIRS=<path_to_boost_libs>
        @endcode
    - The usual system paths (/usr/local/lib, /usr/lib etc.)
    - Use the the thirdparty version

If you wish to force the use of the thridparty packaged version add the flag -DUSE_THIRDPARTY_BOOST=true to ignore any boost versions installed on the system.
Note that boost units is a header-only library and does not need to be built.

Cmake Variable         |  Description
-----------------------|-------------
USE_THIRDPARTY_BOOST   |	Force the use of the packaged boost version, ignoring any installed boost versions (default: false)
Boost_LIBRARY_DIRS     |	Explicitly specify a directory where the boost libraries reside
Boost_INCLUDE_DIRS     |	Explicitly specify a directory where the boost header files reside
Boost_ROOT             |	Explicitly specify a directory where the boost distiburion resides
