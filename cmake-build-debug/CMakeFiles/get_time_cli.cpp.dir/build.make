# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /softs/clion-2017.1.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /softs/clion-2017.1.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /open_source/webtcp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /open_source/webtcp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/get_time_cli.cpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/get_time_cli.cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/get_time_cli.cpp.dir/flags.make

CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o: CMakeFiles/get_time_cli.cpp.dir/flags.make
CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o: ../get_time_cli.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/open_source/webtcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o -c /open_source/webtcp/get_time_cli.cpp

CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /open_source/webtcp/get_time_cli.cpp > CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.i

CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /open_source/webtcp/get_time_cli.cpp -o CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.s

CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.requires:

.PHONY : CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.requires

CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.provides: CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.requires
	$(MAKE) -f CMakeFiles/get_time_cli.cpp.dir/build.make CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.provides.build
.PHONY : CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.provides

CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.provides.build: CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o


# Object files for target get_time_cli.cpp
get_time_cli_cpp_OBJECTS = \
"CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o"

# External object files for target get_time_cli.cpp
get_time_cli_cpp_EXTERNAL_OBJECTS =

../bin/get_time_cli.cpp: CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o
../bin/get_time_cli.cpp: CMakeFiles/get_time_cli.cpp.dir/build.make
../bin/get_time_cli.cpp: CMakeFiles/get_time_cli.cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/open_source/webtcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/get_time_cli.cpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/get_time_cli.cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/get_time_cli.cpp.dir/build: ../bin/get_time_cli.cpp

.PHONY : CMakeFiles/get_time_cli.cpp.dir/build

CMakeFiles/get_time_cli.cpp.dir/requires: CMakeFiles/get_time_cli.cpp.dir/get_time_cli.cpp.o.requires

.PHONY : CMakeFiles/get_time_cli.cpp.dir/requires

CMakeFiles/get_time_cli.cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/get_time_cli.cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/get_time_cli.cpp.dir/clean

CMakeFiles/get_time_cli.cpp.dir/depend:
	cd /open_source/webtcp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /open_source/webtcp /open_source/webtcp /open_source/webtcp/cmake-build-debug /open_source/webtcp/cmake-build-debug /open_source/webtcp/cmake-build-debug/CMakeFiles/get_time_cli.cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/get_time_cli.cpp.dir/depend
