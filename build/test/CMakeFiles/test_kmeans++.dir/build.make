# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.5/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/catkin/workspace/seu

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/catkin/workspace/seu/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_kmeans++.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_kmeans++.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_kmeans++.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_kmeans++.dir/flags.make

test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o: test/CMakeFiles/test_kmeans++.dir/flags.make
test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o: /Users/catkin/workspace/seu/test/test_kmeans++.cc
test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o: test/CMakeFiles/test_kmeans++.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/catkin/workspace/seu/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o"
	cd /Users/catkin/workspace/seu/build/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o -MF CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o.d -o CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o -c /Users/catkin/workspace/seu/test/test_kmeans++.cc

test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.i"
	cd /Users/catkin/workspace/seu/build/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/catkin/workspace/seu/test/test_kmeans++.cc > CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.i

test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.s"
	cd /Users/catkin/workspace/seu/build/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/catkin/workspace/seu/test/test_kmeans++.cc -o CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.s

# Object files for target test_kmeans++
test_kmeans_______OBJECTS = \
"CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o"

# External object files for target test_kmeans++
test_kmeans_______EXTERNAL_OBJECTS =

bin/test/test_kmeans++: test/CMakeFiles/test_kmeans++.dir/test_kmeans++.cc.o
bin/test/test_kmeans++: test/CMakeFiles/test_kmeans++.dir/build.make
bin/test/test_kmeans++: lib/libseu.a
bin/test/test_kmeans++: /opt/homebrew/lib/libjsoncpp.26.dylib
bin/test/test_kmeans++: /Library/gurobi1102/macos_universal2/lib/libgurobi_c++.a
bin/test/test_kmeans++: /usr/local/lib/libgurobi110.dylib
bin/test/test_kmeans++: test/CMakeFiles/test_kmeans++.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/catkin/workspace/seu/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test/test_kmeans++"
	cd /Users/catkin/workspace/seu/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_kmeans++.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_kmeans++.dir/build: bin/test/test_kmeans++
.PHONY : test/CMakeFiles/test_kmeans++.dir/build

test/CMakeFiles/test_kmeans++.dir/clean:
	cd /Users/catkin/workspace/seu/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_kmeans++.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_kmeans++.dir/clean

test/CMakeFiles/test_kmeans++.dir/depend:
	cd /Users/catkin/workspace/seu/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/catkin/workspace/seu /Users/catkin/workspace/seu/test /Users/catkin/workspace/seu/build /Users/catkin/workspace/seu/build/test /Users/catkin/workspace/seu/build/test/CMakeFiles/test_kmeans++.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/CMakeFiles/test_kmeans++.dir/depend
