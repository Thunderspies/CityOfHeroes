set(assertdlg_SOURCES
	src/assertdlg.cpp
	src/stdafx.cpp
)

set(assertdlg_HEADERS
	src/assertdlg.h
	src/resource.h
	src/resource1.h
	src/stdafx.h
)

set(assertdlg_RESOURCES
	src/assertdlg.rc
	src/errordlg.rc
	src/assertdlg.ico
	src/small.ico
)

add_executable(assertdlg WIN32
	${assertdlg_SOURCES} ${assertdlg_HEADERS} ${assertdlg_RESOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${assertdlg_SOURCES} ${assertdlg_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${assertdlg_RESOURCES})
cox_target_defaults(assertdlg)

target_include_directories(assertdlg PRIVATE src)
target_compile_definitions(assertdlg PRIVATE _WINDOWS _MBCS)

target_compile_options(assertdlg PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(assertdlg PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
