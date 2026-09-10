include_guard(GLOBAL)
include(RuntimeStaging)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "The bundled PhysX SDK requires MSVC Win32")
endif()

set(_physx_root "${CMAKE_CURRENT_LIST_DIR}/../../3rdparty/PhysX")
cmake_path(NORMAL_PATH _physx_root)
set(_physx_includes "")
foreach(component IN ITEMS Physics Cooking Foundation PhysXLoader)
	list(APPEND _physx_includes "${_physx_root}/SDKs/${component}/include")
endforeach()
set(_physx_required ${_physx_includes}
	"${_physx_root}/SDKs/Physics/include/NxPhysics.h"
	"${_physx_root}/SDKs/Cooking/include/NxCooking.h"
	"${_physx_root}/SDKs/Foundation/include/NxVersionNumber.h"
	"${_physx_root}/SDKs/PhysXLoader/include/PhysXLoader.h")

foreach(component IN ITEMS Loader Cooking)
	add_library(PhysX::${component} SHARED IMPORTED GLOBAL)
	set_target_properties(PhysX::${component} PROPERTIES
		IMPORTED_CONFIGURATIONS "Debug;Release"
		INTERFACE_INCLUDE_DIRECTORIES "${_physx_includes}"
		MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
		MAP_IMPORTED_CONFIG_OPTDEBUG Release
		MAP_IMPORTED_CONFIG_MINSIZEREL Release
		IMPORTED_IMPLIB
			"${_physx_root}/SDKs/lib/Win32/PhysX${component}.lib"
		IMPORTED_LOCATION
			"${_physx_root}/bin/Release/PhysX${component}.dll")
	foreach(config IN ITEMS Debug Release)
		set(suffix "")
		if(config STREQUAL "Debug")
			set(suffix CHECKED)
		endif()
		set(library
			"${_physx_root}/SDKs/lib/Win32/PhysX${component}${suffix}.lib")
		set(dll "${_physx_root}/bin/${config}/PhysX${component}${suffix}.dll")
		string(TOUPPER "${config}" config_upper)
		set_target_properties(PhysX::${component} PROPERTIES
			IMPORTED_IMPLIB_${config_upper} "${library}"
			IMPORTED_LOCATION_${config_upper} "${dll}")
		list(APPEND _physx_required "${library}" "${dll}")
	endforeach()
endforeach()

# Loader also opens DLLs dynamically; TARGET_RUNTIME_DLLS cannot discover them.
set(_physx_runtime "")
foreach(component IN ITEMS NxCharacter PhysXCooking PhysXCore PhysXLoader)
	set(debug_dll "${_physx_root}/bin/Debug/${component}CHECKED.dll")
	set(release_dll "${_physx_root}/bin/Release/${component}.dll")
	list(APPEND _physx_required "${debug_dll}" "${release_dll}")
	set(runtime_dll "$<IF:$<CONFIG:Debug>,${debug_dll},${release_dll}>")
	list(APPEND _physx_runtime "${runtime_dll}")
	if(component STREQUAL "PhysXCore")
		set_property(TARGET PhysX::Loader PROPERTY COX_CORE_DLL
			"${runtime_dll}")
	endif()
endforeach()
foreach(dll IN ITEMS
	cudart32_30_9.dll NxCooking.dll physxcudart_20.dll PhysXDevice.dll)
	list(APPEND _physx_runtime "${_physx_root}/bin/Shared/${dll}")
	list(APPEND _physx_required "${_physx_root}/bin/Shared/${dll}")
endforeach()
foreach(path IN LISTS _physx_required)
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "Incomplete bundled PhysX SDK: missing ${path}")
	endif()
endforeach()
set_property(TARGET PhysX::Loader PROPERTY
	COX_RUNTIME_FILES "${_physx_runtime}")

# Stage all PhysX runtime files beside target on every requested build. Copies
# preserve timestamps when unchanged and restore deleted DLLs without relinking.
function(cox_stage_physx target)
	get_target_property(runtime_files PhysX::Loader COX_RUNTIME_FILES)
	get_target_property(core_dll PhysX::Loader COX_CORE_DLL)
	# Even the CHECKED loader requests unsuffixed Core/Cooking filenames.
	# Keep the checked names as well for consumers of their import libraries.
	cox_stage_runtime_files(${target} PhysX FILES ${runtime_files}
		RENAMED "${core_dll}|PhysXCore.dll"
			"$<TARGET_FILE:PhysX::Cooking>|PhysXCooking.dll")
endfunction()
