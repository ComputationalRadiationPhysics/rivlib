#
# Vislib sys
#
cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED BUILDING_VISLIB_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)
find_package(Curses REQUIRED)


#input file
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.h")
file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")
list(REMOVE_ITEM source_files
	"src/DynamicFunctionPointer.cpp"
	"src/PAMException.cpp"
	"src/LinuxDaemon.cpp"
	)

# include directories
include_directories("include" "src"
	"../base/include"
	"../math/include"
	)


# target definition
add_library(vislibsys${BITS} STATIC ${header_files} ${source_files})
target_link_libraries(vislibsys${BITS} ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES} ${CURSES_LIBRARIES} -lrt -lcrypt)


