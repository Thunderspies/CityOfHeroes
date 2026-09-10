set(OGLextextractor_SOURCES
	src/OGL_ext_extractor.cpp
	src/stdafx.cpp
)

set(OGLextextractor_HEADERS
	src/stdafx.h
)

add_executable(OGLextextractor
	${OGLextextractor_SOURCES} ${OGLextextractor_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${OGLextextractor_SOURCES} ${OGLextextractor_HEADERS})
cox_target_defaults(OGLextextractor)

# Preserve the executable name from the fork's Visual Studio project.
set_target_properties(OGLextextractor PROPERTIES OUTPUT_NAME OGL_ext_extractor)
target_include_directories(OGLextextractor PRIVATE src)
target_compile_definitions(OGLextextractor PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(OGLextextractor PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(OGLextextractor PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
