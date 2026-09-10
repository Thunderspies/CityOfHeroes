include_guard(GLOBAL)
include(CPM)

if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "CubeMapGen requires MSVC Win32")
endif()

# AMD's library-only download contains binaries, not the library sources.
CPMAddPackage(
	NAME cubemapgen
	VERSION 1.4
	URL "https://gpuopen.com/download/CubeMapGen-1.4-Source.zip"
	URL_HASH
		"SHA256=fe05c945002d018e9ff2ace0f6a00c41806045abc28bb8dedb6da8df8fdab469"
	DOWNLOAD_ONLY YES
)

# These four library units match the fork's filtering implementation. The
# pristine header also enables HDR export, which requires HDRWrite.cpp.
# Consumers use only the filtering API; no GUI or sample program is built.
function(cox_add_cubemapgen)
	set(source_dir "${cubemapgen_SOURCE_DIR}/CubeMapGen-1.4-Source")
	set(sources CBBoxInt32.cpp CCubeMapProcessor.cpp CImageSurface.cpp
		ErrorMsg.cpp HDRWrite.cpp)
	list(TRANSFORM sources PREPEND "${source_dir}/")
	add_library(cubemapgen STATIC EXCLUDE_FROM_ALL ${sources})
	add_library(CubeMapGen::CubeMapGen ALIAS cubemapgen)

	# Preserve the existing namespaced include layout without editing headers.
	foreach(header CBBoxInt32.h CCubeMapProcessor.h CImageSurface.h
		ErrorMsg.h Types.h VectorMacros.h HDRWrite.h)
		configure_file("${source_dir}/${header}"
			"${cubemapgen_BINARY_DIR}/include/libcubemapgen/${header}"
			COPYONLY)
	endforeach()
	target_include_directories(cubemapgen PUBLIC
		"${cubemapgen_BINARY_DIR}/include")
	target_compile_definitions(cubemapgen PRIVATE UNICODE _UNICODE _LIB)
	target_link_libraries(cubemapgen PUBLIC user32)
	set_target_properties(cubemapgen PROPERTIES
		CXX_STANDARD 98
		CXX_STANDARD_REQUIRED YES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

cox_add_cubemapgen()
