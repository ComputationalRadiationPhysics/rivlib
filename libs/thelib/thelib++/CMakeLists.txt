# CMakeLists.txt
#
# Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# - Neither the name of TheLib, TheLib Team, nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Set minimum version to 2.8
cmake_minimum_required(VERSION 2.8)

# Definition of the project name
project(TheLib)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# Gather header files and store the result in the variable header_files
# Find the files relative to the directory of this cmake file
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.h")
# Exclude the following files. No warnings will be shown for non-existing files
# (The remove_file1.h and remove_file2.h are examples that shall be
#  deleted when real files must be excluded)
list(REMOVE_ITEM header_files 
	"include/remove_file1.h"
	"include/remove_file2.h")

# Gather source files and store the result in the variable source_files
file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")
# Exclude the following files
list(REMOVE_ITEM source_files
	"src/graphics/win32_window_behaviour.cpp"
	"src/graphics/directx/d2d_interop_surface.cpp"
	"src/graphics/directx/d3d11_rendering_behaviour.cpp"
	"src/graphics/directx/debug.cpp"
	"src/graphics/directx/dwrite_interop_surface.cpp"
	"src/graphics/directx/vertex_pos_base.cpp"
	"src/graphics/directx/vertex_pos_tex_base.cpp"
	"src/graphics/directx/vertex_xyz.cpp"
	"src/graphics/directx/vertex_xyzrs.cpp"
	"src/graphics/directx/vertex_xyzrst.cpp"
	"src/graphics/directx/vertex_xyzw.cpp"
	"src/system/net/simple_message_dispatcher.cpp"
	)


# Set the include directory to point to the subdirectory "include"
include_directories("include")


# Compiler defintions
add_definitions(-std=c++0x -pedantic -fPIC -DUNIX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG -D_DEBUG -ggdb")


# Set the library as static target
add_library(thelib STATIC ${header_files} ${source_files})


# additional common dependancies
target_link_libraries(thelib pthread rt)

# Add the test project as a dependent target
add_subdirectory(test)
