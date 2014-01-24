#
# Riv Provider Test CMakeLists
#
cmake_minimum_required(VERSION 2.8)
# Check if project is riv target
if (NOT DEFINED BUILDING_RIV_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()

#input file
file(GLOB header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "./*.h")
file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "./*.cpp")

# include directories
include_directories("../../rivlib/include"
	${THELIB_INCLUDE_DIRS}
	${VISLIB_INCLUDE_DIRS}
	${VISGLUT_INCLUDE_DIRS}
	${ANTTWEAKBAR_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	)

# compiler options
add_definitions(-std=c++0x -pedantic -fPIC -DUNIX)

# target definition
add_executable(rivprovtest ${header_files} ${source_files})
target_link_libraries(rivprovtest
	${RIVLIB}
	${THELIB}
	${VISLIB_BASE}
	${VISLIB_SYS}
	${VISLIB_NET}
	${VISLIB_MATH}
	${VISLIB_GRAPHICS}
	${VISLIB_GL}
	${VISGLUT}
	${ANTTWEAKBAR}
	${OPENGL_LIBRARIES}
	pthread rt)

