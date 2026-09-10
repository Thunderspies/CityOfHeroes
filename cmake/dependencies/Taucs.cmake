include_guard(GLOBAL)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	NOT (CMAKE_C_COMPILER_ARCHITECTURE_ID STREQUAL "X86" OR
		CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86"))
	message(FATAL_ERROR "The bundled TAUCS solver requires MSVC Win32")
endif()

# GetVrml's compatible TAUCS/BLAS/LAPACK binary stack remains a legacy SDK
# exception. The x86 static archives and generated ABI headers are paired;
# every build configuration uses the same original release archives.
set(_taucs_root "${CMAKE_CURRENT_LIST_DIR}/../../Utilities/GetVrml/src")
cmake_path(NORMAL_PATH _taucs_root)
foreach(header IN ITEMS
	taucs.h taucs_config_build.h taucs_config_tests.h taucs_private.h)
	if(NOT EXISTS "${_taucs_root}/${header}")
		message(FATAL_ERROR
			"Incomplete bundled TAUCS SDK: missing ${_taucs_root}/${header}")
	endif()
endforeach()

set(_taucs_libraries)
foreach(library IN ITEMS
	libtaucs blas_win32 lapack_win32 libatlas libcblas libf77blas
	liblapack libmetis vcf2c)
	set(path "${_taucs_root}/libtaucs/${library}.lib")
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "Incomplete bundled TAUCS SDK: missing ${path}")
	endif()
	add_library(TAUCS::${library} STATIC IMPORTED GLOBAL)
	set_target_properties(TAUCS::${library} PROPERTIES
		IMPORTED_LOCATION "${path}")
	list(APPEND _taucs_libraries TAUCS::${library})
endforeach()

# Consumers use /MT[d]. Resolve the archives' old stdio entry points through
# Microsoft's compatibility library and their CRT references through the
# active configuration's runtime. Their objects predate SafeSEH metadata.
# Matrix allocation/free and in-memory factorization are verified in both
# configurations; no legacy FILE interface or runtime DLL is introduced.
add_library(TAUCS::TAUCS INTERFACE IMPORTED GLOBAL)
set_target_properties(TAUCS::TAUCS PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${_taucs_root}"
	INTERFACE_LINK_LIBRARIES
		"${_taucs_libraries};legacy_stdio_definitions.lib"
	INTERFACE_LINK_OPTIONS
		"/SAFESEH:NO;$<$<CONFIG:Debug>:/NODEFAULTLIB:LIBCMT>")
