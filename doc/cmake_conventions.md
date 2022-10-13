\anchor cmake-conventions
# CMake Coding Conventions

## 1. Indentation and Spacing

-# Indent each new level using 4 spaces; do not use tabs.

-# Code block inside the following blocks shall be indented:
    -# if/else/endif
    -# foreach/endforeach
    -# while/endwhile
    -# macro/endmacro
    -# function/endfunction

-# No blank lines at the end of files, no whitespace at the end of lines.

-# Do not put a space between a loop/conditional word and the opening parenthesis.

    E.g. do this:

        if(SOMEVAR)
            ...
        else()
            ...
        endif()

    Do not do this:

        if (SOMEVAR)
            ...
        else ()
            ...
        endif ()

-# Do not put spaces between parentheses and arguments.

    E.g. do this:

        my_macro(ARG)

    Do not do this:

        my_macro( ARG )

## 2. Naming Conventions

-# Function and macro names shall be:

    -# all lower case
    -# snake_case

    E.g. my_method(), my_macro(). Do not use camelCase (e.g. myMethod()) or PascalCase (e.g. MyMethod()).

-# Keywords like "EXISTS" and "NOT" shall always be capitalised, e.g. 'if(NOT EXISTS ...)' instead of 'if(not exists ...)'.

-# All words in variables shall be separated by an underscore (_).

-# Local variables, i.e. these that are intended to be visible in their current scope and any dependent scope. For instance, if you are using a variable in a function/macro, or using it in another CMake file that is called from the file in which you set it, then it is a local variable.

    These variables shall be snake_case.

    some_local_var

-# Temporary (working) variables, i.e variables which are defined in the current scope but are not intended to be used outside of that scope. Note that this only applies to macros; variables set in functions are in their own private scope, unless they are promoted to the parent scope. Also, this type of variable is not intended for use in general code, i.e. outside of macros/functions.

    These variables shall be snake_case and start with an underscore.

    _some_temp_var

-# Variable names with a cache or global scope shall be fully capitalised.

    -# SOME_VAR - just a variable

    -# BOOST_SOME_VAR - a variable relating to Boost

    -# OPENCL_SOME_VAR - a variable relating to OpenCL

    For pacakge names, prefer all caps over normal stylisation (generally PascalCase) to match conventions for variables in Find files (see below).

-# Variables that point to executables (for instance, those that are output by find_program()) shall be global variables, i.e. all caps, with _EXE appended, e.g.:

    MY_PROGRAM_EXE

## 3. Language Conventions

-# Always include macro/function names in their respective endmacro()/endfunction().

    E.g. do this:

        macro(my_macro)
            ...
        endmacro(my_macro)

    Do not do this:

        macro(my_macro)
            ...
        endmacro()

-# Do not include complex logic or variables in end() commands, e.g. endforeach(), endif(), endfunction(), endmacro(), endwhile(). Also, do not include it in else().

    E.g. do this:

        if(SOME COMPLEX LOGIC OR LONG VARIABLE)
            ...
        else()
            ...
        endif()

    Do not do this:

        if(SOME COMPLEX LOGIC OR LONG VARIABLE)
            ...
        else(SOME COMPLEX LOGIC OR LONG VARIABLE)
            ...
        endif(SOME COMPLEX LOGIC OR LONG VARIABLE)

-# Prefer not to include input variables in else() or end() commands, e.g. endforeach(), endif(), endfunction(), endmacro(), endwhile(), unless thay add clarity and are simple, single variables.

    E.g. do this:

        if(VAR)
            ...
        else(VAR)
            ...
        endif(VAR)

    only if it helps make it visually clearer to which if() statement the else() and endif() belong. Even then, only include the variable in the else()/endif() if it doesn't make the code look too busy.

-# Appending an element to a list variable shall preferably be done with the 'APPEND' keyword, not the 'set' macro (unless setting the variable in the parent scope).

    E.g. try to do this:

        list(APPEND XYZ ...)

    Try not do this (only use this method if appending to the list fails):

        set(xyz ${XYZ} ...)

    One possibility for failure of 'list(APPEND ...)' could be that this function creates a semicolon separated list of strings, whereas 'set(xyz ...)' creates a single string, so might impact setting CXX flags, for example.

-# Prefer functions over macros, as CMake functions have their own scope, i.e. any variables set in them will not appear in any other scope (calling, dependent, parent) unless a variable is set as visible in the parent scope.

-# Every .cmake file is required to have guards at the beginning to stop the content of the file from being reread if the file is accidentally loaded multiple times.

    Guards shall have the following general syntax:

        if(NOT PSS_ASTROTYPES_MY_FILE_GUARD_VAR)
            set(PSS_ASTROTYPES_MY_FILE_GUARD_VAR TRUE)
        else()
            return()
        endif()

    This way, if the file has already been included, this guard will return from the .cmake file before any of the rest of it is processed.

## 4. Dependencies

-# For checking the results inside a Find module, the macro find_package_handle_standard_args() shall be used.

-# Every tool must have its own .cmake file inside of the src/cpp/cmake/ directory, named appropriately.

    -# If the only function of the .cmake file is to find an external tool, the file shall be named FindXxx.cmake (e.g. FindAstroTypes.cmake),
    -# Otherwise the file shall be named ${package}.cmake (e.g. subpackage.cmake).

-# Do not bypass checking user input.

    E.g.:

        if(FOO_LIBRARY AND FOO_INCLUDE_DIR)
            set(FOO_FOUND TRUE)
        else()
            ... execute the whole find-logic
        endif()

    The find-logic shall always be executed. These shortcuts can cause problems, e.g. when the same file is used from multiple directories but with different required versions or components.

-# Every dependency shall have an option to enable it.

    E.g.:

        ENABLE_PACKAGE_NAME

    These shall be set to "off" by default.

-# Manually quieting the module shall not be done.

    e.g. do not do things like this:

        if(FOO_INCLUDE_DIR AND FOO_LIBRARIES)
            set(FOO_FIND_QUIETLY TRUE)
        endif()

    If find_package_handle_standard_args() is used, this is completely unnecessary, since find_package_handle_standard_args() only prints something if the result has changed compared to the previous run. So if, as in this example, Foo has already been found, find_package_handle_standard_args() will only print this information during the first CMake run, but not on successive runs.

    ### 1. Find Modules

    -# In astrotypes, most Find modules are written for module mode (as opposed to config mode).

        -# A Find module shall set the <PACKAGE>_FOUND variable.
        -# All modules shall provide backwards compatibility with CMake version 2.8.12.

    -# Most of the cache variables shall be hidden in the ccmake interface unless the user explicitly asks to edit them:

            mark_as_advanced(
                FOO_LIBRARY
                FOO_INCLUDE_DIR
            )

    ### 2. Required Variables For Dependencies

    -# The following variable names shall be used to keep things consistent between Find modules. Note that all variables start with XXX_, which (unless otherwise noted) must exactly match the name of the FindXxx.cmake file, but in all caps. This prefix on the variable names ensures that they do not conflict with variables from other Find modules. The same pattern shall also be followed for any macros and functions defined by the Find module.

        - XXX_INCLUDE_DIRS
            - The final set of include directories listed in a single variable. This shall not be a cache entry (note that this also means this variable shall not be set as the output of the find_path() command - see XXX_INCLUDE_DIR below for that).

        - XXX_LIBRARIES
            - The libraries to use with the module. These may be CMake targets, full absolute paths to a library binary or the name of a library that the linker must find in its search path. This shall not be a cache entry (note that this also means this variable shall not be set to the output of the find_library() command - see XXX_LIBRARY below for that).

        - XXX_LIBRARY_DIRS
            - Optionally, the final set of library directories listed in a single variable. This shall not be a cache entry.

        - XXX_SOURCE_DIR
            - Where to find the base directory of the module.

        - XXX_yyy_FOUND
            - For variables of this form, yyy is the name of a component for the module. It shall exactly match one of the valid component names that may be passed to the find_package() command for the module. If a variable of this form is set to FALSE, it means that the yyy component of module XXX was not found or is not available. Variables of this form shall typically be used for optional components so that the caller can check whether an optional component is available.

        - XXX_FOUND
            - When the find_package() command returns to the caller, this variable will be set to true if the module was deemed to have been found successfully.

        - XXX_VERSION
            - The full version string of the package found, if any. Note that many existing modules provide XXX_VERSION_STRING instead.

        - XXX_VERSION_MAJOR
            - The major version of the package found, if any.

        - XXX_VERSION_MINOR
            - The minor version of the package found, if any.

    -# The following names shall not be used in CMakeLists.txt files. They are intended for use by Find modules to specify and cache the locations of specific files or directories. Users are typically able to set and edit these variables to control the behavior of Find modules (like entering the path to a library manually):

        - XXX_LIBRARY
            - The path of the library. Use this form only when the module provides a single library. It is appropriate to set this to the output of a call to find_library().

        - XXX_yyy_LIBRARY
            - The path of library yyy provided by the module XXX. Use this form when the module provides more than one library or where other modules may also provide a library of the same name. It is also appropriate to set this to the output of a call to find_library().

        - XXX_INCLUDE_DIR
            - When the module provides only a single library, this variable can be used to specify where to find headers for using the library (or more accurately, the path that consumers of the library shall add to their header search path). It is appropriate to set this to the output of a call to find_path().

        - XXX_yyy_INCLUDE_DIR
            - If the module provides more than one library or where other modules may also provide a library of the same name, this form is recommended for specifying where to find headers for using library yyy provided by the module XXX. Again, it is appropriate to set this to the output of a call to find_path().

        To prevent users being overwhelmed with settings to configure, try to keep as many options as possible out of the cache, leaving at least one option which can be used to disable use of the module, or locate a not-found library (e.g. XXX_SOURCE_DIR). For the same reason, mark most cache options as advanced.

## 5. Documentation

-# Functions and Macros

    Every .cmake file shall be documented to help the user follow what is being done. This can be done in a simple way, with a single comment above a line of code, or a brief synopsis at the beginning of a file to explain the flow of the code. Specifically, however, macros and functions shall be documented more stringently, in a doxygen-like style with set required sections and other optional sections.

    -# Put the function/macro name at the top.

    -# The documentation must include a "brief" section. This shall be short, but be clear enough so the user knows what the function/macro does. If more than one sentence is required, put the extra info in a "detail" section.

    -# The documentation must include a "usage" section. This shall mirror the way the user will call the function/macro, with no extraneous info. If any clarification is required, put the extra info in a "detail" section.

    -# The "details", "examples", and "dev notes" sections are optional, and can be used separately or together, as needed.

    Here is an example:

        #========================================================
        # my_func
        #
        # brief: short description of the purpose of the function/macro
        #
        # usage: my_func(REQUIRED_VARS)
        #
        # details: any specifics required by the user on how to properly use the function/macro or
        #          what the function/macro might do/return that is not clear from the brief description
        #
        # examples: any useful examples that might show the user how to use the function/macro in
        #           a different way from the standard usage, especially if there are multiple or
        #           extended ways to use the function/macro
        #
        # dev notes: any notes that might be useful to other devs as to why something was done a
        #            certain way that may not be conventional, e.g. "this must be a macro because..."
        #========================================================

-# Find modules

    Documentation shall consist of:

    -# An underlined heading specifying the module name.
    -# A simple description of what the module finds.
    -# Any information required and relevant to the user, e.g. selectable components.
    -# A section (or sections, if there are different variables for multiple sections from the above step) listing variables set by the module.
    -# A section listing variables that can be used to specify component locations, if any.

    If the module provides any macros or functions, they shall be listed in an additional section, but can be documented by additional comment blocks immediately above where those macros or functions are defined.

    The Find module implementation may begin below the documentation block.

#### Sample Find Module

    # Distributed under the OSI-approved BSD 3-Clause License.
    # See https://cmake.org/licensing for details.

    #=======================================================================
    # FindFoo
    # -------
    #
    # Finds the Foo library.
    #
    # Components
    # ^^^^^^^^^^
    #
    # libraries - Foo libraries
    #
    # headers - Foo header files
    #
    # Result Variables
    # ^^^^^^^^^^^^^^^^
    #
    # libraries -
    #
    #     This will define the following variables:
    #
    #         ``FOO_FOUND``
    #           True if the system has the Foo library.
    #
    #         ``FOO_VERSION``
    #           The version of the Foo library which was found.
    #
    #         ``FOO_LIBRARIES``
    #           Libraries needed to link to Foo.
    #
    # headers -
    #
    #     This will define the following variables:
    #
    #         ``FOO_FOUND``
    #           True if the system has the Foo library.
    #
    #         ``FOO_VERSION``
    #           The version of the Foo library which was found.
    #
    #         ``FOO_INCLUDE_DIRS``
    #           Include directories needed to use Foo.
    #
    # Location Variables
    # ^^^^^^^^^^^^^^^^^^
    #
    # The following variables can be set to specify the location of a component:
    #
    #     ``FOO_INCLUDE_DIR``
    #       The directory containing ``foo.h``.
    #
    #     ``FOO_LIBRARY``
    #       The path to the Foo library.
    #
    #=======================================================================

## 6. Changes/deprecations

The following changes/deprecations could impact us if we upgrade to a newer version of CMake, as we currently use these features. Try to use the newer syntax/variables/macros where possible to avoid work later if/when the version of CMake is updated:

-# Version 3.0 - The COMPILE_FLAGS target property is now documented as deprecated, though no warning is issued. Use the COMPILE_OPTIONS target property or the target_compile_options() command instead.

-# Version 3.6 - The find_library(), find_path(), and find_file() commands no longer search in installation prefixes derived from the PATH environment variable on non-Windows platforms. This behavior was added in CMake 3.3 to support Windows hosts but has proven problematic on UNIX hosts. Users that keep some <prefix>/bin directories in the PATH just for their tools do not necessarily want any supporting <prefix>/lib directories searched. One may set the CMAKE_PREFIX_PATH environment variable with a semicolon-separated list of prefixes that are to be searched.

-# Version 3.9 - The FindDoxygen module has deprecated several variables. (NB we do not use this supplied module, we have written our own Doxygen Find module)
