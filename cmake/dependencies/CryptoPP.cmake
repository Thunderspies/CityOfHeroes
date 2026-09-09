include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME cryptopp
	GIT_REPOSITORY "https://github.com/weidai11/cryptopp"
	# First post-8.9.0 revision with the modern MSVC stdext fix (GH #1339).
	GIT_TAG b5242667a24e3db8e4600e77b2e502ef204e5280
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/cryptopp-cmake.patch"
)
