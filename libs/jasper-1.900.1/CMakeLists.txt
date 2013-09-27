cmake_minimum_required(VERSION 2.8)

project(jasper)

option(JASPER_BUILD_SHARED "Build shared library" ON)
option(JASPER_BUILD_STATIC "Build static library" ON)

set(JASPER_SRC
	src/libjasper/base/jas_cm.c
	src/libjasper/base/jas_debug.c
	src/libjasper/base/jas_getopt.c
	src/libjasper/base/jas_icc.c
	src/libjasper/base/jas_iccdata.c
	src/libjasper/base/jas_image.c
	src/libjasper/base/jas_init.c
	src/libjasper/base/jas_malloc.c
	src/libjasper/base/jas_seq.c
	src/libjasper/base/jas_stream.c
	src/libjasper/base/jas_string.c
	src/libjasper/base/jas_tmr.c
	src/libjasper/base/jas_tvp.c
	src/libjasper/base/jas_version.c
	src/libjasper/bmp/bmp_cod.c
	src/libjasper/bmp/bmp_dec.c
	src/libjasper/bmp/bmp_enc.c
	src/libjasper/jp2/jp2_cod.c
	src/libjasper/jp2/jp2_dec.c
	src/libjasper/jp2/jp2_enc.c
	src/libjasper/jpc/jpc_bs.c
	src/libjasper/jpc/jpc_cs.c
	src/libjasper/jpc/jpc_dec.c
	src/libjasper/jpc/jpc_enc.c
	src/libjasper/jpc/jpc_math.c
	src/libjasper/jpc/jpc_mct.c
	src/libjasper/jpc/jpc_mqcod.c
	src/libjasper/jpc/jpc_mqdec.c
	src/libjasper/jpc/jpc_mqenc.c
	src/libjasper/jpc/jpc_qmfb.c
	src/libjasper/jpc/jpc_t1cod.c
	src/libjasper/jpc/jpc_t1dec.c
	src/libjasper/jpc/jpc_t1enc.c
	src/libjasper/jpc/jpc_t2cod.c
	src/libjasper/jpc/jpc_t2dec.c
	src/libjasper/jpc/jpc_t2enc.c
	src/libjasper/jpc/jpc_tagtree.c
	src/libjasper/jpc/jpc_tsfb.c
	src/libjasper/jpc/jpc_util.c
	src/libjasper/jpg/jpg_dummy.c
	src/libjasper/jpg/jpg_val.c
	src/libjasper/mif/mif_cod.c
	src/libjasper/pgx/pgx_cod.c
	src/libjasper/pgx/pgx_dec.c
	src/libjasper/pgx/pgx_enc.c
	src/libjasper/pnm/pnm_cod.c
	src/libjasper/pnm/pnm_dec.c
	src/libjasper/pnm/pnm_enc.c
	src/libjasper/ras/ras_cod.c
	src/libjasper/ras/ras_dec.c
	src/libjasper/ras/ras_enc.c
)

if(MSVC)
	add_definitions(-DJAS_WIN_MSVC_BUILD)
endif()

if (JASPER_BUILD_STATIC)
	add_library(jasper-static STATIC ${JASPER_SRC})
	target_include_directories(jasper-static PUBLIC src/libjasper/include)
endif()

if (JASPER_BUILD_SHARED)
	add_library(jasper SHARED ${JASPER_SRC})
	target_include_directories(jasper PUBLIC src/libjasper/include)
endif()
