include(dependencies/Minizip)

set(dumpstk_SOURCES
	src/dumpstk.cpp
	src/out.cpp
)

set(dumpstk_HEADERS
	src/out.hpp
)

add_executable(dumpstk ${dumpstk_SOURCES} ${dumpstk_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${dumpstk_SOURCES} ${dumpstk_HEADERS})
cox_target_defaults(dumpstk)

target_include_directories(dumpstk PRIVATE src)
target_link_libraries(dumpstk PRIVATE Minizip::Minizip Dbgeng Dbghelp)
target_compile_definitions(dumpstk PRIVATE _CONSOLE _MBCS)

target_compile_options(dumpstk PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(dumpstk PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
