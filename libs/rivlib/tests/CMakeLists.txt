#
# RIV Tests CMakeLists
#
cmake_minimum_required(VERSION 2.8)

# Check if project is riv target
if (NOT DEFINED BUILDING_RIV_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


# dependancies required by the tests
set(VISLIB_INCLUDE_DIRS ${VISLIB_INCLUDE_DIRS} ${VISLIB_DIR}gl/include/ )
set(VISLIB_LIBRARIES ${VISLIB_LIBRARIES} ${VISLIB_DIR}build/gl/ )
find_library(VISLIB_GL vislibgl${BITS} ${VISLIB_LIBRARIES})

set(ANTTWEAKBAR_INCLUDE_DIRS ${ANTTWEAKBAR_DIR}include/)
set(ANTTWEAKBAR_LIBRARIES ${ANTTWEAKBAR_DIR}lib/)
find_library(ANTTWEAKBAR AntTweakBar ${ANTTWEAKBAR_LIBRARIES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)


#actual tests
add_subdirectory(rivprovtest)
add_subdirectory(rivclnttest)

