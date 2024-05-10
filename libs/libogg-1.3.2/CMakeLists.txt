cmake_minimum_required(VERSION 2.8)

project(ogg)

option(OGG_BUILD_SHARED "Build shared library" OFF)
option(OGG_BUILD_STATIC "Build static library" ON)

set(OGG_SRC
	src/bitwise.c
	src/framing.c
)

if (EMSCRIPTEN) # See: https://github.com/emscripten-ports/Ogg/blob/master/CMakeLists.txt
  include(CheckIncludeFiles)

  # Configure config_type.h
  check_include_files(inttypes.h INCLUDE_INTTYPES_H)
  check_include_files(stdint.h INCLUDE_STDINT_H)
  check_include_files(sys/types.h INCLUDE_SYS_TYPES_H)

  set(SIZE16 int16_t)
  set(USIZE16 uint16_t)
  set(SIZE32 int32_t)
  set(USIZE32 uint32_t)
  set(SIZE64 int64_t)

  configure_file(include/ogg/config_types.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ogg/config_types.h @ONLY)
endif ()

if (EMSCRIPTEN) # See: https://github.com/emscripten-ports/Ogg/blob/master/CMakeLists.txt
  include(CheckIncludeFiles)

  # Configure config_type.h
  check_include_files(inttypes.h INCLUDE_INTTYPES_H)
  check_include_files(stdint.h INCLUDE_STDINT_H)
  check_include_files(sys/types.h INCLUDE_SYS_TYPES_H)

  set(SIZE16 int16_t)
  set(USIZE16 uint16_t)
  set(SIZE32 int32_t)
  set(USIZE32 uint32_t)
  set(SIZE64 int64_t)

  configure_file(include/ogg/config_types.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ogg/config_types.h @ONLY)
endif ()

if(OGG_BUILD_STATIC)
	add_library(ogg-static STATIC ${OGG_SRC})
	target_include_directories(ogg-static PUBLIC include)
endif()

if(OGG_BUILD_SHARED)
	add_library(ogg SHARED ${OGG_SRC} win32/ogg.def)
	target_include_directories(ogg PUBLIC include)
endif()
