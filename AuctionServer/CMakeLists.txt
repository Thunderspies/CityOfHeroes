set(AuctionServer_SOURCES
	../Common/auction/Auction.c
	../Common/auction/Xact.c
	../Common/cmdparse/AppServerCmd.c
	../Common/cmdparse/cmdauction.c
	../Common/persist/persist_sql.c
	../Common/persist/textcrcdb.c
	../Common/persist/textcrcjournal.c
	../Common/sql/sqlconn.c
	../Common/sql/sqltask.cpp
	src/AsyncFileWriter.c
	src/auctioncmds.c
	src/AuctionDb.c
	src/auctionfulfill.c
	src/auctionhistory.c
	src/AuctionServer.c
	src/AuctionSql.cpp
	src/BinHeap.c
	src/XactServer.c
)

set(AuctionServer_HEADERS
	../Common/auction/Auction.h
	../Common/auction/Xact.h
	../Common/cmdparse/AppServerCmd.h
	../Common/cmdparse/cmdauction.h
	../Common/cmdparse/cmdenum.h
	../Common/entity/gametypes.h
	../Common/persist/persist_sql.h
	../Common/persist/textcrcdb.h
	../Common/persist/textcrcjournal.h
	../Common/sql/sqlconn.h
	../Common/sql/sqltask.h
	src/AsyncFileWriter.h
	src/auctioncmds.h
	src/AuctionDb.h
	src/auctionfulfill.h
	src/auctionhistory.h
	src/AuctionServer.h
	src/AuctionSql.h
	src/BinHeap.h
	src/XactServer.h
	src/XactServerInternal.h
)

add_executable(AuctionServer
	${AuctionServer_SOURCES} ${AuctionServer_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${AuctionServer_SOURCES} ${AuctionServer_HEADERS})
cox_target_defaults(AuctionServer)
target_include_directories(AuctionServer PRIVATE src)
target_link_libraries(AuctionServer PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB odbc32)
target_compile_definitions(AuctionServer PRIVATE
	AUCTIONSERVER SECURE_STRINGS _CONSOLE)
target_compile_options(AuctionServer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(AuctionServer PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
