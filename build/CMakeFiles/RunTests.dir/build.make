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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.1/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build

# Include any dependencies generated for this target.
include CMakeFiles/RunTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RunTests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RunTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RunTests.dir/flags.make

CMakeFiles/RunTests.dir/tests/test.cpp.o: CMakeFiles/RunTests.dir/flags.make
CMakeFiles/RunTests.dir/tests/test.cpp.o: /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/tests/test.cpp
CMakeFiles/RunTests.dir/tests/test.cpp.o: CMakeFiles/RunTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RunTests.dir/tests/test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RunTests.dir/tests/test.cpp.o -MF CMakeFiles/RunTests.dir/tests/test.cpp.o.d -o CMakeFiles/RunTests.dir/tests/test.cpp.o -c /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/tests/test.cpp

CMakeFiles/RunTests.dir/tests/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RunTests.dir/tests/test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/tests/test.cpp > CMakeFiles/RunTests.dir/tests/test.cpp.i

CMakeFiles/RunTests.dir/tests/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RunTests.dir/tests/test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/tests/test.cpp -o CMakeFiles/RunTests.dir/tests/test.cpp.s

CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o: CMakeFiles/RunTests.dir/flags.make
CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o: /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/MessageComposer.cpp
CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o: CMakeFiles/RunTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o -MF CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o.d -o CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o -c /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/MessageComposer.cpp

CMakeFiles/RunTests.dir/src/MessageComposer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RunTests.dir/src/MessageComposer.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/MessageComposer.cpp > CMakeFiles/RunTests.dir/src/MessageComposer.cpp.i

CMakeFiles/RunTests.dir/src/MessageComposer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RunTests.dir/src/MessageComposer.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/MessageComposer.cpp -o CMakeFiles/RunTests.dir/src/MessageComposer.cpp.s

CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o: CMakeFiles/RunTests.dir/flags.make
CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o: /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/FracturedMessage.cpp
CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o: CMakeFiles/RunTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o -MF CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o.d -o CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o -c /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/FracturedMessage.cpp

CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/FracturedMessage.cpp > CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.i

CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/FracturedMessage.cpp -o CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.s

CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o: CMakeFiles/RunTests.dir/flags.make
CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o: /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/CommunicationChannel.cpp
CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o: CMakeFiles/RunTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o -MF CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o.d -o CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o -c /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/CommunicationChannel.cpp

CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/CommunicationChannel.cpp > CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.i

CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/src/CommunicationChannel.cpp -o CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.s

# Object files for target RunTests
RunTests_OBJECTS = \
"CMakeFiles/RunTests.dir/tests/test.cpp.o" \
"CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o" \
"CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o" \
"CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o"

# External object files for target RunTests
RunTests_EXTERNAL_OBJECTS =

RunTests: CMakeFiles/RunTests.dir/tests/test.cpp.o
RunTests: CMakeFiles/RunTests.dir/src/MessageComposer.cpp.o
RunTests: CMakeFiles/RunTests.dir/src/FracturedMessage.cpp.o
RunTests: CMakeFiles/RunTests.dir/src/CommunicationChannel.cpp.o
RunTests: CMakeFiles/RunTests.dir/build.make
RunTests: /opt/homebrew/lib/libssl.dylib
RunTests: /opt/homebrew/lib/libcrypto.dylib
RunTests: CMakeFiles/RunTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable RunTests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RunTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RunTests.dir/build: RunTests
.PHONY : CMakeFiles/RunTests.dir/build

CMakeFiles/RunTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RunTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RunTests.dir/clean

CMakeFiles/RunTests.dir/depend:
	cd /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build /Users/prathameshwalunj/Desktop/SubmarineCryProject/secure-communications-underwater/build/CMakeFiles/RunTests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/RunTests.dir/depend

