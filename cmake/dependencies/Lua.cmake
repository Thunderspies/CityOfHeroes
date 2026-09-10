include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME lua51
	VERSION 5.1.5
	URL "https://www.lua.org/ftp/lua-5.1.5.tar.gz"
	URL_HASH
		"SHA256=2640fc56a795f29d28ef15e13c34a47e223960b0240e8cb0a82d9b0738695333"
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/lua-cmake.patch"
)
