if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	# Protop: run cmake like this: cmake -G "<generator>" -H. -Bbuild
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 2.8)

project(ldep)

set(DEP_ZLIB_VER "1.2.8")
set(DEP_PHYSFS_VER "2.0.3")
set(DEP_LUA51_VER "5.1.5")
set(DEP_LIBPNG_VER "166")
set(DEP_JPEG_VER "9")
set(DEP_LIBOGG_VER "1.3.1")
set(DEP_LIBVORBIS_VER "1.3.3")
set(DEP_MPG123_VER "1.15.3")

set(SKIP_INSTALL_ALL TRUE)

set(ZLIB_FOUND TRUE)
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_INCLUDE_DIR .)

set(OGG_LIBRARY ogg-static)

add_subdirectory("libs/zlib-${DEP_ZLIB_VER}" ${CMAKE_BINARY_DIR}/zlib)
add_subdirectory("libs/physfs-${DEP_PHYSFS_VER}" ${CMAKE_BINARY_DIR}/physfs)
add_subdirectory("libs/lua-${DEP_LUA51_VER}" ${CMAKE_BINARY_DIR}/lua51)
add_subdirectory("libs/lpng${DEP_LIBPNG_VER}" ${CMAKE_BINARY_DIR}/lpng)
add_subdirectory("libs/jpeg-${DEP_JPEG_VER}" ${CMAKE_BINARY_DIR}/jpeg)
add_subdirectory("libs/libogg-${DEP_LIBOGG_VER}" ${CMAKE_BINARY_DIR}/libogg)
add_subdirectory("libs/libvorbis-${DEP_LIBVORBIS_VER}" ${CMAKE_BINARY_DIR}/libvorbis)
add_subdirectory("libs/mpg123-${DEP_MPG123_VER}" ${CMAKE_BINARY_DIR}/mpg123)

add_executable(ldeptest src/test.cpp)
target_link_libraries(ldeptest
	zlibstatic
	physfs-static
	lua51-static
	${PNG_LIB_NAME_STATIC}
	jpeg-static
	ogg-static
	vorbis-static
	vorbisfile-static
	mpg123
)

add_custom_command(TARGET ldeptest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:mpg123>
	${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/$<TARGET_FILE_NAME:mpg123>)
