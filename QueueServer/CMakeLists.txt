set(QueueServer_SOURCES
	../Common/account/AccountData.c
	../Common/ClientLogin/clientcommLogin.c
	src/BinHeap.c
	src/QueueServer.c
	src/QueueServer_Passthrough.c
)

set(QueueServer_HEADERS
	../Common/account/AccountData.h
	../Common/ClientLogin/clientcommLogin.h
	src/BinHeap.h
	src/QueueServer.h
	src/QueueServer_Passthrough.h
)

# StructParser emits these target-specific files even without AST annotations.
# They retain one build owner and are not separate translation units.
set(QueueServer_GENERATED
	src/AutoGen/QueueServer_AutoTest.c
	src/AutoGen/QueueServer_autogen_QueuedFuncs.c
	src/AutoGen/QueueServer_autogen_QueuedFuncs.h
	src/AutoGen/QueueServer_autorun_autogen.cpp
	src/AutoGen/QueueServer_autorun_extrafunc_autogen.c
	src/AutoGen/QueueServer_autotransactions_autogen.c
	src/AutoGen/QueueServer_commands_autogen.c
	src/AutoGen/QueueServer_commands_autogen_CommandFuncs.c
	src/AutoGen/QueueServer_commands_autogen_CommandFuncs.h
	src/AutoGen/QueueServer_latelink_autogen.c
	../Common/AutoGen/QueueServer_autogen_RemoteFuncs.c
	../Common/AutoGen/QueueServer_autogen_RemoteFuncs.h
	../Common/AutoGen/QueueServer_autogen_SlowFuncs.c
	../Common/AutoGen/QueueServer_autogen_SlowFuncs.h
)
set_source_files_properties(${QueueServer_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(QueueServer
	${QueueServer_SOURCES} ${QueueServer_HEADERS} ${QueueServer_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${QueueServer_SOURCES} ${QueueServer_HEADERS} ${QueueServer_GENERATED})
cox_target_defaults(QueueServer)
set_target_properties(QueueServer PROPERTIES
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(QueueServer PRIVATE src)
target_link_libraries(QueueServer PRIVATE Common ServerLib UtilitiesLib)
target_compile_definitions(QueueServer PRIVATE
	QUEUESERVER PROJECT_QUEUESERVER SECURE_STRINGS _CONSOLE _MBCS)
target_compile_options(QueueServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(QueueServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(QueueServer)
