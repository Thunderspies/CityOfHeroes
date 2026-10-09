set(DbServer_SOURCES
	../Common/container/container_query.c
	../Common/account/AccountCatalog.c
	../Common/account/AccountData.c
	../Common/account/AccountTypes.cpp
	../Common/auction/Auction.c
	../Common/auction/Xact.c
	../Common/auth/auth.c
	../Common/auth/authUserData.c
	../Common/ClientLogin/clientcommLogin.c
	../Common/container/containerEventHistory.c
	../Common/container/container_util.c
	../Common/dbserver/servercfg.c
	../Common/entity/LoadDefCommon.c
	../Common/mission/MissionSearch.c
	../Common/mission/missionserver_meta.c
	../Common/sql/sqlconn.c
	../Common/sql/sqltask.cpp
	../Common/turnstile/TurnstileCommon.c
	../Common/turnstile/TurnstileServerCommon.c
	../MapServer/src/dbcomm/staticMapInfo.c
	src/accountservercomm.c
	src/arenacomm.c
	src/auctionservercomm.c
	src/authcomm.c
	src/backup.c
	src/beaconservercomm.c
	src/chatrelay.c
	src/clientcomm.c
	src/container.c
	src/container_diff.c
	src/container_flatfile.c
	src/container_merge.c
	src/container_sql.c
	src/container_tplt.c
	src/container_tplt_utils.c
	src/dbdispatch.c
	src/dbEventHistory.c
	src/dbimport.c
	src/dbinit.c
	src/dblog.c
	src/dbmission.c
	src/dbmsg.c
	src/dbperf.c
	src/dbrelay.c
	src/doors.c
	src/launchercomm.c
	src/loadBalancing.c
	src/logserver.c
	src/mapcrashreport.c
	src/mapxfer.c
	src/missionservercomm.c
	src/namecache.c
	src/offline.c
	src/overloadProtection.c
	src/queueservercomm.c
	src/serverAutoStart.c
	src/sql_fifo.c
	src/statagg.c
	src/statservercomm.c
	src/status.c
	src/svrmoncomm.c
	src/turnstileDb.c
	src/waitingEntities.c
	src/weeklyTFcfg.c
	src/zeromqSocket.c
)

set(DbServer_HEADERS
	../Common/account/AccountCatalog.h
	../Common/account/AccountData.h
	../Common/account/AccountTypes.h
	../Common/auction/Auction.h
	../Common/auction/Xact.h
	../Common/ClientLogin/clientcommLogin.h
	../Common/comm_backend.h
	../Common/container/containerEventHistory.h
	../Common/container/container_util.h
	../Common/dbserver/auctionservercomm.h
	../Common/dbserver/servercfg.h
	../Common/entity/gametypes.h
	../Common/entity/LoadDefCommon.h
	../Common/mission/MissionSearch.h
	../Common/mission/missionserver_meta.h
	../Common/sql/sqlconn.h
	../Common/sql/sqltask.h
	../Common/turnstile/TurnstileCommon.h
	../Common/turnstile/TurnstileServerCommon.h
	../Launcher/src/performance.h
	../MapServer/src/dbcomm/staticMapInfo.h
	../MapServer/src/entity/SgrpStats.h
	src/accountservercomm.h
	src/arenacomm.h
	src/authcomm.h
	src/backup.h
	src/beaconservercomm.h
	src/chatrelay.h
	src/clientcomm.h
	src/container.h
	src/container_diff.h
	src/container_flatfile.h
	src/container_merge.h
	src/container_sql.h
	src/container_tplt.h
	src/container_tplt_utils.h
	src/dbdispatch.h
	src/dbEventHistory.h
	src/dbimport.h
	src/dbinit.h
	src/dblog.h
	src/dbmission.h
	src/dbmsg.h
	src/dbperf.h
	src/dbrelay.h
	src/doors.h
	src/launchercomm.h
	src/loadBalancing.h
	src/logserver.h
	src/mapcrashreport.h
	src/mapxfer.h
	src/missionservercomm.h
	src/namecache.h
	src/offline.h
	src/overloadProtection.h
	src/queueservercomm.h
	src/serverAutoStart.h
	src/sql_fifo.h
	src/statagg.h
	src/statservercomm.h
	src/status.h
	src/svrmoncomm.h
	src/turnstileDb.h
	src/waitingEntities.h
	src/weeklyTFcfg.h
	src/zeromqSocket.h
)

# The shared mission sources include AST files owned by this consumer.
# They are generated includes, not separate compilation units.
set(DbServer_GENERATED
	src/AutoGen/MissionSearch_h_ast.c
	src/AutoGen/MissionSearch_h_ast.h
)

# StructParser writes these target-specific sidecars even without annotations.
list(APPEND DbServer_GENERATED
	src/AutoGen/DbServer_AutoTest.c
	src/AutoGen/DbServer_autogen_QueuedFuncs.c
	src/AutoGen/DbServer_autogen_QueuedFuncs.h
	src/AutoGen/DbServer_autorun_autogen.cpp
	src/AutoGen/DbServer_autorun_extrafunc_autogen.c
	src/AutoGen/DbServer_autotransactions_autogen.c
	src/AutoGen/DbServer_commands_autogen.c
	src/AutoGen/DbServer_commands_autogen_CommandFuncs.c
	src/AutoGen/DbServer_commands_autogen_CommandFuncs.h
	src/AutoGen/DbServer_latelink_autogen.c
	../Common/AutoGen/DbServer_autogen_RemoteFuncs.c
	../Common/AutoGen/DbServer_autogen_RemoteFuncs.h
	../Common/AutoGen/DbServer_autogen_SlowFuncs.c
	../Common/AutoGen/DbServer_autogen_SlowFuncs.h
)
set_source_files_properties(${DbServer_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

include(dependencies/SQLite)
add_library(DbServerStorage STATIC src/storage/database.c src/storage/database.h
	src/storage/database_odbc.c src/storage/database_dialect.c src/storage/database_sqlite.c
	src/storage/database_sqlite_schema.c)
cox_target_defaults(DbServerStorage)
target_include_directories(DbServerStorage PUBLIC src)
target_link_libraries(DbServerStorage PUBLIC Common UtilitiesLib odbc32 SQLite::SQLite3)
target_compile_definitions(DbServerStorage PRIVATE DBSERVER)

add_executable(DbServer
	${DbServer_SOURCES} ${DbServer_HEADERS} ${DbServer_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${DbServer_SOURCES} ${DbServer_HEADERS} ${DbServer_GENERATED})
cox_target_defaults(DbServer)
target_include_directories(DbServer PRIVATE
	src ../MapServer/src ../Launcher/src)
target_link_libraries(DbServer PRIVATE
	Common UtilitiesLib DbServerStorage cryptopp::cryptopp ZLIB::ZLIB odbc32 shlwapi)
target_compile_definitions(DbServer PRIVATE
	DBSERVER _CONSOLE _MBCS _CRT_SECURE_NO_DEPRECATE)
target_compile_options(DbServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(DbServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(StructParser)
StructParse(DbServer)
cox_stage_executable_alias(DbServer LogServer)

if(COX_BUILD_UTILITIES_TESTS)
	set(origin_source "${CMAKE_CURRENT_SOURCE_DIR}/src/clientcomm.c")
	set(choice_source "${PROJECT_SOURCE_DIR}/MapServer/src/entity/alignment_shift.c")
	set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
		"${origin_source}" "${choice_source}")
	file(READ "${origin_source}" origin_code)
	string(FIND "${origin_code}" "unsigned int epicFlags = 0;" origin_start)
	string(FIND "${origin_code}"
		"idx += sprintf(cmd_buf + idx,\"Ents2[0].PlayerSubType" origin_end)
	if(origin_start LESS 0 OR origin_end LESS origin_start)
		message(FATAL_ERROR "Cannot locate initial epic flag writer")
	endif()
	math(EXPR origin_length "${origin_end} - ${origin_start}")
	string(SUBSTRING "${origin_code}" ${origin_start} ${origin_length} origin_code)
	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/epic_origin.inc" "${origin_code}")
	file(READ "${choice_source}" choice_code)
	foreach(faction hero villain)
		string(FIND "${choice_code}"
			"e->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;" choice_start)
		if(choice_start LESS 0)
			message(FATAL_ERROR "Cannot locate neutral tutorial faction choice")
		endif()
		string(SUBSTRING "${choice_code}" ${choice_start} -1 choice_code)
		string(FIND "${choice_code}" "e->db_flags |= DBFLAG_UNLOCK_" flag_start)
		if(flag_start LESS 0)
			message(FATAL_ERROR "Cannot locate tutorial choice boundary")
		endif()
		string(SUBSTRING "${choice_code}" ${flag_start} -1 flag_code)
		string(FIND "${flag_code}" ";" flag_end)
		math(EXPR choice_end "${flag_start} + ${flag_end} + 1")
		string(SUBSTRING "${choice_code}" 0 ${choice_end} faction_code)
		file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/epic_${faction}.inc"
			"${faction_code}")
		string(SUBSTRING "${choice_code}" ${choice_end} -1 choice_code)
	endforeach()
	add_executable(EpicOriginTests tests/epic_origin.c)
	cox_target_defaults(EpicOriginTests)
	target_include_directories(EpicOriginTests PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}")
	target_link_libraries(EpicOriginTests PRIVATE Common UtilitiesLib)
	add_test(NAME EpicOrigin COMMAND EpicOriginTests)
endif()


if(COX_BUILD_DBSERVER_TESTS)
	target_sources(DbServer PRIVATE src/storage/container_sqlite_test.c)
	target_compile_definitions(DbServer PRIVATE COX_BUILD_DBSERVER_TESTS)
	add_test(NAME ContainerSqliteTests COMMAND DbServer --storage-tests)
	add_executable(ContainerQueryTests src/storage/container_query_test.c ../Common/container/container_query.c)
	cox_target_defaults(ContainerQueryTests)
	target_link_libraries(ContainerQueryTests PRIVATE Common UtilitiesLib)
	add_test(NAME ContainerQueryTests COMMAND ContainerQueryTests)
	add_executable(DatabaseDialectTests src/storage/database_dialect_test.c)
	cox_target_defaults(DatabaseDialectTests)
	target_link_libraries(DatabaseDialectTests PRIVATE DbServerStorage)
	add_test(NAME DatabaseDialectTests COMMAND DatabaseDialectTests)
	add_executable(DatabaseSqliteTests src/storage/database_sqlite_test.c)
	cox_target_defaults(DatabaseSqliteTests)
	target_link_libraries(DatabaseSqliteTests PRIVATE DbServerStorage)
	add_test(NAME DatabaseSqliteTests COMMAND DatabaseSqliteTests)
	set(COX_SQLITE_GAME_DATA_ROOT "" CACHE PATH "Optional complete game data root for the SQLite restart smoke test")
	if(COX_SQLITE_GAME_DATA_ROOT)
		find_package(Python3 REQUIRED COMPONENTS Interpreter)
		add_test(NAME SqliteLocalRestartSmoke COMMAND ${Python3_EXECUTABLE}
			${CMAKE_CURRENT_SOURCE_DIR}/tests/sqlite_local_smoke.py
			--bin $<TARGET_FILE_DIR:DbServer> --data-root ${COX_SQLITE_GAME_DATA_ROOT}
			--output ${CMAKE_BINARY_DIR}/sqlite-runtime)
		set_tests_properties(SqliteLocalRestartSmoke PROPERTIES RUN_SERIAL TRUE TIMEOUT 3600 LABELS integration)
	endif()
endif()
