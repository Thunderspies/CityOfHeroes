set(GetAnimation2_SOURCES
	../../Common/seq/animtrack.c
	../../Common/seq/animtrackanimate.c
	../../Common/seq/bones.c
	../GetVrml/src/tree.c
	../GetVrml/src/vrml.c
	src/mainanim.c
	src/outputanim.c
	src/processanim.c
	src/process_animx.c
	src/process_skelx.c
	src/process_vrml.c
)

set(GetAnimation2_HEADERS
	../../Common/seq/animtrack.h
	../../Common/seq/animtrackanimate.h
	../../Common/seq/bones.h
	../GetVrml/src/tree.h
	../GetVrml/src/vrml.h
	src/outputanim.h
	src/processanim.h
	src/process_animx.h
	src/process_skelx.h
	src/process_vrml.h
	src/Resource.h
)

set(GetAnimation2_RESOURCES
	src/GetAnimation2.rc
	src/GetAnimation2.ico
	src/small.ico
)

# Shared bones inputs generate files in this target's own source directory.
# These generated C files are included by the corresponding input sources.
set(GetAnimation2_GENERATED
	src/AutoGen/bones_h_ast.c
	src/AutoGen/bones_h_ast.h
	src/AutoGen/process_animx_c_ast.c
	src/AutoGen/process_animx_c_ast.h
	src/AutoGen/process_skelx_c_ast.c
	src/AutoGen/process_skelx_c_ast.h
)

# StructParser emits these sidecars even without matching annotations.
# GetAnimation2-specific names give every generated output one build owner.
list(APPEND GetAnimation2_GENERATED
	src/AutoGen/GetAnimation2_AutoTest.c
	src/AutoGen/GetAnimation2_autogen_QueuedFuncs.c
	src/AutoGen/GetAnimation2_autogen_QueuedFuncs.h
	src/AutoGen/GetAnimation2_autorun_autogen.cpp
	src/AutoGen/GetAnimation2_autorun_extrafunc_autogen.c
	src/AutoGen/GetAnimation2_autotransactions_autogen.c
	src/AutoGen/GetAnimation2_commands_autogen.c
	src/AutoGen/GetAnimation2_commands_autogen_CommandFuncs.c
	src/AutoGen/GetAnimation2_commands_autogen_CommandFuncs.h
	src/AutoGen/GetAnimation2_latelink_autogen.c
	../../Common/AutoGen/GetAnimation2_autogen_RemoteFuncs.c
	../../Common/AutoGen/GetAnimation2_autogen_RemoteFuncs.h
	../../Common/AutoGen/GetAnimation2_autogen_SlowFuncs.c
	../../Common/AutoGen/GetAnimation2_autogen_SlowFuncs.h
)
set_source_files_properties(${GetAnimation2_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(GetAnimation2
	${GetAnimation2_SOURCES} ${GetAnimation2_HEADERS}
	${GetAnimation2_RESOURCES} ${GetAnimation2_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${GetAnimation2_SOURCES} ${GetAnimation2_HEADERS}
	${GetAnimation2_RESOURCES} ${GetAnimation2_GENERATED})
cox_target_defaults(GetAnimation2)

target_include_directories(GetAnimation2 PRIVATE
	src ../GetVrml/src ../../Game/src)
target_link_libraries(GetAnimation2 PRIVATE Common UtilitiesLib)
target_compile_definitions(GetAnimation2 PRIVATE _CONSOLE UNICODE _UNICODE)
target_compile_options(GetAnimation2 PRIVATE
	$<$<AND:$<CONFIG:Release,OptDebug>,$<COMPILE_LANGUAGE:C,CXX>>:/Gy;/Oi;/Zi>)
target_link_options(GetAnimation2 PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(GetAnimation2)
