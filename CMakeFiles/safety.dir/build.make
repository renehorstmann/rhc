# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# The CMake executable.
CMAKE_COMMAND = /data/data/ru.iiec.cxxdroid/files/bin/cmake

# The command to remove a file.
RM = /data/data/ru.iiec.cxxdroid/files/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /storage/emulated/0/GitProjects/rhc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/emulated/0/GitProjects/rhc

# Include any dependencies generated for this target.
include CMakeFiles/safety.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/safety.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/safety.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/safety.dir/flags.make

CMakeFiles/safety.dir/examples/safety.c.o: CMakeFiles/safety.dir/flags.make
CMakeFiles/safety.dir/examples/safety.c.o: examples/safety.c
CMakeFiles/safety.dir/examples/safety.c.o: CMakeFiles/safety.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/emulated/0/GitProjects/rhc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/safety.dir/examples/safety.c.o"
	/data/user/0/ru.iiec.cxxdroid/files/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/safety.dir/examples/safety.c.o -MF CMakeFiles/safety.dir/examples/safety.c.o.d -o CMakeFiles/safety.dir/examples/safety.c.o -c /storage/emulated/0/GitProjects/rhc/examples/safety.c

CMakeFiles/safety.dir/examples/safety.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/safety.dir/examples/safety.c.i"
	/data/user/0/ru.iiec.cxxdroid/files/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /storage/emulated/0/GitProjects/rhc/examples/safety.c > CMakeFiles/safety.dir/examples/safety.c.i

CMakeFiles/safety.dir/examples/safety.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/safety.dir/examples/safety.c.s"
	/data/user/0/ru.iiec.cxxdroid/files/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /storage/emulated/0/GitProjects/rhc/examples/safety.c -o CMakeFiles/safety.dir/examples/safety.c.s

# Object files for target safety
safety_OBJECTS = \
"CMakeFiles/safety.dir/examples/safety.c.o"

# External object files for target safety
safety_EXTERNAL_OBJECTS =

safety: CMakeFiles/safety.dir/examples/safety.c.o
safety: CMakeFiles/safety.dir/build.make
safety: CMakeFiles/safety.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/emulated/0/GitProjects/rhc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable safety"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/safety.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/safety.dir/build: safety
.PHONY : CMakeFiles/safety.dir/build

CMakeFiles/safety.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/safety.dir/cmake_clean.cmake
.PHONY : CMakeFiles/safety.dir/clean

CMakeFiles/safety.dir/depend:
	cd /storage/emulated/0/GitProjects/rhc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/emulated/0/GitProjects/rhc /storage/emulated/0/GitProjects/rhc /storage/emulated/0/GitProjects/rhc /storage/emulated/0/GitProjects/rhc /storage/emulated/0/GitProjects/rhc/CMakeFiles/safety.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/safety.dir/depend
