# Test/TheTest/CMakeLists.txt
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

cmake_minimum_required(VERSION 2.8)

# This is a dependent project on thelib and cannot be compiled
# alone. The CMake way would be to create a CMake-Find-Script for
# TheLib that this test can find and use to determine the position
# of the library files. This will be done if the library is in a
# more mature state. For now, this make file can only be used as
# a project of the TheLib solution which is defined in the root target.

# Check if the thelib target (the library) is defined and fail if not.
if (NOT TARGET thelib)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()

# Find header files
file(GLOB_RECURSE test_header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
# Find source files
file(GLOB_RECURSE test_source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
# Exclude the following files
list(REMOVE_ITEM test_source_files
	"testmultisz.cpp"
	)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG -D_DEBUG -ggdb")

# Set the include directory to point to the include directory
include_directories("../../include")


# Set the target
add_executable(thetest ${test_header_files} ${test_source_files})

# Link thelib to thetest
target_link_libraries(thetest thelib)
