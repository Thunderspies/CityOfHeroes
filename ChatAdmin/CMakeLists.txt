set(ChatAdmin_SOURCES
	src/ChannelList.c
	src/ChannelMonitor.c
	src/ChatAdmin.c
	src/ChatAdminNet.c
	src/ChatAdminUtils.c
	src/CustomDialogs.c
	src/LangAdminUtil.c
	src/Login.c
	src/MessageView.c
	src/UserList.c
	src/UserMonitor.c
	src/VirtualListView.c
)

set(ChatAdmin_HEADERS
	src/ChannelList.h
	src/ChannelMonitor.h
	src/ChatAdmin.h
	src/ChatAdminNet.h
	src/ChatAdminUtils.h
	src/ChatComm.h
	src/CustomDialogs.h
	src/LangAdminUtil.h
	src/Login.h
	src/MessageView.h
	src/resource.h
	src/UserList.h
	src/UserMonitor.h
	src/VirtualListView.h
)

set(ChatAdmin_RESOURCES
	src/ChatAdmin.rc
	src/icon1.ico
)

add_executable(ChatAdmin WIN32
	${ChatAdmin_SOURCES} ${ChatAdmin_HEADERS} ${ChatAdmin_RESOURCES})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${ChatAdmin_SOURCES} ${ChatAdmin_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${ChatAdmin_RESOURCES})
cox_target_defaults(ChatAdmin)
target_include_directories(ChatAdmin PRIVATE src)
target_link_libraries(ChatAdmin PRIVATE
	Common UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(ChatAdmin PRIVATE _WINDOWS _MBCS)
target_compile_options(ChatAdmin PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(ChatAdmin PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
