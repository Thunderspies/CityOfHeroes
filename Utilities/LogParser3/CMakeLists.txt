set(LogParser_SOURCES
	src/LogConfig.cpp
	src/LogParser3.cpp
	src/LogPattern.cpp
	src/LogSearch.cpp
	src/LogTime.cpp
)

set(LogParser_HEADERS
	src/LogConfig.h
	src/logParser3.h
	src/LogPattern.h
	src/LogSearch.h
	src/LogTime.h
)

add_executable(LogParser ${LogParser_SOURCES} ${LogParser_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${LogParser_SOURCES} ${LogParser_HEADERS})
cox_target_defaults(LogParser)
set_target_properties(LogParser PROPERTIES OUTPUT_NAME LogParser3)

target_include_directories(LogParser PRIVATE src)
target_link_libraries(LogParser PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(LogParser PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(LogParser PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(LogParser PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
