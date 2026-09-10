include_guard(GLOBAL)
include(CPM)
include(dependencies/Zlib)

# FreeType 2.1.9 (VER-2-1-9), matching the existing font engine and options.
CPMAddPackage(
	NAME freetype
	VERSION 2.1.9
	URL "https://codeload.github.com/freetype/freetype/tar.gz/9f182c1fe07d067cd7a6a1b74f53792cc7d203cd"
	URL_HASH
		"SHA256=06747364c9793483a36329635e1c77a68941b71de2e08d71d103dc5551d5fe39"
	DOWNLOAD_ONLY YES
)

# This release predates CMake. Keep the translation-unit manifest from the
# vendored Visual Studio project, including its original autohint module.
function(cox_add_freetype)
	set(sources
		src/autofit/autofit.c
		src/autohint/autohint.c
		src/base/ftbase.c
		src/base/ftglyph.c
		src/base/ftinit.c
		src/base/ftmm.c
		src/base/ftsystem.c
		src/bdf/bdf.c
		src/cache/ftcache.c
		src/cff/cff.c
		src/cid/type1cid.c
		src/gzip/ftgzip.c
		src/lzw/ftlzw.c
		src/pcf/pcf.c
		src/pfr/pfr.c
		src/psaux/psaux.c
		src/pshinter/pshinter.c
		src/psnames/psmodule.c
		src/raster/raster.c
		src/sfnt/sfnt.c
		src/smooth/smooth.c
		src/truetype/truetype.c
		src/type1/type1.c
		src/type42/type42.c
		src/winfonts/winfnt.c)
	if(WIN32)
		list(APPEND sources builds/win32/ftdebug.c)
	else()
		list(APPEND sources src/base/ftdebug.c)
	endif()
	list(TRANSFORM sources PREPEND "${freetype_SOURCE_DIR}/")
	add_library(freetype STATIC EXCLUDE_FROM_ALL ${sources})
	add_library(FreeType::FreeType ALIAS freetype)
	target_include_directories(freetype PUBLIC "${freetype_SOURCE_DIR}/include")
	# Retain gzip font support using the project's existing zlib target.
	target_compile_definitions(freetype PRIVATE
		FT_CONFIG_OPTION_SYSTEM_ZLIB
		$<$<CONFIG:Debug>:FT_DEBUG_LEVEL_ERROR>
		$<$<CONFIG:Debug>:FT_DEBUG_LEVEL_TRACE>)
	target_link_libraries(freetype PRIVATE ZLIB::ZLIB)
	if(MSVC)
		target_compile_definitions(freetype PRIVATE _CRT_SECURE_NO_WARNINGS)
		target_compile_options(freetype PRIVATE /wd4244 /wd4703)
	endif()
	set_target_properties(freetype PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

cox_add_freetype()
