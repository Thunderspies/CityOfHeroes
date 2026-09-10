include_guard(GLOBAL)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86")
	message(FATAL_ERROR "The bundled NVParse library requires MSVC Win32")
endif()

include(dependencies/Glew)
find_package(OpenGL REQUIRED)

# NVParse remains a legacy source dependency exception. Compile the fork's
# Win32 project manifest, including its checked-in flex/bison outputs.
# ps1.0__test_main.cpp is an unused GLUT application, not a library unit.
function(cox_add_nvparse)
	set(root "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../3rdparty/nvparse")
	cmake_path(NORMAL_PATH root)
	set(sources
		nvparse.cpp
		nvparse_errors.cpp
		ps1.0_program.cpp
		rc1.0_combiners.cpp
		rc1.0_final.cpp
		rc1.0_general.cpp
		ts1.0_inst.cpp
		ts1.0_inst_list.cpp
		vcp1.0_impl.cpp
		vp1.0_impl.cpp
		vs1.0_inst.cpp
		vs1.0_inst_list.cpp
		vsp1.0_impl.cpp
		_ps1.0_lexer.cpp
		_ps1.0_parser.cpp
		_rc1.0_lexer.cpp
		_rc1.0_parser.cpp
		_ts1.0_lexer.cpp
		_ts1.0_parser.cpp
		_vs1.0_lexer.cpp
		_vs1.0_parser.cpp)
	set(headers
		macro.h
		nvparse_errors.h
		nvparse_externs.h
		ps1.0_program.h
		rc1.0_combiners.h
		rc1.0_final.h
		rc1.0_general.h
		rc1.0_register.h
		ts1.0_inst.h
		ts1.0_inst_list.h
		vs1.0_inst.h
		vs1.0_inst_list.h
		_ps1.0_parser.h
		_rc1.0_parser.h
		_ts1.0_parser.h
		_vs1.0_parser.h)
	list(TRANSFORM sources PREPEND "${root}/src/")
	list(TRANSFORM headers PREPEND "${root}/src/")
	list(APPEND headers "${root}/include/nvparse/nvparse.h")
	foreach(path IN LISTS sources headers)
		if(NOT EXISTS "${path}")
			message(FATAL_ERROR "Incomplete bundled NVParse: missing ${path}")
		endif()
	endforeach()
	add_library(nvparse_static STATIC EXCLUDE_FROM_ALL ${sources} ${headers})
	add_library(NvParse::NvParse ALIAS nvparse_static)
	target_include_directories(nvparse_static PUBLIC "${root}/include")
	target_link_libraries(nvparse_static PRIVATE GLEW::GLEW OpenGL::GLU)
	target_compile_definitions(nvparse_static PRIVATE
		WIN32 _LIB _MBCS _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS)
	target_compile_options(nvparse_static PRIVATE /W3 /sdl)
	set_target_properties(nvparse_static PROPERTIES
		CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

cox_add_nvparse()
