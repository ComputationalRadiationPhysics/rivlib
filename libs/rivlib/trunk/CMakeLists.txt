#
# RIV Root CMakeLists
#
cmake_minimum_required(VERSION 2.8)

project(riv)
set(BUILDING_RIV_PROJECT 1)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# some manual external dependancies
set(THELIB_DIR /home/schnei49/libs/thelib/thelib++/)
set(VISLIB_DIR /home/schnei49/libs/vislib/)
set(VISGLUT_DIR /home/schnei49/lib/visglut/)
#set(ANTTWEAKBAR_DIR /home/vicci/dev.sgrottel/atb/)

set(THELIB_INCLUDE_DIRS ${THELIB_DIR}include/)
set(THELIB_LIBRARIES ${THELIB_DIR}build/)
set(VISLIB_INCLUDE_DIRS
	${VISLIB_DIR}base/include/
	${VISLIB_DIR}sys/include/
	${VISLIB_DIR}net/include/
	${VISLIB_DIR}math/include/
	${VISLIB_DIR}graphics/include/
	${VISLIB_DIR}gl/include/
	)
set(VISLIB_LIBRARIES ${VISLIB_DIR}lib/)
set(VISGLUT_INCLUDE_DIRS ${VISGLUT_DIR}include/)
set(VISGLUT_LIBRARIES ${VISGLUT_DIR}lib/)
#set(ANTTWEAKBAR_INCLUDE_DIRS ${ANTTWEAKBAR_DIR}include/)
#set(ANTTWEAKBAR_LIBRARIES ${ANTTWEAKBAR_DIR}lib/)

# gack
set(BITS 64)

# libraries required by rivlib and tests
find_library(THELIB thelib ${THELIB_LIBRARIES})
find_library(VISLIB_BASE vislibbase${BITS}d ${VISLIB_LIBRARIES})
find_library(VISLIB_SYS vislibsys${BITS}d ${VISLIB_LIBRARIES})
find_library(VISLIB_NET vislibnet${BITS}d ${VISLIB_LIBRARIES})
find_library(VISLIB_MATH vislibmath${BITS}d ${VISLIB_LIBRARIES})
find_library(VISLIB_GRAPHICS vislibgraphics${BITS}d ${VISLIB_LIBRARIES})
find_library(VISLIB_GL vislibgl${BITS}d ${VISLIB_LIBRARIES})

# actual projects
add_subdirectory(rivlib)
#add_subdirectory(tests)

