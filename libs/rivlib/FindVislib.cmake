#
# Try to find most parts of the vislib
#
# When found sets:
#
#  VISLIB_INCLUDE_DIRS
#  VISLIB_BASE
#  VISLIB_SYS
#  VISLIB_MATH
#  VISLIB_NET
#  VISLIB_GRAPHICS
#  VISLIB_GL
#
cmake_minimum_required(VERSION 2.8)

message(STATUS "Looking for vislib")

# processor word size detection
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)


# paths to search in
set(VISLIB_SEARCH_PATHS
	$ENV{VISLIB_ROOT} 
	${CMAKE_SOURCE_DIR}/../vislib
	)

set(LIB_PATH_SUFFIXES lib lib64 build)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(LIB_PATH_SUFFIXES ${LIB_PATH_SUFFIXES} debug release)
else()
	set(LIB_PATH_SUFFIXES ${LIB_PATH_SUFFIXES} release debug)
endif()


# search for sublibrary roots
FIND_PATH(VISLIB_BASE_PATH include/vislib/assert.h PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES base)
if (VISLIB_BASE_PATH)
	message(STATUS "Looking for vislib - base include - found")
	set(VISLIB_BASE_INCLUDE ${VISLIB_BASE_PATH}/include)
else()
	message(STATUS "Looking for vislib - base include - NOT FOUND")
endif()
FIND_PATH(VISLIB_MATH_PATH include/vislib/AbstractCuboid.h PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES math)
if (VISLIB_MATH_PATH)
	message(STATUS "Looking for vislib - math include - found")
	set(VISLIB_MATH_INCLUDE ${VISLIB_MATH_PATH}/include)
else()
	message(STATUS "Looking for vislib - math include - NOT FOUND")
endif()
FIND_PATH(VISLIB_SYS_PATH include/vislib/File.h PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES sys)
if (VISLIB_SYS_PATH)
	message(STATUS "Looking for vislib - sys include - found")
	set(VISLIB_SYS_INCLUDE ${VISLIB_SYS_PATH}/include)
else()
	message(STATUS "Looking for vislib - sys include - NOT FOUND")
endif()
FIND_PATH(VISLIB_NET_PATH include/vislib/TcpCommChannel.h PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES net)
if (VISLIB_NET_PATH)
	message(STATUS "Looking for vislib - net include - found")
	set(VISLIB_NET_INCLUDE ${VISLIB_NET_PATH}/include)
else()
	message(STATUS "Looking for vislib - net include - NOT FOUND")
endif()
FIND_PATH(VISLIB_GRAPHICS_PATH include/vislib/NamedColours.h PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES graphics)
if (VISLIB_GRAPHICS_PATH)
	message(STATUS "Looking for vislib - graphics include - found")
	set(VISLIB_GRAPHICS_INCLUDE ${VISLIB_GRAPHICS_PATH}/include)
else()
	message(STATUS "Looking for vislib - graphics include - NOT FOUND")
endif()
FIND_PATH(VISLIB_GL_PATH include/vislib/CameraOpenGL.h PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES gl)
if (VISLIB_GL_PATH)
	message(STATUS "Looking for vislib - gl include - found")
	set(VISLIB_GL_INCLUDE ${VISLIB_GL_PATH}/include)
else()
	message(STATUS "Looking for vislib - gl include - NOT FOUND")
endif()


# include directories
set(VISLIB_INCLUDE_DIRS
	${VISLIB_BASE_INCLUDE}
	${VISLIB_MATH_INCLUDE}
	${VISLIB_SYS_INCLUDE}
	${VISLIB_NET_INCLUDE}
	${VISLIB_GRAPHICS_INCLUDE}
	)
if (VISLIB_GL_PATH)
	set(VISLIB_INCLUDE_DIRS ${VISLIB_INCLUDE_DIRS} ${VISLIB_GL_INCLUDE})
endif()


# libraries
set(LIB_PSUF)
foreach(DIR ${LIB_PATH_SUFFIXES})
	set(LIB_PSUF ${LIB_PSUF} ${DIR}/base)
endforeach()
find_library(VISLIB_BASE vislibbase${BITS} PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES ${LIB_PSUF})
if (VISLIB_BASE)
	message(STATUS "Looking for vislib - base lib - found")
else()
	message(STATUS "Looking for vislib - base lib - NOT FOUND")
endif()

set(LIB_PSUF)
foreach(DIR ${LIB_PATH_SUFFIXES})
	set(LIB_PSUF ${LIB_PSUF} ${DIR}/math)
endforeach()
find_library(VISLIB_MATH vislibmath${BITS} PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES ${LIB_PSUF})
if (VISLIB_MATH)
	message(STATUS "Looking for vislib - math lib - found")
else()
	message(STATUS "Looking for vislib - math lib - NOT FOUND")
endif()

set(LIB_PSUF)
foreach(DIR ${LIB_PATH_SUFFIXES})
	set(LIB_PSUF ${LIB_PSUF} ${DIR}/sys)
endforeach()
find_library(VISLIB_SYS vislibsys${BITS} PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES ${LIB_PSUF})
if (VISLIB_SYS)
	message(STATUS "Looking for vislib - math sys - found")
else()
	message(STATUS "Looking for vislib - math sys - NOT FOUND")
endif()

set(LIB_PSUF)
foreach(DIR ${LIB_PATH_SUFFIXES})
	set(LIB_PSUF ${LIB_PSUF} ${DIR}/net)
endforeach()
find_library(VISLIB_NET vislibnet${BITS} PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES ${LIB_PSUF})
if (VISLIB_NET)
	message(STATUS "Looking for vislib - math net - found")
else()
	message(STATUS "Looking for vislib - math net - NOT FOUND")
endif()

set(LIB_PSUF)
foreach(DIR ${LIB_PATH_SUFFIXES})
	set(LIB_PSUF ${LIB_PSUF} ${DIR}/graphics)
endforeach()
find_library(VISLIB_GRAPHICS vislibgraphics${BITS} PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES ${LIB_PSUF})
if (VISLIB_GRAPHICS)
	message(STATUS "Looking for vislib - math graphics - found")
else()
	message(STATUS "Looking for vislib - math graphics - NOT FOUND")
endif()

set(LIB_PSUF)
foreach(DIR ${LIB_PATH_SUFFIXES})
	set(LIB_PSUF ${LIB_PSUF} ${DIR}/gl)
endforeach()
find_library(VISLIB_GL vislibgl${BITS} PATHS ${VISLIB_SEARCH_PATHS} PATH_SUFFIXES ${LIB_PSUF})
if (VISLIB_GL)
	message(STATUS "Looking for vislib - math gl - found")
else()
	message(STATUS "Looking for vislib - math gl - NOT FOUND")
endif()

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vislib DEFAULT_MSG VISLIB_BASE VISLIB_MATH VISLIB_SYS VISLIB_NET VISLIB_GRAPHICS VISLIB_INCLUDE_DIRS)

