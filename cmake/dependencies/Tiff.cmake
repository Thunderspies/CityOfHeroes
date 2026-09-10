include_guard(GLOBAL)
include(CPM)
include(dependencies/Jpeg)
include(dependencies/Zlib)

CPMAddPackage(
	NAME tiff
	VERSION 4.0.9
	URL "https://download.osgeo.org/libtiff/tiff-4.0.9.tar.gz"
	URL_HASH
		SHA256=6e7bdeec2c310734e734d19aae3a71ebe37a4d842e0e23dbb1b8921c0026cfcd
	DOWNLOAD_ONLY YES
)

# Generate the pinned templates with the vendored Windows feature set. The
# upstream project also configures tools, installation, and optional codecs.
function(cox_add_tiff)
	if(NOT WIN32)
		message(FATAL_ERROR "The TIFF adapter requires Windows")
	endif()
	set(source_dir "${tiff_SOURCE_DIR}/libtiff")
	set(include_dir "${tiff_BINARY_DIR}/include")

	# Explicitly reset optional template features so parent directory probes
	# cannot silently enable a different codec or platform implementation.
	foreach(header tif_config tiffconf)
		file(STRINGS "${source_dir}/${header}.h.cmake.in" features
			REGEX "^#cmakedefine ")
		foreach(feature IN LISTS features)
			string(REGEX REPLACE "^#cmakedefine ([A-Z0-9_]+).*" "\\1"
				feature "${feature}")
			set(${feature} 0)
		endforeach()
	endforeach()
	foreach(feature
		CCITT_SUPPORT JPEG_SUPPORT LOGLUV_SUPPORT LZW_SUPPORT
		NEXT_SUPPORT OJPEG_SUPPORT PACKBITS_SUPPORT PIXARLOG_SUPPORT
		THUNDER_SUPPORT ZIP_SUPPORT STRIPCHOP_DEFAULT SUBIFD_SUPPORT
		DEFAULT_EXTRASAMPLE_AS_ALPHA CHECK_JPEG_YCBCR_SUBSAMPLING
		MDI_SUPPORT HAVE_IEEEFP HAVE_ASSERT_H HAVE_FCNTL_H HAVE_FLOOR
		HAVE_INTTYPES_H HAVE_IO_H HAVE_LFIND HAVE_LIMITS_H HAVE_MALLOC_H
		HAVE_MEMMOVE HAVE_MEMORY_H HAVE_MEMSET HAVE_POW HAVE_SEARCH_H
		HAVE_SETMODE HAVE_SNPRINTF HAVE_SQRT HAVE_STDINT_H HAVE_STRCHR
		HAVE_STRING_H HAVE_STRRCHR HAVE_STRSTR HAVE_STRTOL HAVE_STRTOUL
		HAVE_STRTOULL HAVE_SYS_TYPES_H)
		set(${feature} 1)
	endforeach()
	set(TIFF_INT8_T "signed char")
	set(TIFF_UINT8_T "unsigned char")
	foreach(type SIGNED UNSIGNED)
		string(TOLOWER "${type}" c_type)
		set(SIZEOF_${type}_SHORT 2)
		set(SIZEOF_${type}_INT 4)
		set(SIZEOF_${type}_LONG 4)
		set(SIZEOF_${type}_LONG_LONG 8)
		if(type STREQUAL "SIGNED")
			set(prefix TIFF_INT)
		else()
			set(prefix TIFF_UINT)
		endif()
		set(${prefix}16_T "${c_type} short")
		set(${prefix}32_T "${c_type} int")
		set(${prefix}64_T "${c_type} long long")
	endforeach()
	# The vendored configuration was generated for x64. Follow the pinned
	# Windows template: TIFFClientOpen requires tmsize_t to match pointers.
	set(TIFF_SIZE_T "unsigned long")
	set(TIFF_SSIZE_T "signed long long")
	set(TIFF_SSIZE_FORMAT "%lld")
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(TIFF_SSIZE_T "signed int")
		set(TIFF_SSIZE_FORMAT "%d")
	endif()
	set(TIFF_PTRDIFF_T ptrdiff_t)
	set(SIZEOF_UNSIGNED_CHAR_P "${CMAKE_SIZEOF_VOID_P}")
	set(TIFF_INT32_FORMAT "%d")
	set(TIFF_UINT32_FORMAT "%u")
	set(TIFF_INT64_FORMAT "%lld")
	set(TIFF_UINT64_FORMAT "%llu")
	set(TIFF_SIZE_FORMAT "%lu")
	set(TIFF_PTRDIFF_FORMAT "%td")
	set(HOST_FILLORDER FILLORDER_MSB2LSB)
	set(HOST_BIG_ENDIAN 0)
	set(STRIP_SIZE_DEFAULT 8192)
	set(INLINE_KEYWORD inline)
	set(FILE_OFFSET_BITS 64)
	set(LIBJPEG_12_PATH "")
	set(PACKAGE_NAME "LibTIFF Software")
	set(PACKAGE_BUGREPORT "tiff@lists.maptools.org")
	set(PACKAGE_STRING "LibTIFF Software 4.0.9")
	set(PACKAGE_TARNAME tiff)
	set(PACKAGE_URL "")
	set(PACKAGE_VERSION 4.0.9)
	configure_file("${source_dir}/tif_config.h.cmake.in"
		"${tiff_BINARY_DIR}/tif_config.h" @ONLY)
	configure_file("${source_dir}/tiffconf.h.cmake.in"
		"${include_dir}/libtiff/tiffconf.h" @ONLY)
	foreach(header tiff.h tiffio.h tiffvers.h)
		configure_file("${source_dir}/${header}"
			"${include_dir}/libtiff/${header}" COPYONLY)
	endforeach()

	# This is the vendored vcxproj manifest. It uses CRT file descriptors via
	# tif_unix.c, despite the dormant USE_WIN32_FILEIO in its private config.
	set(sources
		tif_aux.c tif_close.c tif_codec.c tif_color.c tif_compress.c
		tif_dir.c tif_dirinfo.c tif_dirread.c tif_dirwrite.c tif_dumpmode.c
		tif_error.c tif_extension.c tif_fax3.c tif_fax3sm.c tif_flush.c
		tif_getimage.c tif_jbig.c tif_jpeg.c tif_jpeg_12.c tif_luv.c
		tif_lzma.c tif_lzw.c tif_next.c tif_ojpeg.c tif_open.c tif_packbits.c
		tif_pixarlog.c tif_predict.c tif_print.c tif_read.c tif_strip.c
		tif_swab.c tif_thunder.c tif_tile.c tif_unix.c tif_version.c
		tif_warning.c tif_write.c tif_zip.c)
	list(TRANSFORM sources PREPEND "${source_dir}/")
	add_library(tiff STATIC EXCLUDE_FROM_ALL ${sources})
	add_library(TIFF::TIFF ALIAS tiff)
	target_include_directories(tiff
		PUBLIC "${include_dir}" "${include_dir}/libtiff"
		PRIVATE "${tiff_BINARY_DIR}" "${source_dir}")
	target_compile_definitions(tiff PUBLIC AVOID_WIN32_FILEIO)
	target_link_libraries(tiff PUBLIC JPEG::JPEG ZLIB::ZLIB)
	if(MSVC)
		target_compile_definitions(tiff PRIVATE
			_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
	endif()
	set_target_properties(tiff PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

cox_add_tiff()
