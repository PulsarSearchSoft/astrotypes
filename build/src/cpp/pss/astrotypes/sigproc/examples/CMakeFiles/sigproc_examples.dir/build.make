# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/arun/astrotypes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arun/astrotypes/build

# Include any dependencies generated for this target.
include src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/depend.make

# Include the progress variables for this target.
include src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/progress.make

# Include the compile flags for this target's objects.
include src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/flags.make

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/flags.make
src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o: ../src/cpp/pss/astrotypes/sigproc/examples/src/ExtendedHeader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arun/astrotypes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o"
	cd /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o -c /home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/examples/src/ExtendedHeader.cpp

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.i"
	cd /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/examples/src/ExtendedHeader.cpp > CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.i

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.s"
	cd /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/examples/src/ExtendedHeader.cpp -o CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.s

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.requires:

.PHONY : src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.requires

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.provides: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.requires
	$(MAKE) -f src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/build.make src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.provides.build
.PHONY : src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.provides

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.provides.build: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o


# Object files for target sigproc_examples
sigproc_examples_OBJECTS = \
"CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o"

# External object files for target sigproc_examples
sigproc_examples_EXTERNAL_OBJECTS =

src/cpp/pss/astrotypes/sigproc/examples/libsigproc_examples.a: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o
src/cpp/pss/astrotypes/sigproc/examples/libsigproc_examples.a: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/build.make
src/cpp/pss/astrotypes/sigproc/examples/libsigproc_examples.a: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/arun/astrotypes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsigproc_examples.a"
	cd /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples && $(CMAKE_COMMAND) -P CMakeFiles/sigproc_examples.dir/cmake_clean_target.cmake
	cd /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sigproc_examples.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/build: src/cpp/pss/astrotypes/sigproc/examples/libsigproc_examples.a

.PHONY : src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/build

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/requires: src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/src/ExtendedHeader.o.requires

.PHONY : src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/requires

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/clean:
	cd /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples && $(CMAKE_COMMAND) -P CMakeFiles/sigproc_examples.dir/cmake_clean.cmake
.PHONY : src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/clean

src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/depend:
	cd /home/arun/astrotypes/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arun/astrotypes /home/arun/astrotypes/src/cpp/pss/astrotypes/sigproc/examples /home/arun/astrotypes/build /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples /home/arun/astrotypes/build/src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/cpp/pss/astrotypes/sigproc/examples/CMakeFiles/sigproc_examples.dir/depend

