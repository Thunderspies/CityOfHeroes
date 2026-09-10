set(ServerLib_SOURCES
	src/serverlib_main.c
	../../Common/ServerLib/serverlib_meta.c
	../../Common/persist/persist.c
	../../Common/persist/persist_diffbig.c
	../../Common/persist/persist_diffsmall.c
	../../Common/persist/persist_flat.c
	../../Common/persist/textcrcdb.c
	../../Common/persist/textcrcjournal.c
)

set(ServerLib_HEADERS
	include/serverlib/serverlib.h
	../../Common/ServerLib/serverlib_meta.h
	../../Common/persist/persist.h
	../../Common/persist/persist_diffbig.h
	../../Common/persist/persist_diffsmall.h
	../../Common/persist/persist_flat.h
	../../Common/persist/persist_internal.h
	../../Common/persist/textcrcdb.h
	../../Common/persist/textcrcjournal.h
)

# These files are included by the persistence sources, not compiled separately.
# StructParser writes AST files beneath this target's source directory.
set(ServerLib_GENERATED
	src/AutoGen/persist_h_ast.c
	src/AutoGen/persist_h_ast.h
	src/AutoGen/persist_internal_h_ast.c
	src/AutoGen/persist_internal_h_ast.h
)
set_source_files_properties(${ServerLib_GENERATED} PROPERTIES
	GENERATED TRUE HEADER_FILE_ONLY TRUE)

add_library(ServerLib STATIC
	${ServerLib_SOURCES} ${ServerLib_HEADERS} ${ServerLib_GENERATED})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES
	${ServerLib_SOURCES} ${ServerLib_HEADERS} ${ServerLib_GENERATED})
cox_target_defaults(ServerLib)
target_include_directories(ServerLib PUBLIC include PRIVATE src)
target_link_libraries(ServerLib PUBLIC Common UtilitiesLib)
target_compile_definitions(ServerLib PRIVATE PROJECT_SERVERLIB _LIB)

include(StructParser)
StructParse(ServerLib)
