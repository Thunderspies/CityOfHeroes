include(dependencies/SQLite)

# DIA ships with Visual Studio; this cache entry also supports an SDK installed
# separately from the selected generator instance.
find_path(memtrack_DIA_INCLUDE_DIR dia2.h
	HINTS
		"${CMAKE_GENERATOR_INSTANCE}/DIA SDK/include"
		"$ENV{VSINSTALLDIR}/DIA SDK/include"
		"${MSVC_DIA_SDK_DIR}/include"
	DOC "Path to the DIA SDK headers used by memtrack"
	REQUIRED)

set(memtrack_SOURCES
	src/memtrack.cpp
	src/SymbolHelper.cpp
)

set(memtrack_HEADERS
	src/memtrack_types.h
	src/stdafx.h
	src/SymbolHelper.h
)

add_executable(memtrack ${memtrack_SOURCES} ${memtrack_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${memtrack_SOURCES} ${memtrack_HEADERS})
cox_target_defaults(memtrack)

target_include_directories(memtrack PRIVATE src "${memtrack_DIA_INCLUDE_DIR}")
target_link_libraries(memtrack PRIVATE SQLite::SQLite3 dbghelp ole32 oleaut32)
# The fork uses narrow command-line arguments and Windows API strings.
target_compile_definitions(memtrack PRIVATE _CONSOLE _MBCS)

target_compile_options(memtrack PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(memtrack PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
