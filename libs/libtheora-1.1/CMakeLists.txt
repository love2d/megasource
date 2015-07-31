cmake_minimum_required(VERSION 2.8)

project(theora)

option(THEORA_BUILD_SHARED "Build shared libtheora" OFF)
option(THEORA_BUILD_STATIC "Build static libtheora" ON)

set(THEORA_SRC
	lib/analyze.c
    lib/apiwrapper.c
    lib/bitpack.c
    lib/cpu.c
    lib/decapiwrapper.c
    lib/decinfo.c
    lib/decode.c
    lib/dequant.c
    lib/encapiwrapper.c
    lib/encfrag.c
    lib/encinfo.c
    lib/encode.c
    lib/encoder_disabled.c
    lib/enquant.c
    lib/fdct.c
    lib/fragment.c
    lib/huffdec.c
    lib/huffenc.c
    lib/idct.c
    lib/info.c
    lib/internal.c
    lib/mathops.c
    lib/mcenc.c
    lib/quant.c
    lib/rate.c
    lib/state.c
    lib/tokenize.c
)

if(THEORA_BUILD_STATIC)
	add_library(theora-static STATIC ${THEORA_SRC})
	target_include_directories(theora-static PUBLIC include lib)
	target_link_libraries(theora-static ${OGG_LIBRARY})
endif()

if(THEORA_BUILD_SHARED)
	add_library(theora SHARED ${THEORA_SRC} lib/theora.def)
	target_include_directories(theora PUBLIC include lib)
	target_link_libraries(theora ${OGG_LIBRARY})
endif()
