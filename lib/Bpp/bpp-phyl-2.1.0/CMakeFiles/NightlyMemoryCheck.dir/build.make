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
CMAKE_SOURCE_DIR = /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0

# Utility rule file for NightlyMemoryCheck.

# Include the progress variables for this target.
include CMakeFiles/NightlyMemoryCheck.dir/progress.make

CMakeFiles/NightlyMemoryCheck:
	/usr/local/Cellar/cmake/3.13.2/bin/ctest -D NightlyMemoryCheck

NightlyMemoryCheck: CMakeFiles/NightlyMemoryCheck
NightlyMemoryCheck: CMakeFiles/NightlyMemoryCheck.dir/build.make

.PHONY : NightlyMemoryCheck

# Rule to build all files generated by this target.
CMakeFiles/NightlyMemoryCheck.dir/build: NightlyMemoryCheck

.PHONY : CMakeFiles/NightlyMemoryCheck.dir/build

CMakeFiles/NightlyMemoryCheck.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NightlyMemoryCheck.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NightlyMemoryCheck.dir/clean

CMakeFiles/NightlyMemoryCheck.dir/depend:
	cd /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0 /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0 /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0 /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0 /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0/CMakeFiles/NightlyMemoryCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NightlyMemoryCheck.dir/depend

