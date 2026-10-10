include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME enet
	GITHUB_REPOSITORY lsalzman/enet
	GIT_TAG v1.3.18
	PATCHES "${CMAKE_CURRENT_LIST_DIR}/../patches/enet-cmake.patch"
)

if(enet_ADDED)
	# ENet's directory-scope includes do not propagate to consumers.
	target_include_directories(enet PUBLIC
		$<BUILD_INTERFACE:${enet_SOURCE_DIR}/include>)
endif()
