set(AILib_SOURCES
	src/aiBehaviorDebug.c
	src/aiBehaviorInterface.c
	src/aiBehaviorProcess.c
	src/aiBScript.c
	src/aiExpression.c
	src/aiLib.c
	src/aiMessages.c
)

set(AILib_HEADERS
	include/ailib/aiBehaviorDebug.h
	include/ailib/aiBehaviorInterface.h
	include/ailib/aiBehaviorPublic.h
	include/ailib/aiExpression.h
	include/ailib/aiLib.h
	include/ailib/aiStruct.h
	src/aiBehaviorPrivate.h
)

add_library(AILib STATIC ${AILib_SOURCES} ${AILib_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${AILib_SOURCES} ${AILib_HEADERS})
set_target_properties(AILib PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED ON
	CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_include_directories(AILib PUBLIC include)
target_link_libraries(AILib PUBLIC UtilitiesLib)
target_compile_definitions(AILib PRIVATE
	WIN32 _LIB
	_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE
	_WINSOCK_DEPRECATED_NO_WARNINGS
	$<$<CONFIG:Debug>:FULLDEBUG;_CRTDBG_MAP_ALLOC;_DEBUG>
	$<$<NOT:$<CONFIG:Debug,OptDebug>>:FINAL;NDEBUG>)
target_compile_options(AILib PRIVATE /W3 /sdl /permissive- /wd4996
	$<$<COMPILE_LANGUAGE:C,CXX>:/MP>)

include(StructParser)
StructParse(AILib)
