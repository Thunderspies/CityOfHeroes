set(TurnstileServer_SOURCES
	../Common/turnstile/TurnstileCommon.c
	../Common/turnstile/TurnstileServerCommon.c
	src/TurnstileServer.c
	src/TurnstileServerEvent.c
	src/TurnstileServerGroup.c
	src/TurnstileServerMsg.c
)

set(TurnstileServer_HEADERS
	../Common/turnstile/TurnstileCommon.h
	../Common/turnstile/TurnstileServerCommon.h
	src/TurnstileServer.h
	src/TurnstileServerEvent.h
	src/TurnstileServerGroup.h
	src/TurnstileServerMsg.h
)

# StructParser emits these target-specific files even without AST annotations.
# They retain one build owner and are not separate translation units.
set(TurnstileServer_GENERATED
	src/AutoGen/TurnstileServer_AutoTest.c
	src/AutoGen/TurnstileServer_autogen_QueuedFuncs.c
	src/AutoGen/TurnstileServer_autogen_QueuedFuncs.h
	src/AutoGen/TurnstileServer_autorun_autogen.cpp
	src/AutoGen/TurnstileServer_autorun_extrafunc_autogen.c
	src/AutoGen/TurnstileServer_autotransactions_autogen.c
	src/AutoGen/TurnstileServer_commands_autogen.c
	src/AutoGen/TurnstileServer_commands_autogen_CommandFuncs.c
	src/AutoGen/TurnstileServer_commands_autogen_CommandFuncs.h
	src/AutoGen/TurnstileServer_latelink_autogen.c
	../Common/AutoGen/TurnstileServer_autogen_RemoteFuncs.c
	../Common/AutoGen/TurnstileServer_autogen_RemoteFuncs.h
	../Common/AutoGen/TurnstileServer_autogen_SlowFuncs.c
	../Common/AutoGen/TurnstileServer_autogen_SlowFuncs.h
)
set_source_files_properties(${TurnstileServer_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(TurnstileServer
	${TurnstileServer_SOURCES} ${TurnstileServer_HEADERS}
	${TurnstileServer_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${TurnstileServer_SOURCES} ${TurnstileServer_HEADERS}
	${TurnstileServer_GENERATED})
cox_target_defaults(TurnstileServer)
target_include_directories(TurnstileServer PRIVATE src)
target_link_libraries(TurnstileServer PRIVATE
	Common ServerLib UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(TurnstileServer PRIVATE
	TURNSTILESERVER PROJECT_TURNSTILESERVER SECURE_STRINGS _CONSOLE _MBCS)
target_compile_options(TurnstileServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(TurnstileServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(TurnstileServer)
