set(LWCpipetest_SOURCES
	src/LWC_pipe_test.cpp
	src/stdafx.cpp
)

set(LWCpipetest_HEADERS
	src/stdafx.h
)

add_executable(LWCpipetest ${LWCpipetest_SOURCES} ${LWCpipetest_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${LWCpipetest_SOURCES} ${LWCpipetest_HEADERS})
cox_target_defaults(LWCpipetest)

# Preserve the executable name from the fork's Visual Studio project.
set_target_properties(LWCpipetest PROPERTIES OUTPUT_NAME LWC_pipe_test)
target_include_directories(LWCpipetest PRIVATE src)
target_compile_definitions(LWCpipetest PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(LWCpipetest PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(LWCpipetest PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
