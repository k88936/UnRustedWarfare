# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/CLionProjects/tmxlite

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/CLionProjects/tmxlite

# Include any dependencies generated for this target.
include tmxlite/CMakeFiles/tmxlite.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.make

# Include the progress variables for this target.
include tmxlite/CMakeFiles/tmxlite.dir/progress.make

# Include the compile flags for this target's objects.
include tmxlite/CMakeFiles/tmxlite.dir/flags.make

tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o: tmxlite/src/FreeFuncs.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o -MF CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o.d -o CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/FreeFuncs.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/FreeFuncs.cpp > CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/FreeFuncs.cpp -o CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o: tmxlite/src/ImageLayer.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o -MF CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o.d -o CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/ImageLayer.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/ImageLayer.cpp > CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/ImageLayer.cpp -o CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.o: tmxlite/src/Map.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.o -MF CMakeFiles/tmxlite.dir/src/Map.cpp.o.d -o CMakeFiles/tmxlite.dir/src/Map.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/Map.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/Map.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/Map.cpp > CMakeFiles/tmxlite.dir/src/Map.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/Map.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/Map.cpp -o CMakeFiles/tmxlite.dir/src/Map.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.o: tmxlite/src/Object.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.o -MF CMakeFiles/tmxlite.dir/src/Object.cpp.o.d -o CMakeFiles/tmxlite.dir/src/Object.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/Object.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/Object.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/Object.cpp > CMakeFiles/tmxlite.dir/src/Object.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/Object.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/Object.cpp -o CMakeFiles/tmxlite.dir/src/Object.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o: tmxlite/src/ObjectGroup.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o -MF CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o.d -o CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/ObjectGroup.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/ObjectGroup.cpp > CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/ObjectGroup.cpp -o CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.o: tmxlite/src/Property.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.o -MF CMakeFiles/tmxlite.dir/src/Property.cpp.o.d -o CMakeFiles/tmxlite.dir/src/Property.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/Property.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/Property.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/Property.cpp > CMakeFiles/tmxlite.dir/src/Property.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/Property.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/Property.cpp -o CMakeFiles/tmxlite.dir/src/Property.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o: tmxlite/src/TileLayer.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o -MF CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o.d -o CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/TileLayer.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/TileLayer.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/TileLayer.cpp > CMakeFiles/tmxlite.dir/src/TileLayer.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/TileLayer.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/TileLayer.cpp -o CMakeFiles/tmxlite.dir/src/TileLayer.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o: tmxlite/src/LayerGroup.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o -MF CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o.d -o CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/LayerGroup.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/LayerGroup.cpp > CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/LayerGroup.cpp -o CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.o: tmxlite/src/Tileset.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.o -MF CMakeFiles/tmxlite.dir/src/Tileset.cpp.o.d -o CMakeFiles/tmxlite.dir/src/Tileset.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/Tileset.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/Tileset.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/Tileset.cpp > CMakeFiles/tmxlite.dir/src/Tileset.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/Tileset.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/Tileset.cpp -o CMakeFiles/tmxlite.dir/src/Tileset.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o: tmxlite/src/ObjectTypes.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o -MF CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o.d -o CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/ObjectTypes.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/ObjectTypes.cpp > CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/ObjectTypes.cpp -o CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.s

tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.o: tmxlite/src/miniz.c
tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.o -MF CMakeFiles/tmxlite.dir/src/miniz.c.o.d -o CMakeFiles/tmxlite.dir/src/miniz.c.o -c /root/CLionProjects/tmxlite/tmxlite/src/miniz.c

tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tmxlite.dir/src/miniz.c.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/miniz.c > CMakeFiles/tmxlite.dir/src/miniz.c.i

tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tmxlite.dir/src/miniz.c.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/miniz.c -o CMakeFiles/tmxlite.dir/src/miniz.c.s

tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/flags.make
tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o: tmxlite/src/detail/pugixml.cpp
tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o: tmxlite/CMakeFiles/tmxlite.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o -MF CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o.d -o CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o -c /root/CLionProjects/tmxlite/tmxlite/src/detail/pugixml.cpp

tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.i"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CLionProjects/tmxlite/tmxlite/src/detail/pugixml.cpp > CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.i

tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.s"
	cd /root/CLionProjects/tmxlite/tmxlite && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CLionProjects/tmxlite/tmxlite/src/detail/pugixml.cpp -o CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.s

# Object files for target tmxlite
tmxlite_OBJECTS = \
"CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o" \
"CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o" \
"CMakeFiles/tmxlite.dir/src/Map.cpp.o" \
"CMakeFiles/tmxlite.dir/src/Object.cpp.o" \
"CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o" \
"CMakeFiles/tmxlite.dir/src/Property.cpp.o" \
"CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o" \
"CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o" \
"CMakeFiles/tmxlite.dir/src/Tileset.cpp.o" \
"CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o" \
"CMakeFiles/tmxlite.dir/src/miniz.c.o" \
"CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o"

# External object files for target tmxlite
tmxlite_EXTERNAL_OBJECTS =

tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/FreeFuncs.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/ImageLayer.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/Map.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/Object.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/ObjectGroup.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/Property.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/TileLayer.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/LayerGroup.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/Tileset.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/ObjectTypes.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/miniz.c.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/src/detail/pugixml.cpp.o
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/build.make
tmxlite/libtmxlite-d.so: tmxlite/CMakeFiles/tmxlite.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/root/CLionProjects/tmxlite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX shared library libtmxlite-d.so"
	cd /root/CLionProjects/tmxlite/tmxlite && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tmxlite.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tmxlite/CMakeFiles/tmxlite.dir/build: tmxlite/libtmxlite-d.so
.PHONY : tmxlite/CMakeFiles/tmxlite.dir/build

tmxlite/CMakeFiles/tmxlite.dir/clean:
	cd /root/CLionProjects/tmxlite/tmxlite && $(CMAKE_COMMAND) -P CMakeFiles/tmxlite.dir/cmake_clean.cmake
.PHONY : tmxlite/CMakeFiles/tmxlite.dir/clean

tmxlite/CMakeFiles/tmxlite.dir/depend:
	cd /root/CLionProjects/tmxlite && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/CLionProjects/tmxlite /root/CLionProjects/tmxlite/tmxlite /root/CLionProjects/tmxlite /root/CLionProjects/tmxlite/tmxlite /root/CLionProjects/tmxlite/tmxlite/CMakeFiles/tmxlite.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tmxlite/CMakeFiles/tmxlite.dir/depend
