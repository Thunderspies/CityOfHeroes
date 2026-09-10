include_guard(GLOBAL)
include(RuntimeStaging)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86")
	message(FATAL_ERROR "The bundled Cg SDK requires MSVC Win32")
endif()

# NVIDIA Cg 2.2 February 2010 remains a legacy SDK exception. The original
# Win32 project uses these same import libraries and DLLs in every config.
set(_cg_root "${CMAKE_CURRENT_LIST_DIR}/../../3rdparty/cg")
cmake_path(NORMAL_PATH _cg_root)
foreach(path IN ITEMS
	"${_cg_root}/include/Cg/cg.h" "${_cg_root}/include/Cg/cgGL.h"
	"${_cg_root}/lib/cg.lib" "${_cg_root}/lib/cgGL.lib"
	"${_cg_root}/bin/cg.dll" "${_cg_root}/bin/cgGL.dll")
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "Incomplete bundled Cg SDK: missing ${path}")
	endif()
endforeach()

foreach(component IN ITEMS Cg CgGL)
	if(component STREQUAL "Cg")
		set(library cg)
	else()
		set(library cgGL)
	endif()
	add_library(Cg::${component} SHARED IMPORTED GLOBAL)
	set_target_properties(Cg::${component} PROPERTIES
		IMPORTED_IMPLIB "${_cg_root}/lib/${library}.lib"
		IMPORTED_LOCATION "${_cg_root}/bin/${library}.dll"
		INTERFACE_INCLUDE_DIRECTORIES "${_cg_root}/include")
endforeach()
set_property(TARGET Cg::CgGL PROPERTY INTERFACE_LINK_LIBRARIES Cg::Cg)

# Run for every requested consumer build so a deleted DLL is restored even
# when its executable is already up to date. Unchanged copies keep timestamps.
function(cox_stage_cg target)
	cox_stage_runtime_files(${target} Cg FILES
		"$<TARGET_FILE:Cg::Cg>" "$<TARGET_FILE:Cg::CgGL>")
endfunction()
