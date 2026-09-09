include_guard(GLOBAL)
include(CPM)
include(dependencies/Zlib)

# Minizip 1.1 with upstream Windows SDK and 64-bit seek fixes. The zlib
# sources in this archive are not built; use the shared pinned ZLIB::ZLIB.
CPMAddPackage(
	NAME minizip
	VERSION 1.1
	URL "https://github.com/madler/zlib/archive/05527a1b1ef39014b44281fbbbec5deabb4be106.tar.gz"
	URL_HASH
		"SHA256=20cac8a3feb6059b44e9cf74db86a8a9170e9e2976961d7ffbc67420066bd4a7"
	SOURCE_SUBDIR contrib/minizip
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/minizip-cmake.patch"
)
