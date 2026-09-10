include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME sparsehash
	VERSION 2.0.4
	GITHUB_REPOSITORY sparsehash/sparsehash
	GIT_TAG f93c0c69e959c1c77611d0ba8d107aa971338811
	DOWNLOAD_ONLY YES
)

function(cox_add_sparsehash)
	set(headers
		dense_hash_map dense_hash_set sparse_hash_map sparse_hash_set
		sparsetable template_util.h type_traits.h
		internal/densehashtable.h internal/hashtable-common.h
		internal/libc_allocator_with_realloc.h internal/sparsehashtable.h)
	list(TRANSFORM headers PREPEND "${sparsehash_SOURCE_DIR}/src/sparsehash/")

	# The old Windows configuration selects stdext::hash_compare. Both
	# supported toolchains provide the C++17 standard hash and integer types.
	# Generate outside CPM's immutable source cache for each build tree.
	set(include_dir "${sparsehash_BINARY_DIR}/include")
	set(config "${include_dir}/sparsehash/internal/sparseconfig.h")
	file(CONFIGURE OUTPUT "${config}" CONTENT [=[
#ifndef COX_SPARSEHASH_CONFIG_H
#define COX_SPARSEHASH_CONFIG_H
#define GOOGLE_NAMESPACE ::google
#define HASH_FUN_H <functional>
#define HASH_NAMESPACE std
#define SPARSEHASH_HASH HASH_NAMESPACE::hash
#define SPARSEHASH_HASH_NO_NAMESPACE hash
#define HAVE_INTTYPES_H 1
#define HAVE_LONG_LONG 1
#define HAVE_MEMCPY 1
#define HAVE_STDINT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UINT16_T 1
#define _END_GOOGLE_NAMESPACE_ }
#define _START_GOOGLE_NAMESPACE_ namespace google {
#endif
]=] @ONLY)

	add_library(sparsehash INTERFACE ${headers} "${config}")
	add_library(SparseHash::SparseHash ALIAS sparsehash)
	set_source_files_properties(${headers} "${config}"
		PROPERTIES HEADER_FILE_ONLY TRUE)
	target_include_directories(sparsehash INTERFACE
		"${include_dir}" "${sparsehash_SOURCE_DIR}/src")
	target_compile_features(sparsehash INTERFACE cxx_std_17)
endfunction()

cox_add_sparsehash()
