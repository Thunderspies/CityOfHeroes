set(AuthServer_SOURCES
	src/Account.cpp
	src/AccountDB.cpp
	src/Blowfish.cpp
	src/buildVersion.cpp
	src/Config.cpp
	src/CSocketInt.cpp
	src/CSocketServer.cpp
	src/DBConn.cpp
	src/Des.c
	src/Exception.cpp
	src/GlobalAuth.cpp
	src/IOBuffer.cpp
	src/IOObject.cpp
	src/IOServer.cpp
	src/IOSocket.cpp
	src/IPList.cpp
	src/IPSessionDB.cpp
	src/Job.cpp
	src/Lock.cpp
	src/Log.cpp
	src/LogSocket.cpp
	src/md5.cpp
	src/Memory.cpp
	src/PacketServerEx.cpp
	src/Parser.cpp
	src/Reporter.cpp
	src/ServerId.cpp
	src/ServerList.cpp
	src/SocketServerEx.cpp
	src/Thread.cpp
	src/util.cpp
	src/WantedSocket.cpp
)

set(AuthServer_HEADERS
	src/Account.h
	src/AccountDB.h
	src/AuthType.h
	src/Blowfish.h
	src/buildn.h
	src/Config.h
	src/DBConn.h
	src/Des.h
	src/Exception.h
	src/GlobalAuth.h
	src/IOBuffer.h
	src/IOObject.h
	src/IOServer.h
	src/IOSocket.h
	src/IPList.h
	src/IPSessionDB.h
	src/Job.h
	src/Lock.h
	src/Log.h
	src/logprotocol.h
	src/LogSocket.h
	src/md5.h
	src/Memory.h
	src/Packet.h
	src/Parser.h
	src/PreComp.h
	src/Protocol.h
	src/Reporter.h
	src/resource.h
	src/ServerId.h
	src/ServerList.h
	src/Thread.h
	src/util.h
	src/WantedSocket.h
)

add_executable(AuthServer WIN32
	${AuthServer_SOURCES} ${AuthServer_HEADERS} src/resource.rc)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${AuthServer_SOURCES} ${AuthServer_HEADERS} src/resource.rc)
set_target_properties(AuthServer PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED ON
	CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(AuthServer PRIVATE
	cryptLib cryptopp::cryptopp ws2_32 dbghelp mswsock odbc32)

# AuthServer has its own configuration contract, without the engine's FULLDEBUG.
cox_target_optdebug(AuthServer)
target_compile_definitions(AuthServer PRIVATE
	WIN32 _WINDOWS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS
	_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING
	$<$<CONFIG:Debug>:_DEBUG>
	$<$<NOT:$<CONFIG:Debug,OptDebug>>:FINAL;NDEBUG>)
target_compile_options(AuthServer PRIVATE /W3 /sdl /permissive-
	$<$<COMPILE_LANGUAGE:C,CXX>:/MP>
	$<$<COMPILE_LANGUAGE:CXX>:/EHa>)
target_link_options(AuthServer PRIVATE /LARGEADDRESSAWARE)
