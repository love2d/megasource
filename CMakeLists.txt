if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	# Protop: run cmake like this: cmake -G "<generator>" -H. -Bbuild
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 2.8)

project(ldep)

set(DEP_ZLIB_VER "1.2.8")

add_subdirectory("libs/zlib-${DEP_ZLIB_VER}" ${CMAKE_BINARY_DIR}/zlib)

add_executable(ldeptest src/test.cpp)
target_link_libraries(ldeptest zlibstatic)
