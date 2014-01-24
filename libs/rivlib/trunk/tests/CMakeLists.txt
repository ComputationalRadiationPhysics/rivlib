#
# RIV Tests CMakeLists
#
cmake_minimum_required(VERSION 2.8)
# Check if project is riv target
if (NOT DEFINED BUILDING_RIV_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()

# dependancies required by the tests
find_library(VISGLUT visglut${BITS} ${VISGLUT_LIBRARIES})
find_library(ANTTWEAKBAR AntTweakBar ${ANTTWEAKBAR_LIBRARIES})
find_package(OpenGL REQUIRED)
#find_library(RIVLIB rivlib ../rivlib)
set(RIVLIB ${PROJECT_BINARY_DIR}/rivlib/librivlib.so)

#actual tests
add_subdirectory(rivprovtest)
add_subdirectory(rivclnttest)

