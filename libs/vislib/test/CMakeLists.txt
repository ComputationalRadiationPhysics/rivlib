#
# Vislib test
#
cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED BUILDING_VISLIB_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


#input file
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

# include directories
include_directories("include" "src"
	"../base/include"
	"../math/include"
	"../sys/include"
	"../net/include"
	"../graphics/include"
	)

set(CMAKE_THREAD_PREFER_PTHREAD)


# target definition
add_executable(vislibtest ${header_files} ${source_files})
target_link_libraries(vislibtest vislibgraphics${BITS} vislibnet${BITS} vislibsys${BITS} vislibmath${BITS} vislibbase${BITS})

