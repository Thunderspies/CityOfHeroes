include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME chromium_zlib
	GIT_REPOSITORY
		"https://chromium.googlesource.com/chromium/src/third_party/zlib"
	GIT_TAG cf333892c34ab6dbaed37c38a1d5aa6da7b2f99d
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/chromium-zlib-cmake.patch"
)
