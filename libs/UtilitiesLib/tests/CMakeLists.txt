add_executable(UtilitiesLibTests main.c allocation.cpp nchash.c)
set_target_properties(UtilitiesLibTests PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED ON
	CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(UtilitiesLibTests PRIVATE
	"$<LINK_LIBRARY:WHOLE_ARCHIVE,UtilitiesLib>")
if(MINGW)
	target_link_options(UtilitiesLibTests PRIVATE -static)
endif()
add_test(NAME UtilitiesLibIntegration COMMAND UtilitiesLibTests)
set_tests_properties(UtilitiesLibIntegration PROPERTIES TIMEOUT 120)

add_executable(TokenizerTests tokenizer.c)
set_target_properties(TokenizerTests PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
target_link_libraries(TokenizerTests PRIVATE UtilitiesLib)
if(MINGW)
	target_link_options(TokenizerTests PRIVATE -static)
endif()
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tokenizer-fixtures")
add_test(NAME TokenizerLineEndings COMMAND TokenizerTests)
set_tests_properties(TokenizerLineEndings PROPERTIES
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tokenizer-fixtures" TIMEOUT 30)

add_executable(DevelopmentModeTests development_mode.c)
set_target_properties(DevelopmentModeTests PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED ON)
target_link_libraries(DevelopmentModeTests PRIVATE UtilitiesLib)
target_compile_definitions(DevelopmentModeTests PRIVATE
	COX_EXPECT_OPTDEBUG=$<CONFIG:OptDebug>
	COX_EXPECT_DEV_FEATURES=$<IF:$<CONFIG:Debug,OptDebug>,1,0>)
if(MINGW)
	target_link_options(DevelopmentModeTests PRIVATE -static)
endif()
# Each CTest invocation is a fresh process: fileIsUsingDevData caches its result.
set(dev_fixture "${CMAKE_CURRENT_BINARY_DIR}/development-fixtures")
file(MAKE_DIRECTORY "${dev_fixture}/loose" "${dev_fixture}/packaged/piggs")
file(WRITE "${dev_fixture}/packaged/piggs/texts.pigg" "")
foreach(mode loose packaged)
	add_test(NAME DevelopmentMode_${mode} COMMAND DevelopmentModeTests ${mode})
	set_tests_properties(DevelopmentMode_${mode} PROPERTIES
		WORKING_DIRECTORY "${dev_fixture}/${mode}" TIMEOUT 20
		PASS_REGULAR_EXPRESSION "$<IF:$<CONFIG:Debug,OptDebug>,COX_DEVELOPMENT_LOG,Development mode checks passed>"
		FAIL_REGULAR_EXPRESSION "$<$<NOT:$<CONFIG:Debug,OptDebug>>:COX_DEVELOPMENT_LOG>")
endforeach()
