set(NetTest_SOURCES
	src/NetTest.c
)

set(NetTest_HEADERS
	src/NetTest.h
)

add_executable(NetTest ${NetTest_SOURCES} ${NetTest_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${NetTest_SOURCES} ${NetTest_HEADERS})
cox_target_defaults(NetTest)

target_include_directories(NetTest PRIVATE src)

target_link_libraries(NetTest PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)

target_compile_definitions(NetTest PRIVATE _CONSOLE _MBCS)

target_compile_options(NetTest PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(NetTest PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
