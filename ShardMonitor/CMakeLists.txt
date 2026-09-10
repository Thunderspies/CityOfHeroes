if(NOT TARGET ServerMonitor)
	message(FATAL_ERROR "ShardMonitor requires COX_BUILD_SERVERMONITOR=ON")
endif()

set(ShardMonitor_SOURCES
	src/ShardMonitor.c
)

set(ShardMonitor_HEADERS
	src/resource.h
)

set(ShardMonitor_RESOURCES
	src/ShardMonitor.rc
	src/ShardMonitor.ico
)

add_executable(ShardMonitor WIN32
	${ShardMonitor_SOURCES} ${ShardMonitor_HEADERS}
	${ShardMonitor_RESOURCES})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${ShardMonitor_SOURCES} ${ShardMonitor_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${ShardMonitor_RESOURCES})
cox_target_defaults(ShardMonitor)
target_include_directories(ShardMonitor PRIVATE src)
target_link_libraries(ShardMonitor PRIVATE user32)
target_compile_definitions(ShardMonitor PRIVATE _WINDOWS UNICODE _UNICODE)
target_compile_options(ShardMonitor PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(ShardMonitor PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(RuntimeStaging)
cox_stage_runtime_files(ShardMonitor ServerMonitor
	FILES "$<TARGET_FILE:ServerMonitor>" DEPENDS ServerMonitor)
