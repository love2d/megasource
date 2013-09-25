cmake_minimum_required(VERSION 2.8)

project(tiff)

OPTION(TIFF_BUILD_SHARED "Build shared library" TRUE)
OPTION(TIFF_BUILD_STATIC "Build static library" TRUE)

set(TIFF_SRC
	libtiff/tif_aux.c
	libtiff/tif_close.c
	libtiff/tif_codec.c
	libtiff/tif_color.c
	libtiff/tif_compress.c
	libtiff/tif_dir.c
	libtiff/tif_dirinfo.c
	libtiff/tif_dirread.c
	libtiff/tif_dirwrite.c
	libtiff/tif_dumpmode.c
	libtiff/tif_error.c
	libtiff/tif_extension.c
	libtiff/tif_fax3.c
	libtiff/tif_fax3sm.c
	libtiff/tif_flush.c
	libtiff/tif_getimage.c
	libtiff/tif_jbig.c
	libtiff/tif_jpeg.c
	libtiff/tif_jpeg_12.c
	libtiff/tif_luv.c
	libtiff/tif_lzma.c
	libtiff/tif_lzw.c
	libtiff/tif_next.c
	libtiff/tif_ojpeg.c
	libtiff/tif_open.c
	libtiff/tif_packbits.c
	libtiff/tif_pixarlog.c
	libtiff/tif_predict.c
	libtiff/tif_print.c
	libtiff/tif_read.c
	libtiff/tif_stream.cxx
	libtiff/tif_strip.c
	libtiff/tif_swab.c
	libtiff/tif_thunder.c
	libtiff/tif_tile.c
	libtiff/tif_version.c
	libtiff/tif_warning.c
	libtiff/tif_win32.c
	libtiff/tif_write.c
	libtiff/tif_zip.c
)

if(MSVC)
	file(COPY libtiff/tif_config.vc.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/tif_config.vc.h ${CMAKE_CURRENT_BINARY_DIR}/tif_config.h)
	file(COPY libtiff/tiffconf.vc.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/tiffconf.vc.h ${CMAKE_CURRENT_BINARY_DIR}/tiffconf.h)
endif()

if (TIFF_BUILD_STATIC)
	add_library(tiff-static STATIC ${TIFF_SRC})
	target_include_directories(tiff-static PUBLIC libtiff ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (TIFF_BUILD_SHARED)
	add_library(tiff SHARED ${TIFF_SRC})
	target_include_directories(tiff PUBLIC libtiff ${CMAKE_CURRENT_BINARY_DIR})
endif()
