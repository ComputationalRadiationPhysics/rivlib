#
# Vislib glutTest
#
cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED BUILDING_VISLIB_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


find_package(GLUT)
find_package(OpenGL)


#input file
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

# include directories
include_directories("include" "src"
	"../base/include"
	"../math/include"
	"../sys/include"
	"../net/include"
	"../graphics/include"
	"../gl/include"
	"../glutInclude"
	)


# target definition
add_executable(vislibgluttest ${header_files} ${source_files})
target_link_libraries(vislibgluttest 
	vislibgl${BITS} vislibgraphics${BITS} vislibnet${BITS} vislibsys${BITS} vislibmath${BITS} vislibbase${BITS}
	${GLUT_LIBRARIES} ${OPENGL_LIBRARIES}
	)

