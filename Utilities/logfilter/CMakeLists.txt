set(logfilter_SOURCES
	src/logfilter.c
)

add_executable(logfilter ${logfilter_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${logfilter_SOURCES})
cox_target_defaults(logfilter)

target_link_libraries(logfilter PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(logfilter PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(logfilter PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(logfilter PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
