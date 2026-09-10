include_guard(GLOBAL)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86")
	message(FATAL_ERROR "The bundled AlienFX SDK requires MSVC Win32")
endif()

# Legacy SDK exception: retain the headers consumed by Game's optional
# hardware lighting support. The native LightFX.dll comes from the driver
# installation and is loaded dynamically by the consumer. The bundled
# Managed/DLL/LightFX.dll is a managed assembly, not a native replacement.
set(_alienfx_include
	"${CMAKE_CURRENT_LIST_DIR}/../../3rdparty/AlienFX SDK/Unmanaged/includes")
cmake_path(NORMAL_PATH _alienfx_include)
foreach(header IN ITEMS LFX2.h LFXDecl.h)
	if(NOT EXISTS "${_alienfx_include}/${header}")
		message(FATAL_ERROR
			"Incomplete bundled AlienFX SDK: missing ${header}")
	endif()
endforeach()

add_library(AlienFX::AlienFX INTERFACE IMPORTED GLOBAL)
set_target_properties(AlienFX::AlienFX PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${_alienfx_include}")
