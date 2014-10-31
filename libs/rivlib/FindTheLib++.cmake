#
# Try to find thelib++
#
#
#

# searching for this header is not very stable, but ok for now
FIND_PATH(THELIB_INCLUDE_DIRS the/stringize_macro.h
	PATHS
		$ENV{THELIB_PLUSPLUS_ROOT}/include
		${CMAKE_SOURCE_DIR}/../thelib++/include
#		/usr/local/include
#		/usr/X11/include
#		/usr/include
	)
#message(STATUS ${THELIB_INCLUDE_DIRS})
#message(STATUS ${CMAKE_BUILD_TYPE})

set(LIB_PATH_SUFFIXES lib lib64 build)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(LIB_PATH_SUFFIXES ${LIB_PATH_SUFFIXES} debug release)
else()
	set(LIB_PATH_SUFFIXES ${LIB_PATH_SUFFIXES} release debug)
endif()

#message(STATUS ${LIB_PATH_SUFFIXES})

FIND_LIBRARY(THELIB_LIBRARY thelib
	PATHS
		$ENV{THELIB_PLUSPLUS_ROOT}
		${CMAKE_SOURCE_DIR}/../thelib++
#		/usr/local
#		/usr/X11
#		/usr
	PATH_SUFFIXES ${LIB_PATH_SUFFIXES}
	)

#message(STATUS ${THELIB_LIBRARY})

SET(THELIB_FOUND FALSE)
IF (THELIB_INCLUDE_DIRS AND THELIB_LIBRARY)
	SET(THELIB_FOUND TRUE)
ENDIF (THELIB_INCLUDE_DIRS AND THELIB_LIBRARY)

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TheLib++ DEFAULT_MSG THELIB_LIBRARY THELIB_INCLUDE_DIRS)

