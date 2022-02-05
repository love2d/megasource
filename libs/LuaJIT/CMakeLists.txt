cmake_minimum_required(VERSION 3.6)

project(LuaJIT LANGUAGES C)

if(ANDROID)
	add_library(luajit STATIC IMPORTED GLOBAL)
	set_target_properties(luajit PROPERTIES
		IMPORTED_LOCATION android/${ANDROID_ABI}/libluajit.a
		INCLUDE_DIRECTORIES src
	)
else()
	message(WARNING "This script is only for Android")
endif()
