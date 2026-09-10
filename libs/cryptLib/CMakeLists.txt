add_library(cryptLib STATIC
	src/sha512.cpp
	src/sha512Impl.cpp
	src/sha512Impl.h
	include/cryptlib/sha.h
	include/cryptlib/sha512.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	src/sha512.cpp
	src/sha512Impl.cpp
	src/sha512Impl.h
	include/cryptlib/sha.h
	include/cryptlib/sha512.h)

target_include_directories(cryptLib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(cryptLib PROPERTIES
	CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
cox_target_optdebug(cryptLib)
target_compile_definitions(cryptLib PRIVATE WIN32 _LIB
	$<$<CONFIG:Debug>:_DEBUG>
	$<$<NOT:$<CONFIG:Debug>>:NDEBUG>)
target_compile_options(cryptLib PRIVATE /W3 /sdl /permissive- /MP)
