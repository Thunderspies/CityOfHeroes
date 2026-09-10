set(RemoteDebuggerer_SOURCES
	src/main.c
)

set(RemoteDebuggerer_HEADERS
	src/resource.h
)

set(RemoteDebuggerer_RESOURCES
	src/RemoteDebugger.rc
	src/icon1.ico
)

add_executable(RemoteDebuggerer WIN32
	${RemoteDebuggerer_SOURCES} ${RemoteDebuggerer_HEADERS}
	${RemoteDebuggerer_RESOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${RemoteDebuggerer_SOURCES} ${RemoteDebuggerer_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${RemoteDebuggerer_RESOURCES})
cox_target_defaults(RemoteDebuggerer)

# Keep the fork's executable name while retaining the migrated target identity.
set_target_properties(RemoteDebuggerer PROPERTIES OUTPUT_NAME RemoteDebugger)
target_include_directories(RemoteDebuggerer PRIVATE src)
target_link_libraries(RemoteDebuggerer PRIVATE UtilitiesLib)
target_compile_definitions(RemoteDebuggerer PRIVATE _WINDOWS _MBCS)

target_compile_options(RemoteDebuggerer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(RemoteDebuggerer PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
