include(dependencies/Glew)
include(dependencies/Jpeg)
include(dependencies/JpegCompressor)

add_executable(GameJpegTests jpeg.cpp ../src/graphics/jpeg.cpp)
cox_target_defaults(GameJpegTests)
target_compile_definitions(GameJpegTests PRIVATE CLIENT)
target_include_directories(GameJpegTests PRIVATE ../src)
target_link_libraries(GameJpegTests PRIVATE
	Common UtilitiesLib GLEW::GLEW JPEG::JPEG jpge::jpgd jpge::jpge)
add_test(NAME GameJpegIntegration COMMAND GameJpegTests
	"${CMAKE_CURRENT_BINARY_DIR}/jpeg-$<CONFIG>.jpg")
