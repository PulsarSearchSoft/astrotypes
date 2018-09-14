@section Installation Installation
## Header Only Library
astrotypes is a header only library and so installation is a case of 
using `#include` directives and setting the -I option on
your compiler to point to the astrotypes headers install location.
Note that the [dependencies](@ref Dependencies) will also need to be installed.

### Example: including astrotypes using git subtrees
If you are using git to mamange your repository one of the many strategies
available is called subtrees. Although we provide a subtree example we do not
necessarily condone its use. Each stategy has its own merits and
compromises and the user should satisfy themselves as to the most appropriate
one for their project.

From the top level of your project, create a directory for putting in thirdparty
dependencies.
@code
mkdir thirdparty
@endcode
Now use git subtrees to install astrotypes
@code
git subtree add --prefix thirdparty/astrotypes https://github.com/PulsarSearchSoft/astrotypes.git master --squash
@endcode

Any time you need to update the version of astrotypes you can use:
@code
git subtree pull --prefix thirdparty/astrotypes https://github.com/PulsarSearchSoft/astrotypes.git master --squash
@endcode

The only thing left to do is to update your build system so that your thirdparty directory, and the required dependencies
are added to the include path.

## Building the unit tests and examples: cmake
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

@section Dependencies Dependencies 
### Boost
Boost Website: https://www.boost.org

This project requires the boost_units library.
You can use your local, or system installation of boost libs for this, however for convenience we also provide a
packaged version of boost_units (and its dependencies) in the thirdparty directory of this repository.

By default the cmake for building the tests and examples will look for boost in the following order:
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
