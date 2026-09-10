include_guard(GLOBAL)
include(CPM)
include(dependencies/Jpeg)
include(dependencies/Png)

# The final 2.0 branch matches the unreleased 2.0.9 sources in the old build.
# Its VERSION file still says 2.0.8; the published 2.0.8 tag is older.
CPMAddPackage(
	NAME nvtt
	GITHUB_REPOSITORY castano/nvidia-texture-tools
	GIT_TAG c722267b9b0d691e1c0af5259db802fe3cc26d0c
	DOWNLOAD_ONLY YES
)

# Build the historical component manifests without upstream's tool programs,
# global flags, bundled dependency search paths, or installation rules.
function(cox_add_nvtt)
	set(HAVE_UNISTD_H 0)
	set(HAVE_STDARG_H 1)
	set(HAVE_SIGNAL_H 0)
	set(HAVE_EXECINFO_H 0)
	set(HAVE_MALLOC_H 1)
	set(HAVE_PNG 1)
	set(HAVE_JPEG 1)
	set(HAVE_TIFF 0)
	set(HAVE_OPENEXR 0)
	set(HAVE_MAYA 0)
	configure_file("${nvtt_SOURCE_DIR}/src/nvconfig.h.in"
		"${nvtt_BINARY_DIR}/nvconfig.h")

	set(core_sources
		Debug.cpp Library.cpp Memory.cpp Radix.cpp StrLib.cpp
		TextReader.cpp TextWriter.cpp poshlib/posh.c)
	set(math_sources
		Basis.cpp Montecarlo.cpp Plane.cpp Random.cpp
		SphericalHarmonic.cpp Triangle.cpp TriBox.cpp)
	set(image_sources
		BlockDXT.cpp ColorBlock.cpp DirectDrawSurface.cpp Filter.cpp
		FloatImage.cpp HoleFilling.cpp Image.cpp ImageIO.cpp
		NormalMap.cpp NormalMipmap.cpp Quantize.cpp)
	set(tt_sources
		CompressDXT.cpp CompressionOptions.cpp Compressor.cpp
		CompressRGB.cpp InputOptions.cpp nvtt.cpp nvtt_wrapper.cpp
		OptimalCompressDXT.cpp OutputOptions.cpp QuickCompressDXT.cpp
		cuda/CudaCompressDXT.cpp cuda/CudaUtils.cpp
		squish/colourblock.cpp squish/colourfit.cpp squish/colourset.cpp
		squish/maths.cpp squish/weightedclusterfit.cpp)
	foreach(component core math image tt)
		list(TRANSFORM ${component}_sources PREPEND
			"${nvtt_SOURCE_DIR}/src/nv${component}/")
		add_library(nv${component} STATIC EXCLUDE_FROM_ALL
			${${component}_sources})
		target_include_directories(nv${component} PUBLIC
			"${nvtt_BINARY_DIR}" "${nvtt_SOURCE_DIR}/src")
		target_compile_definitions(nv${component} PRIVATE
			_LIB __SSE2__ __SSE__ __MMX__)
		set_target_properties(nv${component} PROPERTIES
			CXX_STANDARD 98
			CXX_STANDARD_REQUIRED YES
			MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endforeach()
	add_library(NVTT::Core ALIAS nvcore)
	add_library(NVTT::Math ALIAS nvmath)
	add_library(NVTT::Image ALIAS nvimage)
	add_library(NVTT::NVTT ALIAS nvtt)

	target_link_libraries(nvmath PUBLIC NVTT::Core)
	target_link_libraries(nvimage PUBLIC
		NVTT::Math JPEG::JPEG PNG::PNG)
	target_link_libraries(nvtt PUBLIC NVTT::Image)
	target_include_directories(nvtt PRIVATE
		"${nvtt_SOURCE_DIR}/src/nvtt/squish")

	# NVTT declares legacy CRT aliases before including the standard headers.
	# Load the CRT declarations first, keeping the upstream sources unchanged.
	target_precompile_headers(nvcore PRIVATE
		"$<$<COMPILE_LANGUAGE:CXX>:<cstdio$<ANGLE-R>>"
		"$<$<COMPILE_LANGUAGE:CXX>:<cstdarg$<ANGLE-R>>")
	target_precompile_headers(nvtt PRIVATE <cstdio> <cstdarg>)
	# The old PNG reader accesses io_ptr directly, as the vendored build did.
	# Provide the pinned PNG structure declaration through the build headers.
	target_precompile_headers(nvimage PRIVATE
		<cstdio> <cstdarg> <png.h> <pngstruct.h>)
endfunction()

cox_add_nvtt()
