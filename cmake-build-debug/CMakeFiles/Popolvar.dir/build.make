# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_SOURCE_DIR = /home/michal/CLionProjects/Popolvar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michal/CLionProjects/Popolvar/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Popolvar.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Popolvar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Popolvar.dir/flags.make

CMakeFiles/Popolvar.dir/main.c.o: CMakeFiles/Popolvar.dir/flags.make
CMakeFiles/Popolvar.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michal/CLionProjects/Popolvar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Popolvar.dir/main.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Popolvar.dir/main.c.o   -c /home/michal/CLionProjects/Popolvar/main.c

CMakeFiles/Popolvar.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Popolvar.dir/main.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/michal/CLionProjects/Popolvar/main.c > CMakeFiles/Popolvar.dir/main.c.i

CMakeFiles/Popolvar.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Popolvar.dir/main.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/michal/CLionProjects/Popolvar/main.c -o CMakeFiles/Popolvar.dir/main.c.s

# Object files for target Popolvar
Popolvar_OBJECTS = \
"CMakeFiles/Popolvar.dir/main.c.o"

# External object files for target Popolvar
Popolvar_EXTERNAL_OBJECTS =

Popolvar: CMakeFiles/Popolvar.dir/main.c.o
Popolvar: CMakeFiles/Popolvar.dir/build.make
Popolvar: CMakeFiles/Popolvar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/michal/CLionProjects/Popolvar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Popolvar"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Popolvar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Popolvar.dir/build: Popolvar

.PHONY : CMakeFiles/Popolvar.dir/build

CMakeFiles/Popolvar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Popolvar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Popolvar.dir/clean

CMakeFiles/Popolvar.dir/depend:
	cd /home/michal/CLionProjects/Popolvar/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michal/CLionProjects/Popolvar /home/michal/CLionProjects/Popolvar /home/michal/CLionProjects/Popolvar/cmake-build-debug /home/michal/CLionProjects/Popolvar/cmake-build-debug /home/michal/CLionProjects/Popolvar/cmake-build-debug/CMakeFiles/Popolvar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Popolvar.dir/depend

