cmake_minimum_required(VERSION 2.8)

project(jpeg)

option(JPEG_BUILD_SHARED "Build shared library" ON)
option(JPEG_BUILD_STATIC "Build static library" ON)

set(JPEG_SRC
	jaricom.c
	jcapimin.c
	jcapistd.c
	jcarith.c
	jccoefct.c
	jccolor.c
	jcdctmgr.c
	jchuff.c
	jcinit.c
	jcmainct.c
	jcmarker.c
	jcmaster.c
	jcomapi.c
	jcparam.c
	jcprepct.c
	jcsample.c
	jctrans.c
	jdapimin.c
	jdapistd.c
	jdarith.c
	jdatadst.c
	jdatasrc.c
	jdcoefct.c
	jdcolor.c
	jddctmgr.c
	jdhuff.c
	jdinput.c
	jdmainct.c
	jdmarker.c
	jdmaster.c
	jdmerge.c
	jdpostct.c
	jdsample.c
	jdtrans.c
	jerror.c
	jfdctflt.c
	jfdctfst.c
	jfdctint.c
	jidctflt.c
	jidctfst.c
	jidctint.c
	jquant1.c
	jquant2.c
	jutils.c
	jmemmgr.c
	jmemansi.c
)

if(MSVC)
	file(COPY jconfig.vc DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/jconfig.vc ${CMAKE_CURRENT_BINARY_DIR}/jconfig.h)
endif()

if(JPEG_BUILD_STATIC)
	add_library(jpeg-static STATIC ${JPEG_SRC})
	target_include_directories(jpeg-static PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(JPEG_BUILD_SHARED)
	add_library(jpeg SHARED ${JPEG_SRC})
	target_include_directories(jpeg PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
endif()
