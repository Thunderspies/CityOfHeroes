include_guard(GLOBAL)
include(CPM)

# Keep upstream options and the pre-CMP0091 Ogg project's runtime policy local.
# Game owns decoder state; the upstream codecs own their allocations.
function(cox_add_vorbis)
	set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
	CPMAddPackage(
		NAME ogg
		GITHUB_REPOSITORY xiph/ogg
		GIT_TAG be05b13e98b048f0b5a0f5fa8ce514d56db5f822 # 1.3.6
		OPTIONS
			"BUILD_SHARED_LIBS OFF"
			"BUILD_TESTING OFF"
			"INSTALL_DOCS OFF"
			"INSTALL_PKG_CONFIG_MODULE OFF"
			"INSTALL_CMAKE_PACKAGE_MODULE OFF"
	)

	CPMAddPackage(
		NAME vorbis
		GIT_REPOSITORY "https://github.com/xiph/vorbis.git"
		GIT_TAG 0657aee69dec8508a0011f47f3b69d7538e9d262 # 1.3.7
		PATCHES "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../patches/vorbis-cmake.patch"
		OPTIONS
			"BUILD_SHARED_LIBS OFF"
			"INSTALL_CMAKE_PACKAGE_MODULE OFF"
	)

	foreach(target ogg vorbis vorbisfile vorbisenc)
		set_target_properties(${target} PROPERTIES
			MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endforeach()
	foreach(target vorbis vorbisfile vorbisenc)
		if(NOT TARGET Vorbis::${target})
			add_library(Vorbis::${target} ALIAS ${target})
		endif()
	endforeach()
endfunction()

cox_add_vorbis()
