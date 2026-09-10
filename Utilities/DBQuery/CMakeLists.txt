set(DbQuery_SOURCES
	../../Common/container/container_util.c
	../../Common/container/dbcontainerpack.c
	../../MapServer/src/container/baseloadsave.c
	../../MapServer/src/dbcomm/characterRestore.c
	../../MapServer/src/dbcomm/dbcomm.c
	../../MapServer/src/dbcomm/dbcontainer.c
	../../MapServer/src/dbcomm/dbquery.c
	../../MapServer/src/svr/dbquery_init.c
)

set(DbQuery_HEADERS
	../../Common/container/container_util.h
	../../Common/container/dbcontainerpack.h
	../../MapServer/src/dbcomm/characterRestore.h
	../../MapServer/src/dbcomm/dbcomm.h
	../../MapServer/src/dbcomm/dbcontainer.h
	../../MapServer/src/dbcomm/dbquery.h
)

add_executable(DbQuery ${DbQuery_SOURCES} ${DbQuery_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${DbQuery_SOURCES} ${DbQuery_HEADERS})
cox_target_defaults(DbQuery)
target_include_directories(DbQuery PRIVATE
	../../MapServer/src ../../DBServer/src ../../Game/src)
target_link_libraries(DbQuery PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(DbQuery PRIVATE
	DBQUERY SERVER AUX_SERVER_DEFINE _CONSOLE _CRT_SECURE_NO_DEPRECATE
	UNICODE _UNICODE)
target_compile_options(DbQuery PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(DbQuery PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
