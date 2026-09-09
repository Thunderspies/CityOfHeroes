if(MINGW)
	# Reporting is loaded dynamically; callers handle an unavailable DLL.
	add_library(CrashRpt INTERFACE)
	target_include_directories(CrashRpt INTERFACE include)
	target_compile_definitions(CrashRpt INTERFACE CRASHRPT_STUB)
elseif(MSVC)
	enable_language(RC)
	add_library(CrashRpt SHARED
		src/CrashHandler.cpp
		src/CrashRpt.cpp
		src/pch.cpp
		src/Stackwalk.cpp
		src/CrashRpt.rc
		src/CrypticIcn.ico
		include/crashrpt/CrashRpt.h
		include/crashrpt/CrashRptConf.h
		include/crashrpt/Stackwalk.h
		src/CrashHandler.h
		src/CReportConduit.h
		src/pch.h
		src/resource.h
	)
	target_include_directories(CrashRpt PUBLIC include PRIVATE src)
	cox_target_optdebug(CrashRpt)
	target_precompile_headers(CrashRpt PRIVATE
		"$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>")
	target_compile_definitions(CrashRpt PRIVATE
		CRASHRPT_EXPORTS _WINDOWS _USRDLL
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		target_compile_definitions(CrashRpt PRIVATE WIN32)
	endif()
	target_compile_options(CrashRpt PRIVATE
		$<$<COMPILE_LANGUAGE:CXX>:/W3;/sdl;/permissive-;/std:c++latest>
		$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release,OptDebug>>:/Gy;/Oi>)
	# Match the DLL project's default /MD[d], independently of UtilitiesLib.
	set_target_properties(CrashRpt PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	target_link_options(CrashRpt PRIVATE /SUBSYSTEM:WINDOWS /MANIFESTUAC:NO)
	target_link_libraries(CrashRpt PRIVATE user32 advapi32)
else()
	message(FATAL_ERROR "CrashRpt requires MSVC or MinGW")
endif()
