set(ServerMonitor_SOURCES
	../CmdRelay/src/relay_util.c
	src/chatMonitor.c
	src/processMonitor.c
	src/prompt.c
	src/relaycomm.c
	src/relay_utils.c
	src/serverMonitor.c
	src/serverMonitorCmdRelay.c
	src/serverMonitorCommon.c
	src/serverMonitorCrashMsg.c
	src/serverMonitorEnts.c
	src/serverMonitorListener.c
	src/serverMonitorNet.c
	src/serverMonitorOverload.c
	src/shardMonitor.c
	src/shardMonitorCmdRelay.c
	src/shardMonitorComm.c
	src/shardMonitorConfigure.c
	src/shardMonitorConfigureNewShard.c
	src/updateServerList.c
)

set(ServerMonitor_HEADERS
	src/chatMonitor.h
	src/processMonitor.h
	src/prompt.h
	src/relaycomm.h
	src/relay_utils.h
	src/resource.h
	src/serverList.h
	src/serverMonitor.h
	src/serverMonitorCmdRelay.h
	src/serverMonitorCommon.h
	src/serverMonitorCrashMsg.h
	src/serverMonitorEnts.h
	src/serverMonitorListener.h
	src/serverMonitorNet.h
	src/serverMonitorOverload.h
	src/shardMonitor.h
	src/shardMonitorCmdRelay.h
	src/shardMonitorComm.h
	src/shardMonitorConfigure.h
	src/shardMonitorConfigureNewShard.h
	src/updateServerList.h
)

set(ServerMonitor_RESOURCES
	src/ServerMonitor.rc
	src/CrypticPrc1.ico
	src/ShardMonitor.ico
)

add_executable(ServerMonitor WIN32
	${ServerMonitor_SOURCES} ${ServerMonitor_HEADERS}
	${ServerMonitor_RESOURCES})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${ServerMonitor_SOURCES} ${ServerMonitor_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${ServerMonitor_RESOURCES})
cox_target_defaults(ServerMonitor)
target_include_directories(ServerMonitor PRIVATE
	src
	../Common/entity
	../DBServer/src
	../MapServer/src
	../MapServer/src/container
	../MapServer/src/dbcomm
	../Launcher/src
	../CmdRelay/src)
target_link_libraries(ServerMonitor PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB psapi)
target_compile_definitions(ServerMonitor PRIVATE _WINDOWS _MBCS)
target_compile_options(ServerMonitor PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(ServerMonitor PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
