set(AccountServer_SOURCES
	../Common/account/AccountCatalog.c
	../Common/account/AccountData.c
	../Common/account/AccountTypes.cpp
	../Common/cmdparse/AppServerCmd.c
	../Common/cmdparse/cmdaccountserver.c
	../Common/entity/LoadDefCommon.c
	../Common/persist/textcrcdb.c
	../Common/persist/textcrcjournal.c
	../Common/Playspan/JSONParser.cpp
	../Common/sql/sqlconn.c
	../Common/sql/sqltask.cpp
	src/accountcmds.cpp
	src/AccountDb.cpp
	src/AccountServer.cpp
	src/AccountSql.cpp
	src/account_inventory.cpp
	src/account_loyaltyrewards.cpp
	src/request.cpp
	src/request_certification.cpp
	src/request_multi.cpp
	src/request_rename.cpp
	src/request_respec.cpp
	src/request_shardxfer.cpp
	src/request_slot.cpp
	src/transaction.cpp
)

set(AccountServer_HEADERS
	../Common/account/AccountCatalog.h
	../Common/account/AccountData.h
	../Common/account/AccountTypes.h
	../Common/cmdparse/AppServerCmd.h
	../Common/cmdparse/cmdaccountserver.h
	../Common/cmdparse/cmdenum.h
	../Common/entity/gametypes.h
	../Common/persist/textcrcdb.h
	../Common/persist/textcrcjournal.h
	../Common/Playspan/JSONParser.h
	../Common/sql/sqlconn.h
	../Common/sql/sqltask.h
	src/AccountCmds.h
	src/AccountDb.hpp
	src/AccountServer.hpp
	src/AccountSql.h
	src/account_inventory.h
	src/account_loyaltyrewards.h
	src/request.hpp
	src/request_certification.hpp
	src/request_multi.hpp
	src/request_rename.hpp
	src/request_respec.hpp
	src/request_shardxfer.hpp
	src/request_slot.hpp
	src/transaction.h
)

include(dependencies/Yajl)

add_executable(AccountServer
	${AccountServer_SOURCES} ${AccountServer_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${AccountServer_SOURCES} ${AccountServer_HEADERS})
cox_target_defaults(AccountServer)
target_include_directories(AccountServer PRIVATE src)
target_link_libraries(AccountServer PRIVATE
	Common UtilitiesLib YAJL::YAJL cryptopp::cryptopp ZLIB::ZLIB odbc32)
target_compile_definitions(AccountServer PRIVATE
	ACCOUNTSERVER SECURE_STRINGS CRYPTOPP_ENABLE_NAMESPACE_WEAK _CONSOLE)
target_compile_options(AccountServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(AccountServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
