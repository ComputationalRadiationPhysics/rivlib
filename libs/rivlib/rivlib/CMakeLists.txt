#
# RIVlib CMakeLists
#
cmake_minimum_required(VERSION 2.8)
# Check if project is riv target
if (NOT DEFINED BUILDING_RIV_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()

#dependancies
find_package(ZLIB REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)

#input file
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.h")
file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")

# include directories
include_directories("include" "src"
	${THELIB_INCLUDE_DIRS}
	${VISLIB_INCLUDE_DIRS}
	${ZLIB_INCLUDE_DIRS}
	)

# compiler options
add_definitions(-std=c++0x -pedantic -fPIC -DRIVLIB_EXPORTS=1 -DUNIX)

# target definition
add_library(rivlib SHARED ${header_files} ${source_files})
#add_library(rivlib STATIC ${header_files} ${source_files})
target_link_libraries(rivlib
	${THELIB}
	${VISLIB_BASE}
	${VISLIB_SYS}
	${VISLIB_NET}
	${VISLIB_MATH}
	${VISLIB_GRAPHICS}
	${ZLIB_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT} -lrt)

