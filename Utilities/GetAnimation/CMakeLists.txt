set(GetAnimation_SOURCES
	../../Common/seq/animtrack.c
	../../Common/seq/animtrackanimate.c
	../../Common/seq/bones.c
	../GetVrml/src/tree.c
	../GetVrml/src/vrml.c
	src/mainanim.c
	src/outputanim.c
	src/processanim.c
	src/process_anime.c
)

set(GetAnimation_HEADERS
	../../Common/seq/animtrack.h
	../../Common/seq/animtrackanimate.h
	../../Common/seq/bones.h
	../GetVrml/src/tree.h
	../GetVrml/src/vrml.h
	src/outputanim.h
	src/processanim.h
	src/process_anime.h
	src/Resource.h
)

set(GetAnimation_RESOURCES
	src/GetAnimation.rc
	src/GetAnimation.ico
	src/small.ico
)

# Shared bones inputs generate files in this target's own source directory.
# These generated C files are included by bones.c and process_anime.c.
set(GetAnimation_GENERATED
	src/AutoGen/bones_h_ast.c
	src/AutoGen/bones_h_ast.h
	src/AutoGen/process_anime_c_ast.c
	src/AutoGen/process_anime_c_ast.h
)

# StructParser emits these sidecars even without matching annotations.
# GetAnimation-specific names give every generated output one build owner.
list(APPEND GetAnimation_GENERATED
	src/AutoGen/GetAnimation_AutoTest.c
	src/AutoGen/GetAnimation_autogen_QueuedFuncs.c
	src/AutoGen/GetAnimation_autogen_QueuedFuncs.h
	src/AutoGen/GetAnimation_autorun_autogen.cpp
	src/AutoGen/GetAnimation_autorun_extrafunc_autogen.c
	src/AutoGen/GetAnimation_autotransactions_autogen.c
	src/AutoGen/GetAnimation_commands_autogen.c
	src/AutoGen/GetAnimation_commands_autogen_CommandFuncs.c
	src/AutoGen/GetAnimation_commands_autogen_CommandFuncs.h
	src/AutoGen/GetAnimation_latelink_autogen.c
	../../Common/AutoGen/GetAnimation_autogen_RemoteFuncs.c
	../../Common/AutoGen/GetAnimation_autogen_RemoteFuncs.h
	../../Common/AutoGen/GetAnimation_autogen_SlowFuncs.c
	../../Common/AutoGen/GetAnimation_autogen_SlowFuncs.h
)
set_source_files_properties(${GetAnimation_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(GetAnimation
	${GetAnimation_SOURCES} ${GetAnimation_HEADERS}
	${GetAnimation_RESOURCES} ${GetAnimation_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${GetAnimation_SOURCES} ${GetAnimation_HEADERS}
	${GetAnimation_RESOURCES} ${GetAnimation_GENERATED})
cox_target_defaults(GetAnimation)

target_include_directories(GetAnimation PRIVATE
	src ../GetVrml/src ../../Game/src)
target_link_libraries(GetAnimation PRIVATE Common UtilitiesLib)
target_compile_definitions(GetAnimation PRIVATE _CONSOLE UNICODE _UNICODE)
target_compile_options(GetAnimation PRIVATE
	$<$<AND:$<CONFIG:Release,OptDebug>,$<COMPILE_LANGUAGE:C,CXX>>:/Gy;/Oi;/Zi>)
target_link_options(GetAnimation PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(GetAnimation)
