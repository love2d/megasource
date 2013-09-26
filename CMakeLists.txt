if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	# Protop: run cmake like this: cmake -G "<generator>" -H. -Bbuild
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 2.8)

project(megasource)

set(DEP_ZLIB_VER "1.2.8")
set(DEP_PHYSFS_VER "2.0.3")
set(DEP_LUA51_VER "5.1.5")
set(DEP_LIBPNG_VER "166")
set(DEP_JPEG_VER "9")
set(DEP_LIBOGG_VER "1.3.1")
set(DEP_LIBVORBIS_VER "1.3.3")
set(DEP_MPG123_VER "1.15.3")
set(DEP_FREETYPE_VER "2.5.0.1")
set(DEP_SDL2_VER "2.0.0")
set(DEP_OPENAL_VER "1.15.1")
set(DEP_JASPER_VER "1.900.1")
set(DEP_TIFF_VER "4.0.3")
set(DEP_DEVIL_VER "1.7.8")
set(DEP_MODPLUG_VER "0.8.8.4")

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
add_subdirectory("libs/freetype-${DEP_FREETYPE_VER}" ${CMAKE_BINARY_DIR}/freetype)
add_subdirectory("libs/SDL2-${DEP_SDL2_VER}" ${CMAKE_BINARY_DIR}/SDL2)
add_subdirectory("libs/openal-soft-${DEP_OPENAL_VER}" ${CMAKE_BINARY_DIR}/openal-soft)
add_subdirectory("libs/jasper-${DEP_JASPER_VER}" ${CMAKE_BINARY_DIR}/jasper)
add_subdirectory("libs/tiff-${DEP_TIFF_VER}" ${CMAKE_BINARY_DIR}/tiff)
add_subdirectory("libs/DevIL-${DEP_DEVIL_VER}" ${CMAKE_BINARY_DIR}/DevIL)
add_subdirectory("libs/libmodplug-${DEP_MODPLUG_VER}" ${CMAKE_BINARY_DIR}/libmodplug)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/love/CMakeLists.txt)
	message(STATUS "Making LOVE")
	add_subdirectory("libs/love" ${CMAKE_BINARY_DIR}/love)
else()
	message(STATUS "LOVE was not found")
endif()

link_directories(${SDL_LINK_DIR})
add_executable(megatest src/test.cpp)
target_link_libraries(megatest
	zlibstatic
	physfs-static
	lua51-static
	${PNG_LIB_NAME_STATIC}
	jpeg-static
	ogg-static
	vorbis-static
	vorbisfile-static
	mpg123
	freetype-static
	SDL2
	SDL2main
	${OPENAL_LIB_NAME}
	jasper-static
	tiff-static
	DevIL
	modplug-static
)

function(add_move_dll ARG_TARGET)
	add_custom_command(TARGET megatest POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		$<TARGET_FILE:${ARG_TARGET}>
		${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/$<TARGET_FILE_NAME:${ARG_TARGET}>)
endfunction()

add_move_dll(mpg123)
add_move_dll(SDL2)
add_move_dll(${OPENAL_LIB_NAME})
add_move_dll(DevIL)
