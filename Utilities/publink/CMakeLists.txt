set(publink_SOURCES
	src/publink.c
)

add_executable(publink ${publink_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${publink_SOURCES})
cox_target_defaults(publink)

target_link_libraries(publink PRIVATE Common UtilitiesLib)
target_compile_definitions(publink PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(publink PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(publink PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
