set(ChatClient_SOURCES
	src/chatclient.c
	src/coh_net.c
	src/coh_string.c
	src/md5.c
)

set(ChatClient_HEADERS
	src/coh_net.h
	src/coh_string.h
	src/md5.h
)

add_executable(ChatClient ${ChatClient_SOURCES} ${ChatClient_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${ChatClient_SOURCES} ${ChatClient_HEADERS})
cox_target_defaults(ChatClient)

target_include_directories(ChatClient PRIVATE src)
target_link_libraries(ChatClient PRIVATE Ws2_32)
target_compile_definitions(ChatClient PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(ChatClient PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(ChatClient PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
