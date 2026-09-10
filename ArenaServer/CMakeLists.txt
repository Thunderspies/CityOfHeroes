set(ArenaServer_SOURCES
	../Common/container/container_store.c
	../Common/container/container_util.c
	../Common/container/dbcontainerpack.c
	../Common/dbserver/servercfg.c
	../Common/gameData/arenaschedule.c
	../Common/gameData/arenastruct.c
	../DBServer/src/container_diff.c
	../DBServer/src/container_tplt_utils.c
	../MapServer/src/container/containerArena.c
	../MapServer/src/dbcomm/dbcomm.c
	../MapServer/src/dbcomm/dbcontainer.c
	src/arenadb.c
	src/arenaevent.c
	src/arenaplayer.c
	src/arenaranking.c
	src/arenaserver.c
	src/arenaupdates.c
)

set(ArenaServer_HEADERS
	../Common/container/container_store.h
	../Common/container/container_util.h
	../Common/container/dbcontainerpack.h
	../Common/dbserver/servercfg.h
	../Common/gameData/arenaref.h
	../Common/gameData/arenaschedule.h
	../Common/gameData/arenastruct.h
	../DBServer/src/container_diff.h
	../DBServer/src/container_tplt_utils.h
	../MapServer/src/container/containerArena.h
	../MapServer/src/dbcomm/dbcomm.h
	../MapServer/src/dbcomm/dbcontainer.h
	src/arenaevent.h
	src/arenaplayer.h
	src/arenaranking.h
	src/arenaserver.h
	src/arenaupdates.h
)

add_executable(ArenaServer ${ArenaServer_SOURCES} ${ArenaServer_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${ArenaServer_SOURCES} ${ArenaServer_HEADERS})
cox_target_defaults(ArenaServer)
target_include_directories(ArenaServer PRIVATE
	src ../DBServer/src ../Game/src ../MapServer/src)
target_link_libraries(ArenaServer PRIVATE Common UtilitiesLib)
target_compile_definitions(ArenaServer PRIVATE AUX_SERVER_DEFINE _CONSOLE)
target_link_options(ArenaServer PRIVATE /LARGEADDRESSAWARE)
