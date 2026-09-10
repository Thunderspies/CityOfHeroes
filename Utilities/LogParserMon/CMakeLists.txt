set(LogParserMon_SOURCES
	src/LogParserMon.c
)

set(LogParserMon_HEADERS
	src/resource.h
)

set(LogParserMon_RESOURCES
	src/LogParserMon.rc
	src/icon1.ico
)

add_executable(LogParserMon WIN32
	${LogParserMon_SOURCES} ${LogParserMon_HEADERS}
	${LogParserMon_RESOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${LogParserMon_SOURCES} ${LogParserMon_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${LogParserMon_RESOURCES})
cox_target_defaults(LogParserMon)

target_include_directories(LogParserMon PRIVATE src)
target_link_libraries(LogParserMon PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
# Command buffers and list-view payloads use narrow strings.
target_compile_definitions(LogParserMon PRIVATE _WINDOWS _MBCS)

target_compile_options(LogParserMon PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(LogParserMon PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
