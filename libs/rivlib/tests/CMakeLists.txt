#
# RIV Tests CMakeLists
#
cmake_minimum_required(VERSION 2.8)

# Check if project is riv target
if (NOT DEFINED BUILDING_RIV_PROJECT)
	message(FATAL_ERROR "This CMakefile cannot be processed independently.")
endif()


# dependancies required by the tests
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(AntTweakBar REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)


#actual tests
add_subdirectory(rivprovtest)
add_subdirectory(rivclnttest)

