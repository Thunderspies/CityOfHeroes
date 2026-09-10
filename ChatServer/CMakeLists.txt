set(ChatServer_SOURCES
	../Common/filter/profanity.c
	../Common/filter/validate_name.c
	../Common/persist/persist.c
	../Common/persist/persist_diffbig.c
	../Common/persist/persist_diffsmall.c
	../Common/persist/persist_flat.c
	../Common/persist/persist_sql.c
	../Common/persist/textcrcdb.c
	../Common/persist/textcrcjournal.c
	../Common/sql/sqlconn.c
	../Common/sql/sqltask.cpp
	../Launcher/src/performance.c
	../MapServer/src/filter/reserved_names.c
	src/admin.c
	src/channels.c
	src/ChatSql.cpp
	src/chatsqldb.c
	src/csr.c
	src/friends.c
	src/ignore.c
	src/loadsave.c
	src/messagefile.c
	src/monitor.c
	src/msgsend.c
	src/shardnet.c
	src/users.c
)

set(ChatServer_HEADERS
	../ChatAdmin/src/ChatComm.h
	../Common/chatdb.h
	../Common/persist/persist.h
	../Common/persist/persist_diffbig.h
	../Common/persist/persist_diffsmall.h
	../Common/persist/persist_flat.h
	../Common/persist/persist_internal.h
	../Common/persist/persist_sql.h
	../Common/persist/textcrcdb.h
	../Common/persist/textcrcjournal.h
	../Common/sql/sqlconn.h
	../Common/sql/sqltask.h
	src/admin.h
	src/channels.h
	src/ChatSql.h
	src/chatsqldb.h
	src/csr.h
	src/friends.h
	src/ignore.h
	src/loadsave.h
	src/messagefile.h
	src/monitor.h
	src/msgsend.h
	src/shardnet.h
	src/users.h
)

# ChatServer owns these AST files for its copy of the persistence sources.
# The persistence sources include them; they are not separate translation units.
set(ChatServer_GENERATED
	src/AutoGen/persist_h_ast.c
	src/AutoGen/persist_h_ast.h
	src/AutoGen/persist_internal_h_ast.c
	src/AutoGen/persist_internal_h_ast.h
)

# StructParser also emits these target-specific files unconditionally. They
# remain uncompiled, as in the original project, but have a single build owner.
list(APPEND ChatServer_GENERATED
	src/AutoGen/ChatServer_AutoTest.c
	src/AutoGen/ChatServer_autogen_QueuedFuncs.c
	src/AutoGen/ChatServer_autogen_QueuedFuncs.h
	src/AutoGen/ChatServer_autorun_autogen.cpp
	src/AutoGen/ChatServer_autorun_extrafunc_autogen.c
	src/AutoGen/ChatServer_autotransactions_autogen.c
	src/AutoGen/ChatServer_commands_autogen.c
	src/AutoGen/ChatServer_commands_autogen_CommandFuncs.c
	src/AutoGen/ChatServer_commands_autogen_CommandFuncs.h
	src/AutoGen/ChatServer_latelink_autogen.c
	../Common/AutoGen/ChatServer_autogen_RemoteFuncs.c
	../Common/AutoGen/ChatServer_autogen_RemoteFuncs.h
	../Common/AutoGen/ChatServer_autogen_SlowFuncs.c
	../Common/AutoGen/ChatServer_autogen_SlowFuncs.h
)
set_source_files_properties(${ChatServer_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_executable(ChatServer
	${ChatServer_SOURCES} ${ChatServer_HEADERS} ${ChatServer_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${ChatServer_SOURCES} ${ChatServer_HEADERS} ${ChatServer_GENERATED})
cox_target_defaults(ChatServer)
target_include_directories(ChatServer PRIVATE
	src ../Launcher/src ../MapServer/src ../ChatAdmin/src ../ServerMonitor/src)
target_link_libraries(ChatServer PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB odbc32)
target_compile_definitions(ChatServer PRIVATE
	CHATSERVER SECURE_STRINGS _CONSOLE _MBCS)
target_compile_options(ChatServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(ChatServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(ChatServer)
