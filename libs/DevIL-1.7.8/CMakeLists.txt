cmake_minimum_required(VERSION 2.8)

project(DevIL)

option(DEVIL_BUILD_SHARED "Build shared library" ON)
option(DEVIL_BUILD_STATIC "Build static library" OFF)

set(DEVIL_SRC
	src-IL/src/altivec_common.c
	src-IL/src/altivec_typeconversion.c
	src-IL/src/il_alloc.c
	src-IL/src/il_bits.c
	src-IL/src/il_blp.c
	src-IL/src/il_bmp.c
	src-IL/src/il_convbuff.c
	src-IL/src/il_convert.c
	src-IL/src/il_cut.c
	src-IL/src/il_dcx.c
	src-IL/src/il_dds-save.c
	src-IL/src/il_dds.c
	src-IL/src/il_devil.c
	src-IL/src/il_dicom.c
	src-IL/src/il_doom.c
	src-IL/src/il_dpx.c
	src-IL/src/il_endian.c
	src-IL/src/il_error.c
	src-IL/src/il_exr.cpp
	src-IL/src/il_fastconv.c
	src-IL/src/il_files.c
	src-IL/src/il_fits.c
	src-IL/src/il_ftx.c
	src-IL/src/il_gif.c
	src-IL/src/il_hdr.c
	src-IL/src/il_header.c
	src-IL/src/il_icns.c
	src-IL/src/il_icon.c
	src-IL/src/il_iff.c
	src-IL/src/il_ilbm.c
	src-IL/src/il_internal.c
	src-IL/src/il_io.c
	src-IL/src/il_iwi.c
	src-IL/src/il_jp2.c
	src-IL/src/il_jpeg.c
	src-IL/src/il_lif.c
	src-IL/src/il_main.c
	src-IL/src/il_manip.c
	src-IL/src/il_mdl.c
	src-IL/src/il_mng.c
	src-IL/src/il_mp3.c
	src-IL/src/il_neuquant.c
	src-IL/src/il_nvidia.cpp
	src-IL/src/il_pal.c
	src-IL/src/il_pcd.c
	src-IL/src/il_pcx.c
	src-IL/src/il_pic.c
	src-IL/src/il_pix.c
	src-IL/src/il_png.c
	src-IL/src/il_pnm.c
	src-IL/src/il_profiles.c
	src-IL/src/il_psd.c
	src-IL/src/il_psp.c
	src-IL/src/il_pxr.c
	src-IL/src/il_quantizer.c
	src-IL/src/il_raw.c
	src-IL/src/il_rawdata.c
	src-IL/src/il_register.c
	src-IL/src/il_rle.c
	src-IL/src/il_rot.c
	src-IL/src/il_sgi.c
	src-IL/src/il_size.c
	src-IL/src/il_squish.cpp
	src-IL/src/il_stack.c
	src-IL/src/il_states.c
	src-IL/src/il_sun.c
	src-IL/src/il_targa.c
	src-IL/src/il_texture.c
	src-IL/src/il_tiff.c
	src-IL/src/il_tpl.c
	src-IL/src/il_utility.c
	src-IL/src/il_utx.cpp
	src-IL/src/il_vtf.c
	src-IL/src/il_wal.c
	src-IL/src/il_wbmp.c
	src-IL/src/il_wdp.c
	src-IL/src/il_xpm.c
	src-IL/msvc9/il.def
)

if(MSVC)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/IL/config.h "// Placeholder")
endif()

add_definitions(
	-DIL_EXPORTS
	-DIL_NO_WDP
	-DIL_NO_MNG
	-DIL_NO_LCMS
	-DIL_NO_EXR
	# Bug in DevIL 1.7.8:
	-Dpng_set_gray_1_2_4_to_8=png_set_expand_gray_1_2_4_to_8
)

if (DEVIL_BUILD_STATIC)
	add_library(DevIL-static STATIC ${DEVIL_SRC})
	target_include_directories(DevIL-static PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})
	target_include_directories(DevIL-static PRIVATE src-IL/include)
	target_link_libraries(DevIL-static ${PNG_LIB_NAME_STATIC} jpeg-static jasper-static tiff-static winmm.lib)
endif()

if (DEVIL_BUILD_SHARED)
	add_library(DevIL SHARED ${DEVIL_SRC})
	target_include_directories(DevIL PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})
	target_include_directories(DevIL PRIVATE src-IL/include)
	target_link_libraries(DevIL ${PNG_LIB_NAME_STATIC} jpeg-static jasper-static tiff-static winmm.lib)
endif()
