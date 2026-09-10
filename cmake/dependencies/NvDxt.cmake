include_guard(GLOBAL)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86")
	message(FATAL_ERROR "The bundled nvDXT SDK requires MSVC Win32")
endif()

# The licensed binary SDK is a legacy dependency exception. These are the
# same static C/C++ runtime variants used by Game's original Win32 project.
set(_nvdxt_root "${CMAKE_CURRENT_LIST_DIR}/../../3rdparty/nvdxt")
cmake_path(NORMAL_PATH _nvdxt_root)
set(_nvdxt_debug "${_nvdxt_root}/Debug/nvDXTlibMT_Sd.lib")
set(_nvdxt_release "${_nvdxt_root}/Release/nvDXTlibMT_S.lib")
foreach(path IN ITEMS
	"${_nvdxt_root}/dxtlib.h" "${_nvdxt_root}/nvdxt_options.h"
	"${_nvdxt_root}/tPixel.h" "${_nvdxt_debug}" "${_nvdxt_release}")
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "Incomplete bundled nvDXT SDK: missing ${path}")
	endif()
endforeach()

# Consumers must use /MT[d] and supply the SDK's C++ ReadDTXnFile and
# WriteDTXnFile callbacks. No vendor DLL is required. The original Debug
# vc70.pdb is absent; its LNK4099 warnings remain visible to consumers.
add_library(NvDxt::NvDxt STATIC IMPORTED GLOBAL)
set_target_properties(NvDxt::NvDxt PROPERTIES
	IMPORTED_CONFIGURATIONS "Debug;Release"
	IMPORTED_LOCATION "${_nvdxt_release}"
	IMPORTED_LOCATION_DEBUG "${_nvdxt_debug}"
	IMPORTED_LOCATION_RELEASE "${_nvdxt_release}"
	MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
	MAP_IMPORTED_CONFIG_OPTDEBUG Release
	MAP_IMPORTED_CONFIG_MINSIZEREL Release
	INTERFACE_INCLUDE_DIRECTORIES "${_nvdxt_root}"
	INTERFACE_COMPILE_DEFINITIONS EXCLUDE_LIBS
	# Microsoft provides the pre-UCRT sprintf and _vsnprintf entry points.
	INTERFACE_LINK_LIBRARIES legacy_stdio_definitions.lib)
