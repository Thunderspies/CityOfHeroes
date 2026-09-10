if(NOT TARGET TestClient)
	message(FATAL_ERROR "TestClientLauncher requires COX_BUILD_TESTCLIENT=ON")
endif()

set(TestClientLauncher_SOURCES
	src/DlgMain.c
	src/DlgText.c
	src/PipeServer.c
	src/TestClientLauncher.c
	src/win_init.c
)

set(TestClientLauncher_HEADERS
	src/DlgMain.h
	src/DlgText.h
	src/PipeServer.h
	src/resource.h
	src/TestClientLauncher.h
	src/win_init.h
)

set(TestClientLauncher_RESOURCES
	src/Resources.rc
	src/TestClientLauncher.ico
)

add_executable(TestClientLauncher WIN32
	${TestClientLauncher_SOURCES} ${TestClientLauncher_HEADERS}
	${TestClientLauncher_RESOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${TestClientLauncher_SOURCES} ${TestClientLauncher_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Resource Files"
	FILES ${TestClientLauncher_RESOURCES})
cox_target_defaults(TestClientLauncher)
target_include_directories(TestClientLauncher PRIVATE src)
target_link_libraries(TestClientLauncher PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(TestClientLauncher PRIVATE _WINDOWS UNICODE _UNICODE)

target_compile_options(TestClientLauncher PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(TestClientLauncher PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)

include(RuntimeStaging)
cox_stage_runtime_files(TestClientLauncher TestClient
	FILES "$<TARGET_FILE:TestClient>" DEPENDS TestClient)
