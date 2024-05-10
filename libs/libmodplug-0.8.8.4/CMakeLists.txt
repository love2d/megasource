cmake_minimum_required(VERSION 2.8)

project(modplug)

option(MODPLUG_BUILD_SHARED "Build shared library" OFF)
option(MODPLUG_BUILD_STATIC "Build static library" ON)

set(MODPLUG_SRC
	src/fastmix.cpp
	src/load_669.cpp
	src/load_abc.cpp
	src/load_amf.cpp
	src/load_ams.cpp
	src/load_dbm.cpp
	src/load_dmf.cpp
	src/load_dsm.cpp
	src/load_far.cpp
	src/load_it.cpp
	src/load_j2b.cpp
	src/load_mdl.cpp
	src/load_med.cpp
	src/load_mid.cpp
	src/load_mod.cpp
	src/load_mt2.cpp
	src/load_mtm.cpp
	src/load_okt.cpp
	src/load_pat.cpp
	src/load_psm.cpp
	src/load_ptm.cpp
	src/load_s3m.cpp
	src/load_stm.cpp
	src/load_ult.cpp
	src/load_umx.cpp
	src/load_wav.cpp
	src/load_xm.cpp
	src/mmcmp.cpp
	src/modplug.cpp
	src/snd_dsp.cpp
	src/snd_flt.cpp
	src/snd_fx.cpp
	src/sndfile.cpp
	src/sndmix.cpp
)

file(COPY src/modplug.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libmodplug)

if(APPLE OR EMSCRIPTEN)
	include(CheckFunctionExists)
	include(CheckIncludeFiles)
	include(CheckLibraryExists)
	include(CheckSymbolExists)
	include(CheckTypeSize)

	check_function_exists(sinf HAVE_SINF)
	check_function_exists(setenv HAVE_SETENV)

	check_include_files(dlfcn.h HAVE_DLFCN_H)
	check_include_files(inttypes.h HAVE_INTTYPES_H)
	check_include_files(malloc.h HAVE_MALLOC_H)
	check_include_files(memory.h HAVE_MEMORY_H)
	check_include_files(stdint.h HAVE_STDINT_H)
	check_include_files(stdlib.h HAVE_STDLIB_H)
	check_include_files(strings.h HAVE_STRINGS_H)
	check_include_files(string.h HAVE_STRING_H)
	check_include_files("sys/stat.h" HAVE_SYS_STAT_H)
	check_include_files("sys/types.h" HAVE_SYS_TYPES_H)
	check_include_files(unistd.h HAVE_UNISTD_H)

	set(VERSION "0.8.8.4")

	configure_file(src/config.h.cmake config.h)
	add_definitions(-DHAVE_CONFIG_H)
endif()

if(MODPLUG_BUILD_STATIC)
	add_library(modplug-static STATIC ${MODPLUG_SRC})
	target_include_directories(modplug-static PUBLIC src src/libmodplug ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(MODPLUG_BUILD_SHARED)
	add_library(modplug SHARED ${MODPLUG_SRC})
	target_include_directories(modplug PUBLIC src src/libmodplug ${CMAKE_CURRENT_BINARY_DIR})
endif()
