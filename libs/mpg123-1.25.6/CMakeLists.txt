cmake_minimum_required(VERSION 2.8)

project(mpg123)

option(MPG123_BUILD_SHARED "Build shared library" ON)
option(MPG123_BUILD_STATIC "Build static library" OFF)

set(MPG123_SRC
	src/libmpg123/dct64.c
	src/libmpg123/equalizer.c
	src/libmpg123/feature.c
	src/libmpg123/format.c
	src/libmpg123/frame.c
	src/libmpg123/icy.c
	src/libmpg123/icy2utf8.c
	src/libmpg123/id3.c
	src/libmpg123/index.c
	src/libmpg123/layer1.c
	src/libmpg123/layer2.c
	src/libmpg123/layer3.c
	src/libmpg123/libmpg123.c
	src/libmpg123/ntom.c
	src/libmpg123/optimize.c
	src/libmpg123/parse.c
	src/libmpg123/readers.c
	src/libmpg123/stringbuf.c
	src/libmpg123/synth.c
	src/libmpg123/synth_8bit.c
	src/libmpg123/synth_real.c
	src/libmpg123/synth_s32.c
	src/libmpg123/tabinit.c
)

set(MPG123_SRC
	${MPG123_SRC}
	src/compat/compat.c
	src/compat/compat_str.c
)

set(MPG123_INCLUDE_DIRS src src/libmpg123 src/compat)

if(MSVC)
	add_definitions(-DOPT_GENERIC)
	set(MPG123_INCLUDE_DIRS ${MPG123_INCLUDE_DIRS} ports/MSVC++)
elseif(APPLE)
	set(MPG123_INCLUDE_DIRS ${MPG123_INCLUDE_DIRS} ports/Xcode)
endif()

if (MPG123_BUILD_STATIC)
	add_library(mpg123-static STATIC ${MPG123_SRC})
	target_include_directories(mpg123-static PUBLIC ${MPG123_INCLUDE_DIRS})
endif()

if (MPG123_BUILD_SHARED)
	add_library(mpg123 SHARED ${MPG123_SRC})
	target_include_directories(mpg123 PUBLIC ${MPG123_INCLUDE_DIRS})
	if(MSVC)
		target_link_libraries(mpg123 Shlwapi)
		set_property(TARGET mpg123 PROPERTY COMPILE_DEFINITIONS BUILD_MPG123_DLL)
	endif()
	install(TARGETS mpg123 RUNTIME DESTINATION . LIBRARY DESTINATION .)
endif()
