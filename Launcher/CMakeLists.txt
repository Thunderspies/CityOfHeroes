set(Launcher_SOURCES
	../Common/dbserver/servercfg.c
	src/launcher.c
	src/monitor.c
	src/performance.c
	src/proclist.c
)

set(Launcher_HEADERS
	../Common/dbserver/servercfg.h
	src/launcher_enum.h
	src/monitor.h
	src/performance.h
	src/proclist.h
)

add_executable(Launcher ${Launcher_SOURCES} ${Launcher_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${Launcher_SOURCES} ${Launcher_HEADERS})
cox_target_defaults(Launcher)
target_include_directories(Launcher PRIVATE src ../DBServer/src)
target_link_libraries(Launcher PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(Launcher PRIVATE SECURE_STRINGS _CONSOLE _MBCS)
target_compile_options(Launcher PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(Launcher PRIVATE /LARGEADDRESSAWARE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
