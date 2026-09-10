include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME jpeg
	GITHUB_REPOSITORY libjpeg-turbo/libjpeg-turbo
	GIT_TAG ec32420f6b5dfa4e86883d42b209e8371e55aeb5 # 3.0.1
	DOWNLOAD_ONLY YES
)

# Upstream intentionally does not support add_subdirectory(). Build its static
# libjpeg API directly, without the TurboJPEG API, tools, tests, or installation.
# SIMD is deliberately disabled: the previous jpeg9c dependency used portable C
# too, and this avoids requiring NASM for consumers of the existing JPEG API.
function(cox_add_jpeg)
	set(VERSION 3.0.1)
	set(LIBJPEG_TURBO_VERSION_NUMBER 3000001)
	set(JPEG_LIB_VERSION 62)
	set(BUILD "3.0.1")
	set(COPYRIGHT_YEAR "1991-2023")
	set(CMAKE_PROJECT_NAME "libjpeg-turbo")
	set(SIZE_T "${CMAKE_SIZEOF_VOID_P}")
	set(C_ARITH_CODING_SUPPORTED 1)
	set(D_ARITH_CODING_SUPPORTED 1)
	set(WITH_SIMD 0)
	set(RIGHT_SHIFT_IS_UNSIGNED 0)
	set(HAVE_BUILTIN_CTZL 0)
	set(HAVE_INTRIN_H 0)
	if(MSVC)
		set(INLINE __forceinline)
		set(THREAD_LOCAL "__declspec(thread)")
		set(HAVE_INTRIN_H 1)
	else()
		set(INLINE inline)
		set(THREAD_LOCAL __thread)
	endif()

	foreach(header jconfig.h jconfigint.h jversion.h)
		configure_file("${jpeg_SOURCE_DIR}/${header}.in"
			"${jpeg_BINARY_DIR}/${header}" @ONLY)
	endforeach()

	# These are the pinned upstream JPEG16_SOURCES, JPEG12_SOURCES, and
	# JPEG_SOURCES manifests, including the default arithmetic codec support.
	set(jpeg16_sources
		jcapistd.c jccolor.c jcdiffct.c jclossls.c jcmainct.c jcprepct.c
		jcsample.c jdapistd.c jdcolor.c jddiffct.c jdlossls.c jdmainct.c
		jdpostct.c jdsample.c jutils.c)
	set(jpeg12_sources ${jpeg16_sources}
		jccoefct.c jcdctmgr.c jdcoefct.c jddctmgr.c jdmerge.c jfdctfst.c
		jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c
		jquant2.c)
	set(jpeg_sources ${jpeg12_sources}
		jcapimin.c jchuff.c jcicc.c jcinit.c jclhuff.c jcmarker.c jcmaster.c
		jcomapi.c jcparam.c jcphuff.c jctrans.c jdapimin.c jdatadst.c
		jdatasrc.c jdhuff.c jdicc.c jdinput.c jdlhuff.c jdmarker.c
		jdmaster.c jdphuff.c jdtrans.c jerror.c jfdctflt.c jmemmgr.c
		jmemnobs.c jaricom.c jcarith.c jdarith.c)
	foreach(sources jpeg16_sources jpeg12_sources jpeg_sources)
		list(TRANSFORM ${sources} PREPEND "${jpeg_SOURCE_DIR}/")
	endforeach()

	# The 8-bit API dispatches to these implementations for higher-precision
	# inputs, so their objects remain necessary even for 8-bit-only consumers.
	add_library(jpeg12-static OBJECT EXCLUDE_FROM_ALL ${jpeg12_sources})
	target_compile_definitions(jpeg12-static PRIVATE BITS_IN_JSAMPLE=12)
	add_library(jpeg16-static OBJECT EXCLUDE_FROM_ALL ${jpeg16_sources})
	target_compile_definitions(jpeg16-static PRIVATE BITS_IN_JSAMPLE=16)
	add_library(jpeg-static STATIC EXCLUDE_FROM_ALL ${jpeg_sources}
		$<TARGET_OBJECTS:jpeg12-static> $<TARGET_OBJECTS:jpeg16-static>)
	add_library(JPEG::JPEG ALIAS jpeg-static)

	foreach(target jpeg12-static jpeg16-static jpeg-static)
		target_include_directories(${target} PRIVATE
			"${jpeg_BINARY_DIR}" "${jpeg_SOURCE_DIR}")
		target_compile_definitions(${target} PRIVATE
			_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
		set_target_properties(${target} PROPERTIES
			MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endforeach()
	target_include_directories(jpeg-static INTERFACE
		"${jpeg_BINARY_DIR}" "${jpeg_SOURCE_DIR}")
endfunction()

cox_add_jpeg()
