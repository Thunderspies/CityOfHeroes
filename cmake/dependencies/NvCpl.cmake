include_guard(GLOBAL)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86")
	message(FATAL_ERROR "The bundled NvCpl SDK requires MSVC Win32")
endif()

# Legacy SDK exception: Game probes the optional NVIDIA control-panel API
# through these headers. NVCPL.dll belongs to the installed display driver;
# the consumer loads it dynamically and tolerates its absence. Do not stage it.
set(_nvcpl_include "${CMAKE_CURRENT_LIST_DIR}/../../3rdparty/nvcpl")
cmake_path(NORMAL_PATH _nvcpl_include)
foreach(header IN ITEMS NvPanelApi.h NvApiError.h)
	if(NOT EXISTS "${_nvcpl_include}/${header}")
		message(FATAL_ERROR
			"Incomplete bundled NvCpl SDK: missing ${header}")
	endif()
endforeach()

add_library(NvCpl::NvCpl INTERFACE IMPORTED GLOBAL)
set_target_properties(NvCpl::NvCpl PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${_nvcpl_include}")
