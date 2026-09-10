include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME yajl
	VERSION 2.1.1
	GITHUB_REPOSITORY lloyd/yajl
	GIT_TAG 5e3a7856e643b4d6410ddc3f84bc2f38174f2872
	DOWNLOAD_ONLY YES
)

# Keep the vendored static manifest without upstream's global flag changes,
# shared library, command-line tools, shell tests, or installation rules.
function(cox_add_yajl)
	set(sources
		yajl.c yajl_alloc.c yajl_buf.c yajl_encode.c yajl_gen.c
		yajl_lex.c yajl_parser.c yajl_tree.c yajl_version.c)
	list(TRANSFORM sources PREPEND "${yajl_SOURCE_DIR}/src/")
	add_library(yajl_static STATIC EXCLUDE_FROM_ALL ${sources})
	add_library(YAJL::YAJL ALIAS yajl_static)

	set(include_dir "${yajl_BINARY_DIR}/include")
	foreach(header yajl_parse.h yajl_gen.h yajl_common.h yajl_tree.h)
		configure_file("${yajl_SOURCE_DIR}/src/api/${header}"
			"${include_dir}/yajl/${header}" COPYONLY)
	endforeach()
	set(YAJL_MAJOR 2)
	set(YAJL_MINOR 1)
	set(YAJL_MICRO 1)
	configure_file("${yajl_SOURCE_DIR}/src/api/yajl_version.h.cmake"
		"${include_dir}/yajl/yajl_version.h")
	target_include_directories(yajl_static PUBLIC "${include_dir}")
	if(MSVC)
		target_compile_definitions(yajl_static PRIVATE
			_CRT_SECURE_NO_WARNINGS)
	endif()
	set_target_properties(yajl_static PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

cox_add_yajl()
