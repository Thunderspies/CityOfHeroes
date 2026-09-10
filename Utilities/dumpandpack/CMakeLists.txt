include(dependencies/Minizip)

set(dumpandpack_SOURCES
	src/dumpandpack.cpp
)

add_executable(dumpandpack ${dumpandpack_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${dumpandpack_SOURCES})
cox_target_defaults(dumpandpack)

target_include_directories(dumpandpack PRIVATE src)
target_link_libraries(dumpandpack PRIVATE Minizip::Minizip Shlwapi)
target_compile_definitions(dumpandpack PRIVATE _CONSOLE)

target_compile_options(dumpandpack PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(dumpandpack PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
