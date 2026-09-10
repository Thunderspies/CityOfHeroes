include_guard(GLOBAL)

if(NOT WIN32 OR NOT MSVC OR WINDOWS_STORE OR
	NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES "^(X86|x64)$")
	message(FATAL_ERROR "D3DX headers require desktop MSVC x86 or x64")
endif()

include(CPM)
CPMAddPackage(
	NAME dxsdk_d3dx
	VERSION 9.29.952.8
	URL "https://www.nuget.org/api/v2/package/Microsoft.DXSDK.D3DX/9.29.952.8"
	URL_HASH
		"SHA256=ead0906ae8a26c18a7525da7490127a2110f7c58f18293738283e30e97c6ea4b"
	DOWNLOAD_NAME dxsdk-d3dx.zip
	DOWNLOAD_ONLY YES
)

set(_d3dx_include "${dxsdk_d3dx_SOURCE_DIR}/build/native/include")
foreach(header IN ITEMS d3dx9.h d3dx9math.h d3dx9math.inl d3dx9core.h
	d3dx9xof.h d3dx9mesh.h d3dx9shader.h d3dx9effect.h d3dx9tex.h
	d3dx9shape.h d3dx9anim.h)
	if(NOT EXISTS "${_d3dx_include}/${header}")
		message(FATAL_ERROR "Incomplete D3DX package: missing ${header}")
	endif()
endforeach()

# June 2010 D3DX with Microsoft's modern Windows SDK header updates.
# GetVrml only consumes inline vector math and supplies its own normalization.
# Do not import the NuGet MSBuild targets, libraries, or runtime DLLs.
add_library(DXSDK::D3DXHeaders INTERFACE IMPORTED GLOBAL)
set_target_properties(DXSDK::D3DXHeaders PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${_d3dx_include}")
