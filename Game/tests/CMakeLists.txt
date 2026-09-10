include(dependencies/Glew)
include(dependencies/Jpeg)
include(dependencies/JpegCompressor)
include(dependencies/Vorbis)

add_executable(GameOggTests ogg.c)
cox_target_defaults(GameOggTests)
target_compile_definitions(GameOggTests PRIVATE CLIENT)
target_include_directories(GameOggTests PRIVATE ../src)
target_link_libraries(GameOggTests PRIVATE
	Common UtilitiesLib Vorbis::vorbisfile Vorbis::vorbisenc)
add_test(NAME GameOggIntegration COMMAND GameOggTests)
set_tests_properties(GameOggIntegration PROPERTIES TIMEOUT 30)

add_executable(GameJpegTests jpeg.cpp ../src/graphics/jpeg.cpp)
cox_target_defaults(GameJpegTests)
target_compile_definitions(GameJpegTests PRIVATE CLIENT)
target_include_directories(GameJpegTests PRIVATE ../src)
target_link_libraries(GameJpegTests PRIVATE
	Common UtilitiesLib GLEW::GLEW JPEG::JPEG jpge::jpgd jpge::jpge)
add_test(NAME GameJpegIntegration COMMAND GameJpegTests
	"${CMAKE_CURRENT_BINARY_DIR}/jpeg-$<CONFIG>.jpg")
