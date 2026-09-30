include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME chromium_zlib
	GIT_REPOSITORY
		"https://chromium.googlesource.com/chromium/src/third_party/zlib"
	GIT_TAG cf333892c34ab6dbaed37c38a1d5aa6da7b2f99d
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/chromium-zlib-cmake.patch"
)

# Piggle links zlib-ng alongside Chromium zlib. Keep Chromium's internal CPU
# dispatch symbol private to this implementation to avoid a duplicate definition.
target_compile_definitions(chromium_zlib PRIVATE
	cpu_check_features=cox_chromium_zlib_cpu_check_features
)
