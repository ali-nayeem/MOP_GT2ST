# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0

# Utility rule file for rpm.

# Include the progress variables for this target.
include CMakeFiles/rpm.dir/progress.make

CMakeFiles/rpm:
	rpmbuild -ta bpp-seq-2.1.0.tar.gz

rpm: CMakeFiles/rpm
rpm: CMakeFiles/rpm.dir/build.make

.PHONY : rpm

# Rule to build all files generated by this target.
CMakeFiles/rpm.dir/build: rpm

.PHONY : CMakeFiles/rpm.dir/build

CMakeFiles/rpm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rpm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rpm.dir/clean

CMakeFiles/rpm.dir/depend:
	cd /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0 /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0 /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0 /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0 /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0/CMakeFiles/rpm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rpm.dir/depend

