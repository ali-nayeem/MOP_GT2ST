# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.13.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.13.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0

# Include any dependencies generated for this target.
include test/CMakeFiles/test_numconstants.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_numconstants.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_numconstants.dir/flags.make

test/CMakeFiles/test_numconstants.dir/test_numconstants.cpp.o: test/CMakeFiles/test_numconstants.dir/flags.make
test/CMakeFiles/test_numconstants.dir/test_numconstants.cpp.o: test/test_numconstants.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_numconstants.dir/test_numconstants.cpp.o"
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test && /opt/local/bin/g++-mp-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_numconstants.dir/test_numconstants.cpp.o -c /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test/test_numconstants.cpp

test/CMakeFiles/test_numconstants.dir/test_numconstants.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_numconstants.dir/test_numconstants.cpp.i"
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test && /opt/local/bin/g++-mp-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test/test_numconstants.cpp > CMakeFiles/test_numconstants.dir/test_numconstants.cpp.i

test/CMakeFiles/test_numconstants.dir/test_numconstants.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_numconstants.dir/test_numconstants.cpp.s"
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test && /opt/local/bin/g++-mp-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test/test_numconstants.cpp -o CMakeFiles/test_numconstants.dir/test_numconstants.cpp.s

# Object files for target test_numconstants
test_numconstants_OBJECTS = \
"CMakeFiles/test_numconstants.dir/test_numconstants.cpp.o"

# External object files for target test_numconstants
test_numconstants_EXTERNAL_OBJECTS =

test/test_numconstants: test/CMakeFiles/test_numconstants.dir/test_numconstants.cpp.o
test/test_numconstants: test/CMakeFiles/test_numconstants.dir/build.make
test/test_numconstants: /opt/local/lib/libbpp-core.dylib
test/test_numconstants: test/CMakeFiles/test_numconstants.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_numconstants"
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_numconstants.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_numconstants.dir/build: test/test_numconstants

.PHONY : test/CMakeFiles/test_numconstants.dir/build

test/CMakeFiles/test_numconstants.dir/clean:
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test && $(CMAKE_COMMAND) -P CMakeFiles/test_numconstants.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_numconstants.dir/clean

test/CMakeFiles/test_numconstants.dir/depend:
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0 /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0 /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/test/CMakeFiles/test_numconstants.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_numconstants.dir/depend

