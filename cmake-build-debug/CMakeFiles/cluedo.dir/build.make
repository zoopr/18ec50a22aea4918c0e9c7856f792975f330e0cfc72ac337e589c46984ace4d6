# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.9

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.2.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.2.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Mat\CLionProjects\cluedo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cluedo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cluedo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cluedo.dir/flags.make

CMakeFiles/cluedo.dir/main.c.obj: CMakeFiles/cluedo.dir/flags.make
CMakeFiles/cluedo.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cluedo.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cluedo.dir\main.c.obj   -c C:\Users\Mat\CLionProjects\cluedo\main.c

CMakeFiles/cluedo.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cluedo.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mat\CLionProjects\cluedo\main.c > CMakeFiles\cluedo.dir\main.c.i

CMakeFiles/cluedo.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cluedo.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mat\CLionProjects\cluedo\main.c -o CMakeFiles\cluedo.dir\main.c.s

CMakeFiles/cluedo.dir/main.c.obj.requires:

.PHONY : CMakeFiles/cluedo.dir/main.c.obj.requires

CMakeFiles/cluedo.dir/main.c.obj.provides: CMakeFiles/cluedo.dir/main.c.obj.requires
	$(MAKE) -f CMakeFiles\cluedo.dir\build.make CMakeFiles/cluedo.dir/main.c.obj.provides.build
.PHONY : CMakeFiles/cluedo.dir/main.c.obj.provides

CMakeFiles/cluedo.dir/main.c.obj.provides.build: CMakeFiles/cluedo.dir/main.c.obj


CMakeFiles/cluedo.dir/BoardManager.c.obj: CMakeFiles/cluedo.dir/flags.make
CMakeFiles/cluedo.dir/BoardManager.c.obj: ../BoardManager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/cluedo.dir/BoardManager.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cluedo.dir\BoardManager.c.obj   -c C:\Users\Mat\CLionProjects\cluedo\BoardManager.c

CMakeFiles/cluedo.dir/BoardManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cluedo.dir/BoardManager.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mat\CLionProjects\cluedo\BoardManager.c > CMakeFiles\cluedo.dir\BoardManager.c.i

CMakeFiles/cluedo.dir/BoardManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cluedo.dir/BoardManager.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mat\CLionProjects\cluedo\BoardManager.c -o CMakeFiles\cluedo.dir\BoardManager.c.s

CMakeFiles/cluedo.dir/BoardManager.c.obj.requires:

.PHONY : CMakeFiles/cluedo.dir/BoardManager.c.obj.requires

CMakeFiles/cluedo.dir/BoardManager.c.obj.provides: CMakeFiles/cluedo.dir/BoardManager.c.obj.requires
	$(MAKE) -f CMakeFiles\cluedo.dir\build.make CMakeFiles/cluedo.dir/BoardManager.c.obj.provides.build
.PHONY : CMakeFiles/cluedo.dir/BoardManager.c.obj.provides

CMakeFiles/cluedo.dir/BoardManager.c.obj.provides.build: CMakeFiles/cluedo.dir/BoardManager.c.obj


CMakeFiles/cluedo.dir/Pieces.c.obj: CMakeFiles/cluedo.dir/flags.make
CMakeFiles/cluedo.dir/Pieces.c.obj: ../Pieces.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/cluedo.dir/Pieces.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cluedo.dir\Pieces.c.obj   -c C:\Users\Mat\CLionProjects\cluedo\Pieces.c

CMakeFiles/cluedo.dir/Pieces.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cluedo.dir/Pieces.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mat\CLionProjects\cluedo\Pieces.c > CMakeFiles\cluedo.dir\Pieces.c.i

CMakeFiles/cluedo.dir/Pieces.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cluedo.dir/Pieces.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mat\CLionProjects\cluedo\Pieces.c -o CMakeFiles\cluedo.dir\Pieces.c.s

CMakeFiles/cluedo.dir/Pieces.c.obj.requires:

.PHONY : CMakeFiles/cluedo.dir/Pieces.c.obj.requires

CMakeFiles/cluedo.dir/Pieces.c.obj.provides: CMakeFiles/cluedo.dir/Pieces.c.obj.requires
	$(MAKE) -f CMakeFiles\cluedo.dir\build.make CMakeFiles/cluedo.dir/Pieces.c.obj.provides.build
.PHONY : CMakeFiles/cluedo.dir/Pieces.c.obj.provides

CMakeFiles/cluedo.dir/Pieces.c.obj.provides.build: CMakeFiles/cluedo.dir/Pieces.c.obj


CMakeFiles/cluedo.dir/Standards.c.obj: CMakeFiles/cluedo.dir/flags.make
CMakeFiles/cluedo.dir/Standards.c.obj: ../Standards.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/cluedo.dir/Standards.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cluedo.dir\Standards.c.obj   -c C:\Users\Mat\CLionProjects\cluedo\Standards.c

CMakeFiles/cluedo.dir/Standards.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cluedo.dir/Standards.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mat\CLionProjects\cluedo\Standards.c > CMakeFiles\cluedo.dir\Standards.c.i

CMakeFiles/cluedo.dir/Standards.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cluedo.dir/Standards.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mat\CLionProjects\cluedo\Standards.c -o CMakeFiles\cluedo.dir\Standards.c.s

CMakeFiles/cluedo.dir/Standards.c.obj.requires:

.PHONY : CMakeFiles/cluedo.dir/Standards.c.obj.requires

CMakeFiles/cluedo.dir/Standards.c.obj.provides: CMakeFiles/cluedo.dir/Standards.c.obj.requires
	$(MAKE) -f CMakeFiles\cluedo.dir\build.make CMakeFiles/cluedo.dir/Standards.c.obj.provides.build
.PHONY : CMakeFiles/cluedo.dir/Standards.c.obj.provides

CMakeFiles/cluedo.dir/Standards.c.obj.provides.build: CMakeFiles/cluedo.dir/Standards.c.obj


CMakeFiles/cluedo.dir/Gameplay.c.obj: CMakeFiles/cluedo.dir/flags.make
CMakeFiles/cluedo.dir/Gameplay.c.obj: ../Gameplay.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/cluedo.dir/Gameplay.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cluedo.dir\Gameplay.c.obj   -c C:\Users\Mat\CLionProjects\cluedo\Gameplay.c

CMakeFiles/cluedo.dir/Gameplay.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cluedo.dir/Gameplay.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mat\CLionProjects\cluedo\Gameplay.c > CMakeFiles\cluedo.dir\Gameplay.c.i

CMakeFiles/cluedo.dir/Gameplay.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cluedo.dir/Gameplay.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mat\CLionProjects\cluedo\Gameplay.c -o CMakeFiles\cluedo.dir\Gameplay.c.s

CMakeFiles/cluedo.dir/Gameplay.c.obj.requires:

.PHONY : CMakeFiles/cluedo.dir/Gameplay.c.obj.requires

CMakeFiles/cluedo.dir/Gameplay.c.obj.provides: CMakeFiles/cluedo.dir/Gameplay.c.obj.requires
	$(MAKE) -f CMakeFiles\cluedo.dir\build.make CMakeFiles/cluedo.dir/Gameplay.c.obj.provides.build
.PHONY : CMakeFiles/cluedo.dir/Gameplay.c.obj.provides

CMakeFiles/cluedo.dir/Gameplay.c.obj.provides.build: CMakeFiles/cluedo.dir/Gameplay.c.obj


CMakeFiles/cluedo.dir/AI.c.obj: CMakeFiles/cluedo.dir/flags.make
CMakeFiles/cluedo.dir/AI.c.obj: ../AI.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/cluedo.dir/AI.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cluedo.dir\AI.c.obj   -c C:\Users\Mat\CLionProjects\cluedo\AI.c

CMakeFiles/cluedo.dir/AI.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cluedo.dir/AI.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mat\CLionProjects\cluedo\AI.c > CMakeFiles\cluedo.dir\AI.c.i

CMakeFiles/cluedo.dir/AI.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cluedo.dir/AI.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mat\CLionProjects\cluedo\AI.c -o CMakeFiles\cluedo.dir\AI.c.s

CMakeFiles/cluedo.dir/AI.c.obj.requires:

.PHONY : CMakeFiles/cluedo.dir/AI.c.obj.requires

CMakeFiles/cluedo.dir/AI.c.obj.provides: CMakeFiles/cluedo.dir/AI.c.obj.requires
	$(MAKE) -f CMakeFiles\cluedo.dir\build.make CMakeFiles/cluedo.dir/AI.c.obj.provides.build
.PHONY : CMakeFiles/cluedo.dir/AI.c.obj.provides

CMakeFiles/cluedo.dir/AI.c.obj.provides.build: CMakeFiles/cluedo.dir/AI.c.obj


# Object files for target cluedo
cluedo_OBJECTS = \
"CMakeFiles/cluedo.dir/main.c.obj" \
"CMakeFiles/cluedo.dir/BoardManager.c.obj" \
"CMakeFiles/cluedo.dir/Pieces.c.obj" \
"CMakeFiles/cluedo.dir/Standards.c.obj" \
"CMakeFiles/cluedo.dir/Gameplay.c.obj" \
"CMakeFiles/cluedo.dir/AI.c.obj"

# External object files for target cluedo
cluedo_EXTERNAL_OBJECTS =

cluedo.exe: CMakeFiles/cluedo.dir/main.c.obj
cluedo.exe: CMakeFiles/cluedo.dir/BoardManager.c.obj
cluedo.exe: CMakeFiles/cluedo.dir/Pieces.c.obj
cluedo.exe: CMakeFiles/cluedo.dir/Standards.c.obj
cluedo.exe: CMakeFiles/cluedo.dir/Gameplay.c.obj
cluedo.exe: CMakeFiles/cluedo.dir/AI.c.obj
cluedo.exe: CMakeFiles/cluedo.dir/build.make
cluedo.exe: CMakeFiles/cluedo.dir/linklibs.rsp
cluedo.exe: CMakeFiles/cluedo.dir/objects1.rsp
cluedo.exe: CMakeFiles/cluedo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable cluedo.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\cluedo.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cluedo.dir/build: cluedo.exe

.PHONY : CMakeFiles/cluedo.dir/build

CMakeFiles/cluedo.dir/requires: CMakeFiles/cluedo.dir/main.c.obj.requires
CMakeFiles/cluedo.dir/requires: CMakeFiles/cluedo.dir/BoardManager.c.obj.requires
CMakeFiles/cluedo.dir/requires: CMakeFiles/cluedo.dir/Pieces.c.obj.requires
CMakeFiles/cluedo.dir/requires: CMakeFiles/cluedo.dir/Standards.c.obj.requires
CMakeFiles/cluedo.dir/requires: CMakeFiles/cluedo.dir/Gameplay.c.obj.requires
CMakeFiles/cluedo.dir/requires: CMakeFiles/cluedo.dir/AI.c.obj.requires

.PHONY : CMakeFiles/cluedo.dir/requires

CMakeFiles/cluedo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\cluedo.dir\cmake_clean.cmake
.PHONY : CMakeFiles/cluedo.dir/clean

CMakeFiles/cluedo.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Mat\CLionProjects\cluedo C:\Users\Mat\CLionProjects\cluedo C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug C:\Users\Mat\CLionProjects\cluedo\cmake-build-debug\CMakeFiles\cluedo.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cluedo.dir/depend
