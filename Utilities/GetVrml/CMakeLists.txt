include(dependencies/D3dx)
include(dependencies/Taucs)

set(GetVrml_SOURCES
	../../Common/gridcoll/grid.c
	../../Common/gridcoll/tritri_isectline.c
	../../Common/seq/anim.c
	../../Common/seq/animtrack.c
	../../Common/seq/AutoLOD.c
	../../Common/seq/bones.c
	../../Common/seq/tricks.c
	../GetTex/src/texloaddesc.c
	src/clip.c
	src/geo.c
	src/groupwrite.c
	src/main.c
	src/manifold.c
	src/NVMeshMender.cpp
	src/NVMeshMenderWrapper.cpp
	src/output.c
	src/poly.c
	src/texsort.c
	src/tree.c
	src/tricututils.c
	src/uvunwrap.c
	src/vrml.c
)

set(GetVrml_HEADERS
	../../Common/gridcoll/ctri.h
	../../Common/gridcoll/tritri_isectline.h
	../../Common/seq/anim.h
	../../Common/seq/animtrack.h
	../../Common/seq/AutoLOD.h
	../../Common/seq/bones.h
	../../Common/seq/tricks.h
	../GetTex/src/texloaddesc.h
	src/clip.h
	src/combinedpoly.h
	src/geo.h
	src/manifold.h
	src/NVMeshMender.h
	src/NVMeshMenderWrapper.h
	src/output.h
	src/poly.h
	src/prim.h
	src/quadric.h
	src/quadric_internal.h
	src/taucs.h
	src/taucs_config_build.h
	src/taucs_config_tests.h
	src/taucs_private.h
	src/texsort.h
	src/tree.h
	src/tricututils.h
	src/uvpack.h
	src/uvrect.h
	src/uvunwrap.h
	src/vrml.h
)

# bones.c and bones.h include these target-local generated files.
set(GetVrml_GENERATED
	src/AutoGen/bones_h_ast.c
	src/AutoGen/bones_h_ast.h
)

# StructParser emits these sidecars even without matching annotations.
# GetVrml-specific names give every generated output one build owner.
list(APPEND GetVrml_GENERATED
	src/AutoGen/GetVrml_AutoTest.c
	src/AutoGen/GetVrml_autogen_QueuedFuncs.c
	src/AutoGen/GetVrml_autogen_QueuedFuncs.h
	src/AutoGen/GetVrml_autorun_autogen.cpp
	src/AutoGen/GetVrml_autorun_extrafunc_autogen.c
	src/AutoGen/GetVrml_autotransactions_autogen.c
	src/AutoGen/GetVrml_commands_autogen.c
	src/AutoGen/GetVrml_commands_autogen_CommandFuncs.c
	src/AutoGen/GetVrml_commands_autogen_CommandFuncs.h
	src/AutoGen/GetVrml_latelink_autogen.c
	../../Common/AutoGen/GetVrml_autogen_RemoteFuncs.c
	../../Common/AutoGen/GetVrml_autogen_RemoteFuncs.h
	../../Common/AutoGen/GetVrml_autogen_SlowFuncs.c
	../../Common/AutoGen/GetVrml_autogen_SlowFuncs.h
)
set_source_files_properties(${GetVrml_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(GetVrml
	${GetVrml_SOURCES} ${GetVrml_HEADERS} ${GetVrml_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${GetVrml_SOURCES} ${GetVrml_HEADERS} ${GetVrml_GENERATED})
cox_target_defaults(GetVrml)

target_include_directories(GetVrml PRIVATE src ../GetTex/src ../../Game/src)
target_link_libraries(GetVrml PRIVATE
	Common UtilitiesLib DXSDK::D3DXHeaders TAUCS::TAUCS)
target_compile_definitions(GetVrml PRIVATE OFFLINE=1 GETVRML=1 _CONSOLE _MBCS
	$<$<CONFIG:Debug>:TRACKED_MEMPOOL;INCLUDE_MEMCHECK>)
target_compile_options(GetVrml PRIVATE
	$<$<AND:$<CONFIG:Release,OptDebug>,$<COMPILE_LANGUAGE:C,CXX>>:/Gy;/Oi;/Zi>)
target_link_options(GetVrml PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(GetVrml)
