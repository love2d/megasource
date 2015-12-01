cmake_minimum_required(VERSION 2.8)

project(ogg)

option(OGG_BUILD_SHARED "Build shared library" OFF)
option(OGG_BUILD_STATIC "Build static library" ON)

set(OGG_SRC
	src/bitwise.c
	src/framing.c
)

if(OGG_BUILD_STATIC)
	add_library(ogg-static STATIC ${OGG_SRC})
	target_include_directories(ogg-static PUBLIC include)
endif()

if(OGG_BUILD_SHARED)
	add_library(ogg SHARED ${OGG_SRC} win32/ogg.def)
	target_include_directories(ogg PUBLIC include)
endif()
