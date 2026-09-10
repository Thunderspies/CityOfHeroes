include(dependencies/Glew)
include(dependencies/Nvtt)

set(GetTex_SOURCES
	../../Common/seq/animtrackanimate.c
	../../Common/seq/AutoLOD.c
	../../Common/seq/tricks.c
	src/askOptions.c
	src/gettex.c
	src/mipmap.c
	src/tdither.c
	src/texloaddesc.c
	src/texture_tools.cpp
)

set(GetTex_HEADERS
	../../Common/seq/animtrackanimate.h
	../../Common/seq/AutoLOD.h
	../../Common/seq/tricks.h
	src/askOptions.h
	src/gettex.h
	src/mipmap.h
	src/resource.h
	src/tdither.h
	src/texloaddesc.h
	src/texture_tools.h
)

set(GetTex_RESOURCES src/GetTexResources.rc)
set(GetTex_IMAGES src/CrypticIcn.ico)

add_executable(GetTex
	${GetTex_SOURCES} ${GetTex_HEADERS} ${GetTex_RESOURCES} ${GetTex_IMAGES})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${GetTex_SOURCES} ${GetTex_HEADERS} ${GetTex_RESOURCES} ${GetTex_IMAGES})
cox_target_defaults(GetTex)

target_include_directories(GetTex PRIVATE src ../../Game/src)
target_link_libraries(GetTex PRIVATE Common UtilitiesLib GLEW::GLEW NVTT::NVTT)
target_compile_definitions(GetTex PRIVATE _CONSOLE _MBCS)
target_compile_options(GetTex PRIVATE
	$<$<AND:$<CONFIG:Release,OptDebug>,$<COMPILE_LANGUAGE:C,CXX>>:/Gy;/Oi;/Zi>)
target_link_options(GetTex PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
