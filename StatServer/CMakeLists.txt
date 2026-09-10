include(dependencies/Lua)

set(StatServer_SOURCES
	../Common/bases/BaseEntry.c
	../Common/bases/baseupkeep.c
	../Common/cmdparse/AppServerCmd.c
	../Common/cmdparse/cmdstatserver.c
	../Common/container/container_store.c
	../Common/container/container_util.c
	../Common/container/dbcontainerpack.c
	../Common/dbserver/servercfg.c
	../Common/entity/attrib_names.c
	../Common/entity/LoadDefCommon.c
	../Common/entity/RewardToken.c
	../Common/entity/SgrpBasePermissions.c
	../Common/entity/Supergroup.c
	../Common/entity/supergroup_eval.c
	../Common/entity/teamup.c
	../Common/gameData/badgestats_db.c
	../Common/gameData/miningaccumulator.c
	../Common/gameData/sgrpstatsstruct.c
	../Common/gameData/statgroupstruct.c
	../Common/player/badges.c
	../DBServer/src/container_diff.c
	../DBServer/src/container_tplt_utils.c
	../MapServer/src/container/containerSupergroup.c
	../MapServer/src/dbcomm/dbcomm.c
	../MapServer/src/dbcomm/dbcontainer.c
	../MapServer/src/dbcomm/logcomm.c
	../MapServer/src/entity/SgrpBadges.c
	../MapServer/src/entity/SgrpStats.c
	../MapServer/src/player/badges_load.c
	../MapServer/src/storyarc/storytaskinfo.c
	src/Entity/stat_SgrpBadges.c
	src/statcmd.c
	src/statdb.c
	src/statgroups.c
	src/statserver.c
)

set(StatServer_HEADERS
	../Common/bases/BaseEntry.h
	../Common/bases/baseupkeep.h
	../Common/cmdparse/AppServerCmd.h
	../Common/cmdparse/cmdstatserver.h
	../Common/container/container_store.h
	../Common/container/container_util.h
	../Common/container/dbcontainerpack.h
	../Common/dbserver/servercfg.h
	../Common/entity/attrib_names.h
	../Common/entity/LoadDefCommon.h
	../Common/entity/load_def.h
	../Common/entity/RewardToken.h
	../Common/entity/SgrpBasePermissions.h
	../Common/entity/Supergroup.h
	../Common/entity/supergroup_eval.h
	../Common/entity/teamup.h
	../Common/gameData/badgestats_db.h
	../Common/gameData/miningaccumulator.h
	../Common/gameData/sgrpstatsstruct.h
	../Common/gameData/statgroupstruct.h
	../Common/player/badges.h
	../DBServer/src/container_diff.h
	../DBServer/src/container_tplt_utils.h
	../MapServer/src/container/containerSupergroup.h
	../MapServer/src/dbcomm/dbcomm.h
	../MapServer/src/dbcomm/dbcontainer.h
	../MapServer/src/dbcomm/logcomm.h
	../MapServer/src/entity/SgrpBadges.h
	../MapServer/src/entity/SgrpStats.h
	../MapServer/src/player/BadgeStats.h
	../MapServer/src/storyarc/storytaskinfo.h
	src/Entity/stat_SgrpBadges.h
	src/statcmd.h
	src/statdb.h
	src/statgroups.h
	src/statserver.h
)

add_executable(StatServer ${StatServer_SOURCES} ${StatServer_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${StatServer_SOURCES} ${StatServer_HEADERS})
cox_target_defaults(StatServer)
target_include_directories(StatServer PRIVATE
	src ../DBServer/src ../Game/src ../MapServer/src)
target_link_libraries(StatServer PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB Lua::Lua)
target_compile_definitions(StatServer PRIVATE
	STATSERVER AUX_SERVER_DEFINE CONTAINER_SERVER _CONSOLE UNICODE _UNICODE
	_CRT_SECURE_NO_DEPRECATE)
target_compile_options(StatServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(StatServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
