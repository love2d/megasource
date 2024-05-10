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
elseif(EMSCRIPTEN)
	add_definitions(-DOPT_GENERIC)

	include(CheckFunctionExists)
	include(CheckIncludeFiles)
	include(CheckLibraryExists)
	include(CheckSymbolExists)
	include(CheckTypeSize)

	check_include_files("alc.h" HAVE_ALC_H)
	check_include_files("Alib.h" HAVE_ALIB_H)
	check_include_files("AL/alc.h" HAVE_AL_ALC_H)
	check_include_files("AL/al.h" HAVE_AL_AL_H)
	check_include_files("al.h" HAVE_AL_H)
	check_include_files("arpa/inet.h" HAVE_ARPA_INET_H)
	check_include_files("asm/audioio.h" HAVE_ASM_AUDIOIO_H)
	check_function_exists(atoll HAVE_ATOLL)
	check_include_files("audios.h" HAVE_AUDIOS_H)
	check_include_files("AudioToolbox/AudioToolbox.h" HAVE_AUDIOTOOLBOX_AUDIOTOOLBOX_H)
	check_include_files("AudioUnit/AudioUnit.h" HAVE_AUDIOUNIT_AUDIOUNIT_H)
	check_include_files("CoreServices/CoreServices.h" HAVE_CORESERVICES_CORESERVICES_H)
	check_include_files("CUlib.h" HAVE_CULIB_H)
	check_include_files("dlfcn.h" HAVE_DLFCN_H)
	# /* Define if getaddrinfo accepts the AI_ADDRCONFIG flag */
	#cmakedefine HAVE_GAI_ADDRCONFIG
	check_function_exists(getaddrinfo HAVE_GETADDRINFO)
	check_function_exists(getpagesize HAVE_GETPAGESIZE)
	check_function_exists(getuid HAVE_GETUID)
	check_include_files("inttypes.h" HAVE_INTTYPES_H)
	check_include_files("langinfo.h" HAVE_LANGINFO_H)
	check_library_exists(m "" "" HAVE_LIBM)
	check_library_exists(mx "" "" HAVE_LIBMX)
	check_include_files("limits.h" HAVE_LIMITS_H)
	check_include_files("linux/soundcard.h" HAVE_LINUX_SOUNDCARD_H)
	check_include_files("locale.h" HAVE_LOCALE_H)
	check_library_exists(ltdl "" "" HAVE_LTDL)
	check_include_files("machine/soundcard.h" HAVE_MACHINE_SOUNDCARD_H)
	check_include_files("memory.h" HAVE_MEMORY_H)
	check_function_exists(mkfifo HAVE_MKFIFO)
	check_function_exists(mmap HAVE_MMAP)
	check_include_files("netdb.h" HAVE_NETDB_H)
	check_include_files("netinet/in.h" HAVE_NETINET_IN_H)
	check_include_files("netinet/tcp.h" HAVE_NETINET_TCP_H)
	check_function_exists(nl_langinfo HAVE_NL_LANGINFO)
	check_include_files("OpenAL/alc.h" HAVE_OPENAL_ALC_H)
	check_include_files("OpenAL/al.h" HAVE_OPENAL_AL_H)
	check_include_files("os2me.h" HAVE_OS2ME_H)
	check_include_files("os2.h" HAVE_OS2_H)
	check_function_exists(random HAVE_RANDOM)
	check_include_files("sched.h" HAVE_SCHED_H)
	check_function_exists(sched_setscheduler HAVE_SCHED_SETSCHEDULER)
	check_function_exists(setlocale HAVE_SETLOCALE)
	check_function_exists(setpriority HAVE_SETPRIORITY)
	check_function_exists(setuid HAVE_SETUID)
	check_include_files("signal.h" HAVE_SIGNAL_H)
	check_include_files("sndio.h" HAVE_SNDIO_H)
	check_include_files("stdint.h" HAVE_STDINT_H)
	check_include_files("stdio.h" HAVE_STDIO_H)
	check_include_files("stdlib.h" HAVE_STDLIB_H)
	check_function_exists(strdup HAVE_STRDUP)
	check_function_exists(strerror HAVE_STRERROR)
	check_include_files("strings.h" HAVE_STRINGS_H)
	check_include_files("string.h" HAVE_STRING_H)
	check_include_files("sun/audioio.h" HAVE_SUN_AUDIOIO_H)
	check_include_files("sys/audioio.h" HAVE_SYS_AUDIOIO_H)
	check_include_files("sys/audio.h" HAVE_SYS_AUDIO_H)
	check_include_files("sys/ioctl.h" HAVE_SYS_IOCTL_H)
	check_include_files("sys/param.h" HAVE_SYS_PARAM_H)
	check_include_files("sys/resource.h" HAVE_SYS_RESOURCE_H)
	check_include_files("sys/signal.h" HAVE_SYS_SIGNAL_H)
	check_include_files("sys/socket.h" HAVE_SYS_SOCKET_H)
	check_include_files("sys/soundcard.h" HAVE_SYS_SOUNDCARD_H)
	check_include_files("sys/stat.h" HAVE_SYS_STAT_H)
	check_include_files("sys/time.h" HAVE_SYS_TIME_H)
	check_include_files("sys/types.h" HAVE_SYS_TYPES_H)
	check_include_files("sys/wait.h" HAVE_SYS_WAIT_H)
	# /* Define this if you have the POSIX termios library */
	#cmakedefine HAVE_TERMIOS
	check_include_files("unistd.h" HAVE_UNISTD_H)
	check_include_files("windows.h" HAVE_WINDOWS_H)
	check_include_files("ws2tcpip.h" HAVE_WS2TCPIP_H)

	set(MPG123_INCLUDE_DIRS ${MPG123_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})
	configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_BINARY_DIR}/config.h)
	configure_file(${PROJECT_SOURCE_DIR}/src/libmpg123/mpg123.h.in ${PROJECT_BINARY_DIR}/mpg123.h)        
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
	install(TARGETS mpg123 RUNTIME DESTINATION . LIBRARY DESTINATION . ARCHIVE DESTINATION .)
endif()
