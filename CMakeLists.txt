if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	# Protop: run cmake like this: cmake -G "<generator>" -H. -Bbuild
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 2.8)

project(megasource)

set(MEGA TRUE)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(MEGA_X64 TRUE)
	set(MEGA_ARCH x64)
else()
	set(MEGA_X86 TRUE)
	set(MEGA_ARCH x86)
endif()

if(MSVC)
	set(CMAKE_SUPPRESS_REGENERATION TRUE)
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP")
	message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
	message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
endif()

function(add_move_dll ARG_POST_TARGET ARG_TARGET)
	add_custom_command(TARGET ${ARG_POST_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		$<TARGET_FILE:${ARG_TARGET}>
		${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/$<TARGET_FILE_NAME:${ARG_TARGET}>)
endfunction()

function(disable_warnings ARG_TARGET)
	get_target_property(OLD_FLAGS ${ARG_TARGET} COMPILE_FLAGS)
	set(NEW_FLAGS -W0)
	if(OLD_FLAGS)
		set(NEW_FLAGS "${OLD_FLAGS} ${NEW_FLAGS}")
	endif()
	set_target_properties(${ARG_TARGET} PROPERTIES COMPILE_FLAGS ${NEW_FLAGS})
endfunction()

# Look for dynamic runtime DLLs.
if(MSVC11)
	set(VS110COMNTOOLS "$ENV{VS110COMNTOOLS}")
	if(${VS110COMNTOOLS} STREQUAL "")
		message(WARNING "Environment variable VS110COMNTOOLS not defined. Is VS11 properly installed?")
	else()
		get_filename_component(VS110_REDIST_DIR ${VS110COMNTOOLS}/../../VC/redist/${MEGA_ARCH}/Microsoft.VC110.CRT ABSOLUTE)
		set(MSVCP110_DLL ${VS110_REDIST_DIR}/msvcp110.dll)
		set(MSVCR110_DLL ${VS110_REDIST_DIR}/msvcr110.dll)
		if(EXISTS ${MSVCP110_DLL} AND EXISTS ${MSVCR110_DLL})
			message(STATUS "Found msvcp110.dll, msvcr110.dll")
			set(MEGA_MSVC_DLLS ${MSVCP110_DLL} ${MSVCR110_DLL})
		else()
			message(WARNING "Could not find msvcp110.dll, msvcr110.dll")
		endif()
	endif()
elseif(MSVC12)
	set(VS120COMNTOOLS "$ENV{VS120COMNTOOLS}")
	if(${VS120COMNTOOLS} STREQUAL "")
		message(WARNING "Environment variable VS120COMNTOOLS not defined. Is VS12 properly installed?")
	else()
		get_filename_component(VS120_REDIST_DIR ${VS120COMNTOOLS}/../../VC/redist/${MEGA_ARCH}/Microsoft.VC120.CRT ABSOLUTE)
		set(MSVCP120_DLL ${VS120_REDIST_DIR}/msvcp120.dll)
		set(MSVCR120_DLL ${VS120_REDIST_DIR}/msvcr120.dll)
		if(EXISTS ${MSVCP120_DLL} AND EXISTS ${MSVCR120_DLL})
			message(STATUS "Found msvcp120.dll, msvcr120.dll")
			set(MEGA_MSVC_DLLS ${MSVCP120_DLL} ${MSVCR120_DLL})
		else()
			message(WARNING "Could not find msvcp120.dll, msvcr120.dll")
		endif()
	endif()
endif()

set(MEGA_ZLIB_VER "1.2.8")
set(MEGA_PHYSFS_VER "2.0.3")
set(MEGA_LUA51_VER "5.1.5")
set(MEGA_LIBPNG_VER "166")
set(MEGA_JPEG_VER "9")
set(MEGA_LIBOGG_VER "1.3.1")
set(MEGA_LIBVORBIS_VER "1.3.3")
set(MEGA_MPG123_VER "1.15.3")
set(MEGA_FREETYPE_VER "2.5.0.1")
set(MEGA_SDL2_VER "2.0.0")
set(MEGA_OPENAL_VER "1.15.1")
set(MEGA_JASPER_VER "1.900.1")
set(MEGA_TIFF_VER "4.0.3")
set(MEGA_DEVIL_VER "1.7.8")
set(MEGA_MODPLUG_VER "0.8.8.4")

set(SKIP_INSTALL_ALL TRUE)

set(ZLIB_FOUND TRUE)
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_INCLUDE_DIR .)

set(OGG_LIBRARY ogg-static)

add_subdirectory("libs/zlib-${MEGA_ZLIB_VER}" ${CMAKE_BINARY_DIR}/zlib)
set(MEGA_ZLIB zlibstatic)

add_subdirectory("libs/physfs-${MEGA_PHYSFS_VER}" ${CMAKE_BINARY_DIR}/physfs)
set(MEGA_PHYSFS physfs-static)

add_subdirectory("libs/lua-${MEGA_LUA51_VER}" ${CMAKE_BINARY_DIR}/lua51)
set(MEGA_LUA51 lua51)

add_subdirectory("libs/lpng${MEGA_LIBPNG_VER}" ${CMAKE_BINARY_DIR}/lpng)
set(MEGA_LIBPNG ${PNG_LIB_NAME_STATIC})

add_subdirectory("libs/jpeg-${MEGA_JPEG_VER}" ${CMAKE_BINARY_DIR}/jpeg)
set(MEGA_JPEG jpeg-static)

add_subdirectory("libs/libogg-${MEGA_LIBOGG_VER}" ${CMAKE_BINARY_DIR}/libogg)
set(MEGA_LIBOGG ogg-static)

add_subdirectory("libs/libvorbis-${MEGA_LIBVORBIS_VER}" ${CMAKE_BINARY_DIR}/libvorbis)
set(MEGA_LIBVORBIS vorbis-static)
set(MEGA_LIBVORBISFILE vorbisfile-static)

add_subdirectory("libs/mpg123-${MEGA_MPG123_VER}" ${CMAKE_BINARY_DIR}/mpg123)
set(MEGA_MPEG123 mpg123)

add_subdirectory("libs/freetype-${MEGA_FREETYPE_VER}" ${CMAKE_BINARY_DIR}/freetype)
set(MEGA_FREETYPE freetype-static)

add_subdirectory("libs/SDL2-${MEGA_SDL2_VER}" ${CMAKE_BINARY_DIR}/SDL2)
set(MEGA_SDL2 SDL2)
set(MEGA_SDL2MAIN SDL2main)

add_subdirectory("libs/openal-soft-${MEGA_OPENAL_VER}" ${CMAKE_BINARY_DIR}/openal-soft)
set(MEGA_OPENAL ${OPENAL_LIB_NAME})

add_subdirectory("libs/jasper-${MEGA_JASPER_VER}" ${CMAKE_BINARY_DIR}/jasper)
set(MEGA_JASPER jasper-static)

add_subdirectory("libs/tiff-${MEGA_TIFF_VER}" ${CMAKE_BINARY_DIR}/tiff)
set(MEGA_TIFF tiff-static)

add_subdirectory("libs/DevIL-${MEGA_DEVIL_VER}" ${CMAKE_BINARY_DIR}/DevIL)
set(MEGA_DEVIL DevIL)

add_subdirectory("libs/libmodplug-${MEGA_MODPLUG_VER}" ${CMAKE_BINARY_DIR}/libmodplug)
set(MEGA_MODPLUG modplug-static)

set(MEGA_3P
	${MEGA_ZLIB}
	${MEGA_PHYSFS}
	${MEGA_LUA51}
	${MEGA_LIBPNG}
	${MEGA_JPEG}
	${MEGA_LIBOGG}
	${MEGA_LIBVORBIS}
	${MEGA_LIBVORBISFILE}
	${MEGA_MPEG123}
	${MEGA_FREETYPE}
	${MEGA_SDL2}
	${MEGA_SDL2MAIN}
	${MEGA_OPENAL}
	${MEGA_JASPER}
	${MEGA_TIFF}
	${MEGA_DEVIL}
	${MEGA_MODPLUG}
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/love/CMakeLists.txt)
	message(STATUS "Making LOVE")
	add_subdirectory("libs/love" ${CMAKE_BINARY_DIR}/love)

	# Turn off third-party warnings when building with LOVE.
	foreach(MEGA_LIB ${MEGA_3P})
		disable_warnings(${MEGA_LIB})
	endforeach()

else()
	message(STATUS "LOVE was not found")
endif()

link_directories(${SDL_LINK_DIR})
add_executable(megatest src/test.cpp)
target_link_libraries(megatest ${MEGA_3P})

add_move_dll(megatest ${MEGA_MPEG123})
add_move_dll(megatest ${MEGA_SDL2})
add_move_dll(megatest ${MEGA_OPENAL})
add_move_dll(megatest ${MEGA_DEVIL})
add_move_dll(megatest ${MEGA_LUA51})
