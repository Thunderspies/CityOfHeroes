set(MissionServer_SOURCES
	../Common/mission/MissionSearch.c
	../Common/mission/missionserver_meta.c
	src/MissionServer.c
	src/MissionServerArcData.c
	src/MissionServerJournal.c
	src/MissionServerSearch.c
)

set(MissionServer_HEADERS
	../Common/mission/MissionSearch.h
	../Common/mission/missionserver_meta.h
	../Common/persist/persist.h
	src/MissionServer.h
	src/MissionServerArcData.h
	src/MissionServerJournal.h
	src/MissionServerSearch.h
)

# AST files are included by sources, not compiled as separate translation
# units. The persistence implementation remains owned by ServerLib.
set(MissionServer_GENERATED
	src/AutoGen/MissionSearch_h_ast.c
	src/AutoGen/MissionSearch_h_ast.h
	src/AutoGen/persist_h_ast.c
	src/AutoGen/persist_h_ast.h
	src/AutoGen/MissionServer_h_ast.c
	src/AutoGen/MissionServer_h_ast.h
	src/AutoGen/MissionServerJournal_c_ast.c
	src/AutoGen/MissionServerJournal_c_ast.h
	src/AutoGen/MissionServerSearch_h_ast.c
	src/AutoGen/MissionServerSearch_h_ast.h
)

# Declare the parser's unconditional target-specific outputs with one owner.
list(APPEND MissionServer_GENERATED
	src/AutoGen/MissionServer_AutoTest.c
	src/AutoGen/MissionServer_autogen_QueuedFuncs.c
	src/AutoGen/MissionServer_autogen_QueuedFuncs.h
	src/AutoGen/MissionServer_autorun_autogen.cpp
	src/AutoGen/MissionServer_autorun_extrafunc_autogen.c
	src/AutoGen/MissionServer_autotransactions_autogen.c
	src/AutoGen/MissionServer_commands_autogen.c
	src/AutoGen/MissionServer_commands_autogen_CommandFuncs.c
	src/AutoGen/MissionServer_commands_autogen_CommandFuncs.h
	src/AutoGen/MissionServer_latelink_autogen.c
	../Common/AutoGen/MissionServer_autogen_RemoteFuncs.c
	../Common/AutoGen/MissionServer_autogen_RemoteFuncs.h
	../Common/AutoGen/MissionServer_autogen_SlowFuncs.c
	../Common/AutoGen/MissionServer_autogen_SlowFuncs.h
)
set_source_files_properties(${MissionServer_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(MissionServer
	${MissionServer_SOURCES} ${MissionServer_HEADERS} ${MissionServer_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${MissionServer_SOURCES} ${MissionServer_HEADERS} ${MissionServer_GENERATED})
cox_target_defaults(MissionServer)
target_include_directories(MissionServer PRIVATE src)
target_link_libraries(MissionServer PRIVATE
	Common ServerLib UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(MissionServer PRIVATE
	MISSIONSERVER PROJECT_MISSIONSERVER SECURE_STRINGS _CONSOLE _MBCS)
target_compile_options(MissionServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(MissionServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(MissionServer)
