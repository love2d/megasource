cmake_minimum_required(VERSION 2.8)

project(freetype)

option(FREETYPE_BUILD_SHARED "Build shared library" ON)
option(FREETYPE_BUILD_STATIC "Build static library" ON)

set(FREETYPE_SRC
	src/autofit/autofit.c
	src/base/ftbase.c
	src/base/ftbbox.c
	src/base/ftbitmap.c
	src/base/ftfstype.c
	src/base/ftgasp.c
	src/base/ftglyph.c
	src/base/ftgxval.c
	src/base/ftinit.c
	src/base/ftlcdfil.c
	src/base/ftmm.c
	src/base/ftotval.c
	src/base/ftpatent.c
	src/base/ftpfr.c
	src/base/ftstroke.c
	src/base/ftsynth.c
	src/base/ftsystem.c
	src/base/fttype1.c
	src/base/ftwinfnt.c
	src/base/ftxf86.c
	src/bdf/bdf.c
	src/cache/ftcache.c
	src/cff/cff.c
	src/cid/type1cid.c
	src/gzip/ftgzip.c
	src/lzw/ftlzw.c
	src/pcf/pcf.c
	src/pfr/pfr.c
	src/psaux/psaux.c
	src/pshinter/pshinter.c
	src/psnames/psmodule.c
	src/raster/raster.c
	src/sfnt/sfnt.c
	src/smooth/smooth.c
	src/truetype/truetype.c
	src/type1/type1.c
	src/type42/type42.c
	src/winfonts/winfnt.c
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS -DFT2_BUILD_LIBRARY)

if(WIN32)
    list(APPEND FREETYPE_SRC builds/win32/ftdebug.c)
endif()

if (FREETYPE_BUILD_STATIC)
	add_library(freetype-static STATIC ${FREETYPE_SRC})
	target_include_directories(freetype-static PUBLIC include)
endif()

if (FREETYPE_BUILD_SHARED)
	add_library(freetype SHARED ${FREETYPE_SRC})
	target_include_directories(freetype PUBLIC include)
endif()
