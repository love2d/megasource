cmake_minimum_required(VERSION 2.8)

project(vorbis)

option(VORBIS_BUILD_SHARED "Build shared libvorbis" OFF)
option(VORBIS_BUILD_STATIC "Build static libvorbis" ON)
option(VORBISFILE_BUILD_SHARED "Build shared libvorbisfile" OFF)
option(VORBISFILE_BUILD_STATIC "Build static libvorbisfile" ON)

set(VORBIS_SRC
	lib/analysis.c
	lib/bitrate.c
	lib/block.c
	lib/codebook.c
	lib/envelope.c
	lib/floor0.c
	lib/floor1.c
	lib/info.c
	lib/lookup.c
	lib/lpc.c
	lib/lsp.c
	lib/mapping0.c
	lib/mdct.c
	lib/psy.c
	lib/registry.c
	lib/res0.c
	lib/sharedbook.c
	lib/smallft.c
	lib/synthesis.c
	lib/vorbisenc.c
	lib/window.c
	win32/vorbis.def
)

set(VORBISFILE_SRC
	lib/vorbisfile.c
	win32/vorbisfile.def
)

if(VORBIS_BUILD_STATIC)
	add_library(vorbis-static STATIC ${VORBIS_SRC})
	target_include_directories(vorbis-static PUBLIC include lib)
	target_link_libraries(vorbis-static ${OGG_LIBRARY})
endif()

if(VORBIS_BUILD_SHARED)
	add_library(vorbis SHARED ${VORBIS_SRC} win32/vorbis.def)
	target_include_directories(vorbis PUBLIC include lib)
	target_link_libraries(vorbis ${OGG_LIBRARY})
endif()

if(VORBISFILE_BUILD_STATIC)
	add_library(vorbisfile-static STATIC ${VORBISFILE_SRC})
	target_include_directories(vorbisfile-static PUBLIC include)
	target_link_libraries(vorbisfile-static vorbis-static ${OGG_LIBRARY})
endif()

if(VORBISFILE_BUILD_SHARED)
	add_library(vorbisfile SHARED ${VORBISFILE_SRC} win32/vorbis.def)
	target_include_directories(vorbisfile PUBLIC include)
	target_link_libraries(vorbisfile vorbis ${OGG_LIBRARY})
endif()
