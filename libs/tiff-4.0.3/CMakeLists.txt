cmake_minimum_required(VERSION 2.8)

project(tiff)

option(TIFF_BUILD_SHARED "Build shared library" OFF)
option(TIFF_BUILD_STATIC "Build static library" ON)

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
	libtiff/tif_write.c
	libtiff/tif_zip.c
)

if(MSVC)
	set(TIFF_SRC ${TIFF_SRC} libtiff/tif_win32.c)
	add_definitions (-DUSE_WIN32_FILEIO)
	file(COPY libtiff/tif_config.vc.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/tif_config.vc.h ${CMAKE_CURRENT_BINARY_DIR}/tif_config.h)
	file(COPY libtiff/tiffconf.vc.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/tiffconf.vc.h ${CMAKE_CURRENT_BINARY_DIR}/tiffconf.h)
elseif(APPLE)
	include(CheckFunctionExists)
	include(CheckIncludeFiles)
	include(CheckLibraryExists)
	include(CheckSymbolExists)
	include(CheckTypeSize)

	set(CCITT_SUPPORT 1)
	set(CHECK_JPEG_YCBCR_SUBSAMPLING 1)
	set(CXX_SUPPORT 1)
	set(DEFAULT_EXTRASAMPLE_AS_ALPHA 1)

	check_include_files(assert.h HAVE_ASSERT_H)
	check_include_files(dlfcn.h HAVE_DLFCN_H)
	check_include_files(fcntl.h HAVE_FCNTL_H)
	check_function_exists(floor HAVE_FLOOR)
	check_function_exists(getopt HAVE_GETOPT)

	set(HAVE_IEEEFP 1)

	check_include_files(inttypes.h HAVE_INTTYPES_H)
	check_include_files(isascii HAVE_ISASCII)
	check_function_exists(lfind HAVE_LFIND)

	check_library_exists(c "" "" HAVE_LIBC)
	check_library_exists(m "" "" HAVE_LIBM)

	check_include_files(limits.h HAVE_LIMITS_H)
	check_include_files(malloc.h HAVE_MALLOC_H)
	check_function_exists(memmove HAVE_MEMMOVE)

	check_include_files(memory.h HAVE_MEMORY_H)

	check_function_exists(memset HAVE_MEMSET)
	check_function_exists(mmap HAVE_MMAP)
	check_function_exists(pow HAVE_POW)

	# Define if you have POSIX threads libraries and header files.
	#undef HAVE_PTHREAD

	check_include_files(search.h HAVE_SEARCH_H)
	check_function_exists(sqrt HAVE_SQRT)
	check_include_files(stdint.h HAVE_STDINT_H)
	check_include_files(stdlib.h HAVE_STDLIB_H)
	check_function_exists(strcasecmp HAVE_STRCASECMP)
	check_function_exists(strchr HAVE_STRCHR)
	check_include_files(strings.h HAVE_STRINGS_H)
	check_include_files(string.h HAVE_STRING_H)
	check_function_exists(strrchr HAVE_STRRCHR)
	check_function_exists(strstr HAVE_STRSTR)
	check_function_exists(strtol HAVE_STRTOL)
	check_function_exists(strtoul HAVE_STRTOUL)
	check_include_files("sys/stat.h" HAVE_SYS_STAT_H)
	check_include_files("sys/time.h" HAVE_SYS_TIME_H)
	check_include_files("sys/types.h" HAVE_SYS_TYPES_H)
	check_include_files(unistd.h HAVE_UNISTD_H)
	check_include_files(windows.h HAVE_WINDOWS_H)

	# Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
	# (Intel)
	set(HOST_BIGENDIAN 0)

	# Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB)
	set(HOST_FILLORDER "FILLORDER_LSB2MSB")
	set(LOGLUV_SUPPORT 1)

	set(LZW_SUPPORT 1)
	set(MDI_SUPPORT 1)
	set(NEXT_SUPPORT 1)

	# Define to 1 if your C compiler doesn't accept -c and -o together.
	#undef NO_MINUS_C_MINUS_O

	# Support Old JPEG compresson (read contrib/ojpeg/README first!
	# Compilation fails with unpatched IJG JPEG library)
	#undef OJPEG_SUPPORT

	set(PACKAGE "tiff")
	set(PACKAGE_BUGREPORT "")
	set(PACKAGE_NAME "LibTIFF Software")
	set(PACKAGE_STRING "LibTIFF Software 4.0.3")
	set(PACKAGE_TARNAME "tiff")
	set(PACKAGE_VERSION "4.0.3")
	set(PACKBITS_SUPPORT 1)
	set(PIXARLOG_SUPPORT 1)

	#Define to necessary symbol if this constant uses a non-standard name onyour system.
	#undef PTHREAD_CREATE_JOINABLE

	check_type_size("int" SIZEOF_INT)
	check_type_size("long" SIZEOF_LONG)

	set(STDC_HEADERS 1)
	set(STRIPCHOP_DEFAULT "TIFF_STRIPCHOP")
	set(STRIP_SIZE_DEFAULT 8192)
	set(SUBIFD_SUPPORT 1)
	set(THUNDER_SUPPORT 1)
	set(TIME_WITH_SYS_TIME 1)

	check_symbol_exists("struct tm" "sys/time.h" TM_IN_SYS_TIME)
	set(VERSION "4.0.3")

	# Define to 1 if your processor stores words with the most significant byte first
	# (like Motorola and SPARC, unlike Intel and VAX).
	#undef WORDS_BIGENDIAN

	set(X_DISPLAY_MISSING 1)
	set(ZIP_SUPPORT 1)
	# Number of bits in a file offset, on hosts where this is settable.
	#undef _FILE_OFFSET_BITS

	# Define for large files, on AIX-style hosts.
	#undef _LARGE_FILES

	# Define to empty if `const' does not conform to ANSI C.
	set(const const)

	configure_file(libtiff/tif_config.h.cmake tif_config.h)
	configure_file(libtiff/tiffconf.h.cmake tiffconf.h)
	add_definitions(-DHAVE_CONFIG_H)
endif()

if(UNIX)
	set(TIFF_SRC ${TIFF_SRC} libtiff/tif_unix.c)
endif()

if (TIFF_BUILD_STATIC)
	add_library(tiff-static STATIC ${TIFF_SRC})
	target_include_directories(tiff-static PUBLIC libtiff ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (TIFF_BUILD_SHARED)
	add_library(tiff SHARED ${TIFF_SRC})
	target_include_directories(tiff PUBLIC libtiff ${CMAKE_CURRENT_BINARY_DIR})
endif()
