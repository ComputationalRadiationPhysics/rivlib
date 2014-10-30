#
# RIV Root CMakeLists
#
cmake_minimum_required(VERSION 2.8)

project(riv)
set(BUILDING_RIV_PROJECT 1)
#set(BUILD_TESTS 1)


#set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(GLUT)


# some manual external dependancies
set(THELIB_DIR /home/grottel/dev/insituvolvis/libs/thelib++/)
set(VISLIB_DIR /home/grottel/dev/insituvolvis/libs/vislib/)
if (BUILD_TESTS)
    set(ANTTWEAKBAR_DIR /home/grottel/dev/insituvolvis/libs/AntTweakBar/)
endif()


set(THELIB_INCLUDE_DIRS ${THELIB_DIR}include/)
set(THELIB_LIBRARIES ${THELIB_DIR}build/)
set(VISLIB_INCLUDE_DIRS
	${VISLIB_DIR}base/include/
	${VISLIB_DIR}sys/include/
	${VISLIB_DIR}net/include/
	${VISLIB_DIR}math/include/
	${VISLIB_DIR}graphics/include/
	)
set(VISLIB_LIBRARIES ${VISLIB_DIR}lib/
	${VISLIB_DIR}build/base/
	${VISLIB_DIR}build/sys/
	${VISLIB_DIR}build/net/
	${VISLIB_DIR}build/math/
	${VISLIB_DIR}build/graphics/
	)


# processor word size detection
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)


# libraries required by rivlib and tests
find_library(THELIB thelib ${THELIB_LIBRARIES})
find_library(VISLIB_BASE vislibbase${BITS} ${VISLIB_LIBRARIES})
find_library(VISLIB_SYS vislibsys${BITS} ${VISLIB_LIBRARIES})
find_library(VISLIB_NET vislibnet${BITS} ${VISLIB_LIBRARIES})
find_library(VISLIB_MATH vislibmath${BITS} ${VISLIB_LIBRARIES})
find_library(VISLIB_GRAPHICS vislibgraphics${BITS} ${VISLIB_LIBRARIES})

# actual projects
add_subdirectory(rivlib)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

