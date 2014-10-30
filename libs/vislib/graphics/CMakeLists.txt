#
# Vislib sys
#
cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED BUILDING_VISLIB_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


#input file
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.h")
file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")

# include directories
include_directories("include" "src"
	"../base/include"
	"../math/include"
	"../sys/include"
	)


# target definition
add_library(vislibgraphics${BITS} STATIC ${header_files} ${source_files})


