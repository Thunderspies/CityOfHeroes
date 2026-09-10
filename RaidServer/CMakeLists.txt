include(dependencies/Lua)

set(RaidServer_SOURCES
	../Common/cmdparse/AppServerCmd.c
	../Common/cmdparse/cmdstatserver.c
	../Common/container/container_server.c
	../Common/container/container_store.c
	../Common/container/container_util.c
	../Common/container/dbcontainerpack.c
	../Common/container/dbstub.c
	../Common/dbserver/servercfg.c
	../Common/entity/RewardToken.c
	../Common/entity/Supergroup.c
	../Common/entity/teamup.c
	../Common/gameData/iopdata.c
	../Common/gameData/raidstruct.c
	../DBServer/src/container_diff.c
	../DBServer/src/container_tplt_utils.c
	../MapServer/src/container/containerSupergroup.c
	../MapServer/src/dbcomm/dbcomm.c
	../MapServer/src/dbcomm/dbcontainer.c
	../MapServer/src/dbcomm/dbnamecache.c
	../MapServer/src/dbcomm/logcomm.c
	../MapServer/src/storyarc/storytaskinfo.c
	src/iopserver.c
	src/raidinfo.c
	src/raidserver.c
)

set(RaidServer_HEADERS
	../Common/cmdparse/AppServerCmd.h
	../Common/cmdparse/cmdstatserver.h
	../Common/container/container_server.h
	../Common/container/container_store.h
	../Common/container/container_util.h
	../Common/container/dbcontainerpack.h
	../Common/dbserver/servercfg.h
	../Common/entity/RewardToken.h
	../Common/gameData/iopdata.h
	../Common/gameData/raidstruct.h
	../DBServer/src/container_diff.h
	../DBServer/src/container_tplt_utils.h
	../MapServer/src/container/containerSupergroup.h
	../MapServer/src/dbcomm/dbcomm.h
	../MapServer/src/dbcomm/dbcontainer.h
	../MapServer/src/dbcomm/dbnamecache.h
	../MapServer/src/dbcomm/logcomm.h
	../MapServer/src/storyarc/storytaskinfo.h
	src/iopserver.h
	src/raidinfo.h
	src/raidserver.h
)

add_executable(RaidServer ${RaidServer_SOURCES} ${RaidServer_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${RaidServer_SOURCES} ${RaidServer_HEADERS})
cox_target_defaults(RaidServer)
set_target_properties(RaidServer PROPERTIES
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(RaidServer PRIVATE
	src ../DBServer/src ../Game/src ../MapServer/src)
target_link_libraries(RaidServer PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB Lua::Lua)
target_compile_definitions(RaidServer PRIVATE
	RAIDSERVER AUX_SERVER_DEFINE CONTAINER_SERVER _CONSOLE _MBCS
	_CRT_SECURE_NO_DEPRECATE)
target_compile_options(RaidServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(RaidServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
