# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /snap/cmake/1361/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1361/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ezequiel/simulador/smp-mdk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ezequiel/simulador/smp-mdk/data

# Include any dependencies generated for this target.
include tests/CMakeFiles/tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/tests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/tests.dir/flags.make

tests/CMakeFiles/tests.dir/test.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/test.cpp.o: /home/ezequiel/simulador/smp-mdk/tests/test.cpp
tests/CMakeFiles/tests.dir/test.cpp.o: tests/CMakeFiles/tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ezequiel/simulador/smp-mdk/data/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/tests.dir/test.cpp.o"
	cd /home/ezequiel/simulador/smp-mdk/data/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/tests.dir/test.cpp.o -MF CMakeFiles/tests.dir/test.cpp.o.d -o CMakeFiles/tests.dir/test.cpp.o -c /home/ezequiel/simulador/smp-mdk/tests/test.cpp

tests/CMakeFiles/tests.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tests.dir/test.cpp.i"
	cd /home/ezequiel/simulador/smp-mdk/data/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ezequiel/simulador/smp-mdk/tests/test.cpp > CMakeFiles/tests.dir/test.cpp.i

tests/CMakeFiles/tests.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tests.dir/test.cpp.s"
	cd /home/ezequiel/simulador/smp-mdk/data/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ezequiel/simulador/smp-mdk/tests/test.cpp -o CMakeFiles/tests.dir/test.cpp.s

# Object files for target tests
tests_OBJECTS = \
"CMakeFiles/tests.dir/test.cpp.o"

# External object files for target tests
tests_EXTERNAL_OBJECTS =

tests/tests: tests/CMakeFiles/tests.dir/test.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/build.make
tests/tests: /usr/local/lib/libCatch2Main.a
tests/tests: /usr/local/lib/libCatch2.a
tests/tests: tests/CMakeFiles/tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ezequiel/simulador/smp-mdk/data/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tests"
	cd /home/ezequiel/simulador/smp-mdk/data/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/tests.dir/build: tests/tests
.PHONY : tests/CMakeFiles/tests.dir/build

tests/CMakeFiles/tests.dir/clean:
	cd /home/ezequiel/simulador/smp-mdk/data/tests && $(CMAKE_COMMAND) -P CMakeFiles/tests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/tests.dir/clean

tests/CMakeFiles/tests.dir/depend:
	cd /home/ezequiel/simulador/smp-mdk/data && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ezequiel/simulador/smp-mdk /home/ezequiel/simulador/smp-mdk/tests /home/ezequiel/simulador/smp-mdk/data /home/ezequiel/simulador/smp-mdk/data/tests /home/ezequiel/simulador/smp-mdk/data/tests/CMakeFiles/tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/tests.dir/depend
