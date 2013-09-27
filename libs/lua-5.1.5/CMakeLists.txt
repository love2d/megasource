cmake_minimum_required(VERSION 2.8)

project(lua51)

option(LUA_BUILD_SHARED "Build shared library" ON)
option(LUA_BUILD_STATIC "Build static library" OFF)

set(LUA_SRC
	src/lapi.c
	src/lauxlib.c
	src/lbaselib.c
	src/lcode.c
	src/ldblib.c
	src/ldebug.c
	src/ldo.c
	src/ldump.c
	src/lfunc.c
	src/lgc.c
	src/linit.c
	src/liolib.c
	src/llex.c
	src/lmathlib.c
	src/lmem.c
	src/loadlib.c
	src/lobject.c
	src/lopcodes.c
	src/loslib.c
	src/lparser.c
	src/lstate.c
	src/lstring.c
	src/lstrlib.c
	src/ltable.c
	src/ltablib.c
	src/ltm.c
	src/lundump.c
	src/lvm.c
	src/lzio.c
	src/print.c
)

if (LUA_BUILD_STATIC)
	add_library(lua51-static STATIC ${LUA_SRC})
	target_include_directories(lua51-static PUBLIC src)
endif()

if (LUA_BUILD_SHARED)
	add_library(lua51 SHARED ${LUA_SRC})
	target_include_directories(lua51 PUBLIC src)
	set_property(TARGET lua51 PROPERTY COMPILE_DEFINITIONS LUA_BUILD_AS_DLL=1)
endif()
