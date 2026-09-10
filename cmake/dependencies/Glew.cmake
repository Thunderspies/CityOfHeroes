include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME glew
	VERSION 2.1.0
	URL "https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.zip"
	URL_HASH
		"SHA256=2700383d4de2455f06114fbaf872684f15529d4bdc5cdea69b5fb0e9aa7763f1"
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/glew-cmake.patch"
)
