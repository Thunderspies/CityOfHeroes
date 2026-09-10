set(FtpGrabber_SOURCES
	src/FtpGrabber.c
	src/getopt.c
)

set(FtpGrabber_HEADERS
	src/getopt.h
)

add_executable(FtpGrabber ${FtpGrabber_SOURCES} ${FtpGrabber_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${FtpGrabber_SOURCES} ${FtpGrabber_HEADERS})
cox_target_defaults(FtpGrabber)

target_include_directories(FtpGrabber PRIVATE src)
target_link_libraries(FtpGrabber PRIVATE
	UtilitiesLib cryptopp::cryptopp ZLIB::ZLIB)
target_compile_definitions(FtpGrabber PRIVATE _CONSOLE _MBCS)

target_compile_options(FtpGrabber PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(FtpGrabber PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
