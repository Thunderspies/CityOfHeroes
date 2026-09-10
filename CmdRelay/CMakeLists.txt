set(CmdRelay_SOURCES
	src/kill_process.c
	src/relay_actions.c
	src/relay_main.c
	src/relay_util.c
)

set(CmdRelay_HEADERS
	src/kill_process.h
	src/relay_actions.h
	src/relay_types.h
	src/relay_util.h
)

add_executable(CmdRelay ${CmdRelay_SOURCES} ${CmdRelay_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${CmdRelay_SOURCES} ${CmdRelay_HEADERS})
cox_target_defaults(CmdRelay)
target_include_directories(CmdRelay PRIVATE src)
target_link_libraries(CmdRelay PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(CmdRelay PRIVATE _CONSOLE _MBCS)
target_compile_options(CmdRelay PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(CmdRelay PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
