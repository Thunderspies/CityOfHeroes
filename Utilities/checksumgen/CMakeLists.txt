set(checksumgen_SOURCES
	src/checksumgen.c
	src/filechecksum.c
	src/patchfileutils.c
	src/patchutils.c
)

set(checksumgen_HEADERS
	src/filechecksum.h
	src/patchfileutils.h
	src/patchutils.h
)

add_executable(checksumgen ${checksumgen_SOURCES} ${checksumgen_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${checksumgen_SOURCES} ${checksumgen_HEADERS})
cox_target_defaults(checksumgen)

target_include_directories(checksumgen PRIVATE src)
target_link_libraries(checksumgen PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(checksumgen PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(checksumgen PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(checksumgen PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
