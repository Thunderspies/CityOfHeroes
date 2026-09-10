include_guard(GLOBAL)
include(CPM)
include(dependencies/Zlib)

CPMAddPackage(
	NAME libpng
	VERSION 1.6.37
	GITHUB_REPOSITORY pnggroup/libpng
	GIT_TAG a40189cf881e9f0db80511c382292a5604c3c3d1
	DOWNLOAD_ONLY YES
)

# Preserve the vendored project manifest and prebuilt feature configuration.
# The upstream project would generate a different configuration and add tools.
function(cox_add_png)
	set(sources
		png.c pngerror.c pngget.c pngmem.c pngpread.c pngread.c
		pngrio.c pngrtran.c pngrutil.c pngset.c pngtrans.c pngwio.c
		pngwrite.c pngwtran.c pngwutil.c)
	list(TRANSFORM sources PREPEND "${libpng_SOURCE_DIR}/")
	add_library(png_static STATIC EXCLUDE_FROM_ALL ${sources})
	add_library(PNG::PNG ALIAS png_static)

	# NVTT needs the same private struct layout as the library. Export the
	# pinned headers in both the flat and legacy libpng/ include layouts.
	set(include_dir "${libpng_BINARY_DIR}/include")
	foreach(header png.h pngconf.h pngstruct.h)
		configure_file("${libpng_SOURCE_DIR}/${header}"
			"${include_dir}/libpng/${header}" COPYONLY)
	endforeach()
	configure_file("${libpng_SOURCE_DIR}/scripts/pnglibconf.h.prebuilt"
		"${include_dir}/libpng/pnglibconf.h" COPYONLY)
	target_include_directories(png_static
		PUBLIC "${include_dir}" "${include_dir}/libpng"
		PRIVATE "${libpng_SOURCE_DIR}")
	target_link_libraries(png_static PUBLIC ZLIB::ZLIB)
	target_compile_definitions(png_static PRIVATE
		$<$<CONFIG:Debug>:PNG_DEBUG=1>)
	if(MSVC)
		target_compile_definitions(png_static PRIVATE _CRT_SECURE_NO_WARNINGS)
	endif()
	set_target_properties(png_static PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

cox_add_png()
