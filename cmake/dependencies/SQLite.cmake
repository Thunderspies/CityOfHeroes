include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME sqlite
	VERSION 3.53.4
	URL https://www.sqlite.org/2026/sqlite-amalgamation-3530400.zip
	URL_HASH
		SHA3_256=628a44cfe82c66aed1ccbbe85a562d2e33ebe64b3288981ed76285612227934e
	DOWNLOAD_ONLY YES
)

# Preserve memtrack's amalgamation and default feature/thread configuration.
# The shell and loadable-extension export list are not part of this library.
add_library(sqlite_static STATIC EXCLUDE_FROM_ALL
	"${sqlite_SOURCE_DIR}/sqlite3.c"
	"${sqlite_SOURCE_DIR}/sqlite3.h"
	"${sqlite_SOURCE_DIR}/sqlite3ext.h")
add_library(SQLite::SQLite3 ALIAS sqlite_static)
target_include_directories(sqlite_static PUBLIC "${sqlite_SOURCE_DIR}")
if(MSVC)
	target_compile_definitions(sqlite_static PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(NOT WIN32)
	find_package(Threads REQUIRED)
	target_link_libraries(sqlite_static PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
endif()
set_target_properties(sqlite_static PROPERTIES
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
