#
# RIV Root CMakeLists
#
cmake_minimum_required(VERSION 2.8)

project(riv)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

set(BUILDING_RIV_PROJECT 1)
#set(BUILD_TESTS 1)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# some manual external dependancies
find_package(TheLib++ REQUIRED)
find_package(Vislib REQUIRED)

# processor word size detection
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)


# actual projects
add_subdirectory(rivlib)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

# hacked: install extra vislib & thelib files
file(GLOB_RECURSE vislib_extra ${CMAKE_SOURCE_DIR}/../vislib/release/*.a)
message(STATUS "Files: ${vislib_extra}")
INSTALL(FILES ${vislib_extra} DESTINATION lib)

#  math/libvislibmath64.a
#  net/libvislibnet64.a
#  base/libvislibbase64.a
#  sys/libvislibsys64.a
#  graphics/libvislibgraphics64.a

INSTALL(FILES ${CMAKE_SOURCE_DIR}/../thelib++/release/libthelib.a DESTINATION lib)
# copy include folder to install folder
INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/../thelib++/include/."
  DESTINATION include
  PATTERN ".svn" EXCLUDE
  PATTERN ".git" EXCLUDE
)
