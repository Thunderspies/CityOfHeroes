add_executable(pig src/pig.c src/pig.rc)
set_target_properties(pig PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED ON
	FOLDER "Utilities")
target_link_libraries(pig PRIVATE UtilitiesLib)
target_compile_definitions(pig PRIVATE
	_CONSOLE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
if(MINGW)
	target_link_options(pig PRIVATE -static)
endif()
if(COX_BUILD_PIG_TESTS)
	add_subdirectory(tests)
endif()
