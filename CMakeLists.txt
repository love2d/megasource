if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	# Protop: run cmake like this: cmake -G "<generator>" -H. -Bbuild
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 3.1)

include(ExternalProject)

project(megasource)

set(MEGA TRUE)

set (CMAKE_CXX_STANDARD 11)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	if(MSVC)
		# Normally we should rely on CMAKE_SYSTEM_PROCESSOR
		# but as of CMake 3.19.1, it sets to AMD64 even when targetting
		# Windows ARM64. This needs to be reevaluated in the future.
		set(_MEGATEMP_CHECKVAR CMAKE_SYSTEM_PROCESSOR)
		if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
			set(_MEGATEMP_CHECKVAR CMAKE_GENERATOR_PLATFORM)
		# Note that MSVC_CXX_ARCHITECTURE_ID is an undocumented
		# variable, but unfortunately we need to depends on it
		# as reason stated above.
		# https://gitlab.kitware.com/cmake/cmake/-/issues/15170
		elseif(DEFINED MSVC_CXX_ARCHITECTURE_ID)
			set(_MEGATEMP_CHECKVAR MSVC_CXX_ARCHITECTURE_ID)
		endif()
		# FIXME: Code below also duplicates MEGA_X64 architecture assignment.
		if(${_MEGATEMP_CHECKVAR} STREQUAL "ARM64")
			set(MEGA_ARM64 TRUE)
			set(MEGA_ARCH ARM64)
		else()
			set(MEGA_X64 TRUE)
			set(MEGA_ARCH x64)
		endif()
		unset(_MEGATEMP_CHECKVAR)
	else()
		set(MEGA_X64 TRUE)
		set(MEGA_ARCH x64)
	endif()
else()
	set(MEGA_X86 TRUE)
	set(MEGA_ARCH x86)
endif()

if(MSVC)
	set(CMAKE_SUPPRESS_REGENERATION TRUE)
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

function(add_move_file ARG_POST_TARGET ARG_FILE)
	get_filename_component(TEMP_FILENAME ${ARG_FILE} NAME)
	add_custom_command(TARGET ${ARG_POST_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${ARG_FILE}
		${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/${TEMP_FILENAME})
endfunction()

function(disable_warnings ARG_TARGET)
	get_target_property(OLD_FLAGS ${ARG_TARGET} COMPILE_FLAGS)
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(NEW_FLAGS -w)
	else ()
		set(NEW_FLAGS -W0)
	endif ()
	if(OLD_FLAGS)
		set(NEW_FLAGS "${OLD_FLAGS} ${NEW_FLAGS}")
	endif()
	set_target_properties(${ARG_TARGET} PROPERTIES COMPILE_FLAGS ${NEW_FLAGS})
endfunction()

# Find vcvarsall.bat and look for dynamic runtime DLLs.
if(MSVC)
	option(LOVE_INSTALL_UCRT "Provide the Windows 10 Universal CRT DLLs with LOVE" OFF)

	if(NOT(MSVC_TOOLSET_VERSION STREQUAL ""))
		set(VSVERSION "${MSVC_TOOLSET_VERSION}")
	elseif((MSVC_VERSION GREATER 1909) AND (MSVC_VERSION LESS 1920))
		set(VSVERSION "141")
	elseif(MSVC_VERSION EQUAL 1900)
		set(VSVERSION "140")
	elseif(MSVC_VERSION EQUAL 1800)
		set(VSVERSION "120")
	elseif(MSVC_VERSION LESS 1800)
		message(WARNING "Visual Studio 2013 (VS12) or newer is required!")
	else()
		message(WARNING "Cannot determine Visual Studio version!")
	endif()

	set(PROGRAMFILES_X86_STR "ProgramFiles(x86)")
	set(PROGRAMFILES_X86 "$ENV{${PROGRAMFILES_X86_STR}}")

	set(VSCOMNTOOLSSTRING "VS${VSVERSION}COMNTOOLS")
	set(VSCOMNTOOLS "$ENV{${VSCOMNTOOLSSTRING}}")

	if(VSCOMNTOOLS STREQUAL "")
		set(VSWHERE_PATH "${PROGRAMFILES_X86}\\Microsoft Visual Studio\\Installer\\vswhere.exe")

		execute_process(COMMAND ${VSWHERE_PATH} "-property" "installationPath" OUTPUT_VARIABLE VS_INSTALL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

		if(NOT(VS_INSTALL_DIR STREQUAL ""))
			set(VSCOMNTOOLS "${VS_INSTALL_DIR}\\Common7\\Tools")
		endif()
	endif()

	if(VSCOMNTOOLS STREQUAL "")
		message(WARNING "Environment variable VSCOMNTOOLS not defined and vswhere.exe could not be found. Is Visual Studio properly installed?")
	else()
		# Find vcvarsall.bat.
		get_filename_component(VS_VCVARSALL_BAT ${VSCOMNTOOLS}/../../VC/Auxiliary/Build/vcvarsall.bat ABSOLUTE)

		if(NOT(EXISTS ${VS_VCVARSALL_BAT}))
			# older VS versions have it here
			get_filename_component(VS_VCVARSALL_BAT ${VSCOMNTOOLS}/../../VC/vcvarsall.bat ABSOLUTE)
		endif()

		if(EXISTS ${VS_VCVARSALL_BAT})
			message(STATUS "Found vcvarsall.bat")
			set(MEGA_MSVC_VCVARSALL_BAT ${VS_VCVARSALL_BAT})

			if(MEGA_X86)
				set(MEGA_MSVC_VCVARSALL_BAT_ARG "x86")
			elseif(MEGA_ARM64)
				set(MEGA_MSVC_VCVARSALL_BAT_ARG "x86_arm64")
			else()
				set(MEGA_MSVC_VCVARSALL_BAT_ARG "x86_amd64")
			endif()
		else()
			message(WARNING "Could not find vsvarsall.bat")
		endif()
	endif()

	if(LOVE_INSTALL_UCRT)
		# We currently ship on platforms which might not have the Universal C Runtime installed.
		set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
	endif()

	set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)

	# TODO: The code for InstallRequiredSystemLibraries needs the UCRTVersion and WindowsSDKVersion
	# environment variables set by vcvarsall in order to detect the correct version of the UCRT, in
	# newer Windows SDK versions. We don't gurantee that vcvarsall has been loaded before running...
	include(InstallRequiredSystemLibraries)

	foreach(RUNTIME_LIB ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
		if(RUNTIME_LIB MATCHES "concrt")
			set(MSVC_RUNTIME_CONCRT_PATH ${RUNTIME_LIB})
		elseif(RUNTIME_LIB MATCHES "msvcp")
			set(MSVC_RUNTIME_MSVCP_PATH ${RUNTIME_LIB})
		endif()
	endforeach()

	# InstallRequiredSystemLibraries adds the concrt dll to its list, but we don't need it.
	if (EXISTS ${MSVC_RUNTIME_CONCRT_PATH})
		list(REMOVE_ITEM CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "${MSVC_RUNTIME_CONCRT_PATH}")
	endif()

	# CMake 3.20 InstallRequiredSystemLibraries recognize ARM64, but their older version aren't.
	# Put this safeguard in place, otherwise it will copy AMD64 DLLS which is NOT what user wants!
	if(MEGA_ARM64 AND ${CMAKE_VERSION} VERSION_LESS "3.20.0")
		message(WARNING "InstallRequiredSystemLibraries of CMake ${CMAKE_VERSION} doesn't support Windows ARM64. Please update your CMake!")
		set(MSVC_RUNTIME_MSVCP_PATH "")
	endif()

	# InstallRequiredSystemLibraries (as of CMake 3.14.5) doesn't include msvcp140_1.dll or msvcp140_2.dll in its list.
	if(EXISTS ${MSVC_RUNTIME_MSVCP_PATH})
		get_filename_component(MSVC_RUNTIME_MSVCP_DIR ${MSVC_RUNTIME_MSVCP_PATH} DIRECTORY)
		message(STATUS "Found MSVC Redist directory: ${MSVC_RUNTIME_MSVCP_DIR}")

		file(GLOB MSVC_RUNTIME_MSVCP_FILES "${MSVC_RUNTIME_MSVCP_DIR}/msvcp*.dll")

		set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} ${MSVC_RUNTIME_MSVCP_FILES})

		list(REMOVE_DUPLICATES CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
	else()
		message(WARNING "InstallRequiredSystemLibraries did not find any msvcp runtime dll")
	endif()

	list(LENGTH CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS MSVC_RUNTIME_COUNT)
	message(STATUS "Found ${MSVC_RUNTIME_COUNT} Visual C/C++ Runtime files")

	set(MEGA_MSVC_DLLS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
endif()


set(MEGA_ZLIB_VER "1.2.11")
set(MEGA_LUA51_VER "5.1.5")
set(MEGA_LUAJIT_VER "2.1.0-a91d0d9")
set(MEGA_LIBOGG_VER "1.3.2")
set(MEGA_LIBVORBIS_VER "1.3.5")
set(MEGA_LIBTHEORA_VER "1.1.1")
set(MEGA_FREETYPE_VER "2.8.1")
set(MEGA_SDL2_VER "2.0.18")
set(MEGA_OPENAL_VER "1.21.1")
set(MEGA_MODPLUG_VER "0.8.8.4")

set(SKIP_INSTALL_ALL TRUE)

set(ZLIB_FOUND TRUE)
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_INCLUDE_DIR .)

set(OGG_LIBRARY ogg-static)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: zlib ${MEGA_ZLIB_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/zlib-${MEGA_ZLIB_VER}" ${CMAKE_BINARY_DIR}/zlib)
set(MEGA_ZLIB zlibstatic)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: lua ${MEGA_LUA51_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/lua-${MEGA_LUA51_VER}" ${CMAKE_BINARY_DIR}/lua51)
set(MEGA_LUA51 lua51)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: libogg ${MEGA_LIBOGG_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/libogg-${MEGA_LIBOGG_VER}" ${CMAKE_BINARY_DIR}/libogg)
set(MEGA_LIBOGG ogg-static)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: libvorbis ${MEGA_LIBVORBIS_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/libvorbis-${MEGA_LIBVORBIS_VER}" ${CMAKE_BINARY_DIR}/libvorbis)
set(MEGA_LIBVORBIS vorbis-static)
set(MEGA_LIBVORBISFILE vorbisfile-static)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: libtheora ${MEGA_LIBTHEORA_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/libtheora-${MEGA_LIBTHEORA_VER}" ${CMAKE_BINARY_DIR}/libtheora)
set(MEGA_LIBTHEORA theora-static)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: freetype ${MEGA_FREETYPE_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/freetype-${MEGA_FREETYPE_VER}" ${CMAKE_BINARY_DIR}/freetype)
set(MEGA_FREETYPE freetype)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: SDL2 ${MEGA_SDL2_VER}")
message(STATUS "-----------------------------------------------------")
if(WIN32 AND MEGA_ARM64)
	# In Windows, SDL always tries to open OpenGL32.dll, which is not available in Windows ARM64.
	# This will cause error later on which makes love.window.setMode unhappy even when
	# LOVE_GRAPHICS_USE_OPENGLES is set, so disable OpenGL backend.
	set(SDL_OPENGL OFF CACHE BOOL "(SDL) Enable OpenGL Backend" FORCE)
endif()
add_subdirectory("libs/SDL2" ${CMAKE_BINARY_DIR}/SDL2)
set(MEGA_SDL2 SDL2)
set(MEGA_SDL2MAIN SDL2main)

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: openal-soft ${MEGA_OPENAL_VER}")
message(STATUS "-----------------------------------------------------")
if(ANDROID)
	set(OBOE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/libs/oboe" CACHE PATH "Oboe Source Code" FORCE)
	set(ALSOFT_REQUIRE_OBOE ON CACHE BOOL "Require Oboe backend" FORCE)
	set(ALSOFT_REQUIRE_OPENSL ON CACHE BOOL "Require OpenSL backend" FORCE)
endif()
add_subdirectory("libs/openal-soft" ${CMAKE_BINARY_DIR}/openal-soft)
set(MEGA_OPENAL ${OPENAL_LIB_NAME})

message(STATUS "-----------------------------------------------------")
message(STATUS "Configuring: libmodplug ${MEGA_MODPLUG_VER}")
message(STATUS "-----------------------------------------------------")
add_subdirectory("libs/libmodplug-${MEGA_MODPLUG_VER}" ${CMAKE_BINARY_DIR}/libmodplug)
set(MEGA_MODPLUG modplug-static)

# LuaJIT
if(MSVC AND NOT MEGA_ARM64)
	message(STATUS "-----------------------------------------------------")
	message(STATUS "Configuring: LuaJIT ${MEGA_LUAJIT_VER}")
	message(STATUS "-----------------------------------------------------")
	set(MEGA_LUAJIT_DIR "LuaJIT")

	# Copy LuaJIT source to binary dir. LuaJIT builds in-source,
	# and we don't want to modify the original source tree, so
	# we copy it.
	file(COPY libs/${MEGA_LUAJIT_DIR} DESTINATION libs)

	set(MEGA_LUAJIT_SOURCE_DIR ${CMAKE_BINARY_DIR}/libs/${MEGA_LUAJIT_DIR})

	set(MEGA_LUAJIT_BUILD_BAT "
		if not defined DevEnvDir (
			call \"${MEGA_MSVC_VCVARSALL_BAT}\" ${MEGA_MSVC_VCVARSALL_BAT_ARG}
		)
		cd \"${MEGA_LUAJIT_SOURCE_DIR}/src\"
		if %1 == \"\" shift /1
		msvcbuild.bat %1 %2
	")

	file(WRITE ${MEGA_LUAJIT_SOURCE_DIR}/megabuild.bat ${MEGA_LUAJIT_BUILD_BAT})

	ExternalProject_Add(luajit
		SOURCE_DIR ${MEGA_LUAJIT_SOURCE_DIR}
		BUILD_IN_SOURCE 1
		CONFIGURE_COMMAND ""
		BUILD_COMMAND "${MEGA_LUAJIT_SOURCE_DIR}/megabuild.bat" $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:debug> $<$<NOT:$<CONFIG:Debug>>:amalg>
		INSTALL_COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/bin
			COMMAND cmake -E copy ${MEGA_LUAJIT_SOURCE_DIR}/src/lua51.dll ${CMAKE_BINARY_DIR}/bin
			COMMAND cmake -E copy ${MEGA_LUAJIT_SOURCE_DIR}/src/lua51.lib ${CMAKE_BINARY_DIR}/bin)

	set(MEGA_LUAJIT_LIB ${CMAKE_BINARY_DIR}/bin/lua51.lib)
	set(MEGA_LUAJIT_DLL ${CMAKE_BINARY_DIR}/bin/lua51.dll)
	set(MEGA_LUAJIT_INCLUDE ${MEGA_LUAJIT_SOURCE_DIR}/src)
elseif(ANDROID)
	# LuaJIT is prebuilt in Android
	message(STATUS "-----------------------------------------------------")
	message(STATUS "Configuring: LuaJIT ${MEGA_LUAJIT_VER}")
	message(STATUS "-----------------------------------------------------")
	set(MEGA_LUAJIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/LuaJIT")

	add_library(luajit IMPORTED SHARED)
	set_target_properties(luajit PROPERTIES
		IMPORTED_LOCATION ${MEGA_LUAJIT_DIR}/android/${ANDROID_ABI}/libluajit.so
		INTERFACE_INCLUDE_DIRECTORIES ${MEGA_LUAJIT_DIR}/src
	)
	set(MEGA_LUAJIT_LIB ${MEGA_LUAJIT_DIR}/android/${ANDROID_ABI}/libluajit.so)
	set(MEGA_LUAJIT_DLL)
	set(MEGA_LUAJIT_INCLUDE ${MEGA_LUAJIT_DIR}/src)
	set(LUA_FOUND ON)
	set(LUA_INCLUDE_DIR ${MEGA_LUAJIT_INCLUDE})
	set(LUA_LIBRARY luajit)
	set(LUA_VERSION_MAJOR 5)
	set(LUA_VERSION_MINOR 1)
	set(LUA_VERSION_PATCH 5)
	set(LUA_VERSION_STRING "LuaJIT ${MEGA_LUAJIT_VER}")
endif()

set(MEGA_3P
	${MEGA_ZLIB}
	${MEGA_LUA51}
	${MEGA_LIBOGG}
	${MEGA_LIBVORBIS}
	${MEGA_LIBVORBISFILE}
	${MEGA_LIBTHEORA}
	${MEGA_FREETYPE}
	${MEGA_SDL2}
	${MEGA_SDL2MAIN}
	${MEGA_OPENAL}
	${MEGA_MODPLUG}
)

# Set to directory of love source. Defaults to ./libs/love.
if(NOT MEGA_LOVE)
	set(MEGA_LOVE ${CMAKE_CURRENT_SOURCE_DIR}/libs/love)
endif()

if(EXISTS ${MEGA_LOVE}/CMakeLists.txt)
	message(STATUS "-----------------------------------------------------")
	message(STATUS "Configuring: love")
	message(STATUS "-----------------------------------------------------")
	if(MEGA_ARM64)
		# LuaJIT currently doesn't support targetting Windows with ARM64
		# architecture yet.
		# TODO: Re-evaluate this once LuaJIT supports it.
		# https://github.com/LuaJIT/LuaJIT/issues/593
		set(LOVE_JIT OFF CACHE BOOL "Enable LOVE JIT" FORCE)
	endif()
	add_subdirectory(${MEGA_LOVE} ${CMAKE_BINARY_DIR}/love)

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

add_move_dll(megatest ${MEGA_SDL2})
add_move_dll(megatest ${MEGA_OPENAL})
add_move_dll(megatest ${MEGA_LUA51})
