#
# Vislib glutInclude
#
cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED BUILDING_VISLIB_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


find_package(GLUT REQUIRED)


add_custom_target(glutInclude ALL
	COMMAND cmake -E copy "${CMAKE_CURRENT_SOURCE_DIR}/glutInclude.system.h" "${CMAKE_CURRENT_SOURCE_DIR}/glutInclude.lin.h"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/glutInclude.system.h"
	)

