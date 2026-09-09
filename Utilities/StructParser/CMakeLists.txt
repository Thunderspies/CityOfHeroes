cmake_minimum_required(VERSION 3.24)
project(StructParser LANGUAGES CXX)

set(STRUCTPARSER_SOURCES
	src/AutoRunManager.cpp
	src/AutoTestManager.cpp
	src/AutoTransactionManager.cpp
	src/GenerationIO.cpp
	src/IdentifierDictionary.cpp
	src/LateLinkManager.cpp
	src/MagicCommandManager.cpp
	src/main.cpp
	src/Platform.cpp
	src/SourceParser.cpp
	src/SourceParserBaseClass.cpp
	src/StructParser.cpp
	src/strutils.cpp
	src/Tokenizer.cpp
	src/utils.cpp
)

if(CMAKE_CROSSCOMPILING AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
	include(ExternalProject)
	find_program(STRUCTPARSER_HOST_CXX_COMPILER NAMES c++ g++ clang++
		NO_CMAKE_FIND_ROOT_PATH REQUIRED)
	if(CMAKE_GENERATOR MATCHES "Ninja")
		set(_host_generator Ninja)
	elseif(CMAKE_GENERATOR MATCHES "Makefiles")
		set(_host_generator "Unix Makefiles")
	else()
		message(FATAL_ERROR
			"StructParser host build needs Ninja or Makefiles")
	endif()
	set(_host_dir "${CMAKE_BINARY_DIR}/host-tools/StructParser")
	# Isolate the host compiler, toolchain, and environment flags.
	set(_host_env ${CMAKE_COMMAND} -E env
		--unset=CC --unset=CXX --unset=CFLAGS --unset=CXXFLAGS
		--unset=CPPFLAGS --unset=LDFLAGS --unset=RC --unset=RCFLAGS
		--unset=CMAKE_TOOLCHAIN_FILE --unset=CMAKE_GENERATOR
		--unset=CMAKE_GENERATOR_PLATFORM --unset=CMAKE_GENERATOR_TOOLSET
		--unset=CMAKE_GENERATOR_INSTANCE --unset=CMAKE_BUILD_TYPE
		--unset=CMAKE_PREFIX_PATH --unset=CMAKE_INCLUDE_PATH
		--unset=CMAKE_LIBRARY_PATH --unset=CPATH
		--unset=CPLUS_INCLUDE_PATH
		--unset=LIBRARY_PATH --unset=SDKROOT)
	set(_host_cxx "${STRUCTPARSER_HOST_CXX_COMPILER}")
	ExternalProject_Add(StructParser
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
		BINARY_DIR "${_host_dir}"
		DOWNLOAD_COMMAND ""
		UPDATE_COMMAND ""
		CONFIGURE_COMMAND ${_host_env} ${CMAKE_COMMAND}
			-S <SOURCE_DIR> -B <BINARY_DIR> -G "${_host_generator}"
			"-DCMAKE_CXX_COMPILER:FILEPATH=${_host_cxx}"
			-DCMAKE_BUILD_TYPE:STRING=Release
			-DCMAKE_TOOLCHAIN_FILE:FILEPATH=
			-DCMAKE_CXX_FLAGS:STRING=
			-DCMAKE_EXE_LINKER_FLAGS:STRING=
		BUILD_COMMAND ${_host_env} ${CMAKE_COMMAND} --build <BINARY_DIR>
		BUILD_ALWAYS TRUE
		BUILD_BYPRODUCTS "${_host_dir}/StructParser"
		INSTALL_COMMAND ""
	)
	add_executable(StructParser::Host IMPORTED GLOBAL)
	set_target_properties(StructParser::Host PROPERTIES
		IMPORTED_LOCATION "${_host_dir}/StructParser")
	add_dependencies(StructParser::Host StructParser)
else()
	add_executable(StructParser ${STRUCTPARSER_SOURCES})
	if(COMMAND cox_target_optdebug)
		cox_target_optdebug(StructParser)
	endif()
	target_compile_features(StructParser PRIVATE cxx_std_17)
	set_target_properties(StructParser PROPERTIES FOLDER "Utilities")
	if(MSVC)
		target_compile_definitions(StructParser PRIVATE
			_CRT_SECURE_NO_WARNINGS=1)
		target_compile_options(StructParser PRIVATE
			/Zc:preprocessor /Zc:__cplusplus)
	endif()
	add_executable(StructParser::Host ALIAS StructParser)
endif()
