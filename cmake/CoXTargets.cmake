include_guard(GLOBAL)

set(COX_MSVC_WIN32 OFF)
if(WIN32 AND MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(COX_MSVC_WIN32 ON)
endif()

option(COX_BUILD_DEV_TOOLS "Build diagnostic utilities" OFF)
option(COX_BUILD_ART_TOOLS "Build art conversion tools" OFF)

# Register a product with an individually overridable build option. GROUP
# selects the default only; an explicit option is never silently disabled.
function(cox_add_project target directory)
	cmake_parse_arguments(PARSE_ARGV 2 project "" "GROUP" "")
	if(project_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unexpected project arguments for ${target}")
	endif()
	set(default "${COX_MSVC_WIN32}")
	if(project_GROUP)
		if(NOT project_GROUP MATCHES "^(DEV_TOOLS|ART_TOOLS)$")
			message(FATAL_ERROR "Unknown target group: ${project_GROUP}")
		endif()
		if(NOT COX_BUILD_${project_GROUP})
			set(default OFF)
		endif()
	endif()
	string(TOUPPER "${target}" option_suffix)
	set(option_name "COX_BUILD_${option_suffix}")
	option(${option_name} "Build ${target} (MSVC Win32 only)" ${default})
	if(${option_name})
		if(NOT COX_MSVC_WIN32)
			message(FATAL_ERROR
				"${target} requires MSVC and 32-bit Windows (Win32)")
		endif()
		add_subdirectory("${directory}")
	endif()
endfunction()

# Restore a colocated executable/PDB alias on every requested target build,
# then refresh it after a link. Component expressions avoid a dependency cycle
# under CMP0112; the pre-build copy simply skips files absent on a fresh build.
function(cox_stage_executable_alias target alias)
	if(NOT MSVC OR NOT alias MATCHES "^[A-Za-z0-9_-]+$")
		message(FATAL_ERROR "Unsupported executable alias: ${target}/${alias}")
	endif()
	string(CONCAT executable "$<TARGET_FILE_DIR:${target}>/"
		"$<TARGET_FILE_NAME:${target}>")
	string(CONCAT pdb "$<TARGET_PDB_FILE_DIR:${target}>/"
		"$<TARGET_PDB_FILE_NAME:${target}>")
	set(stage_command "${CMAKE_COMMAND}"
		"-DCOX_ALIAS_EXE=${executable}" "-DCOX_ALIAS_PDB=${pdb}"
		"-DCOX_ALIAS_NAME=${alias}"
		-P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/StageTargetAlias.cmake")
	add_custom_target(${target}_${alias}
		COMMAND ${stage_command}
		COMMENT "Restoring ${alias} executable alias"
		VERBATIM)
	add_dependencies(${target} ${target}_${alias})
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${stage_command}
		COMMENT "Refreshing ${alias} executable alias"
		VERBATIM)
endfunction()

# Apply the engine's common language, warning and configuration conventions.
# Targets with different CRT/debug contracts set their own definitions.
function(cox_target_defaults target)
	cox_target_optdebug(${target})
	set_target_properties(${target} PROPERTIES
		C_STANDARD 11 C_STANDARD_REQUIRED ON
		CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
	target_compile_definitions(${target} PRIVATE
		WIN32 _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE
		_WINSOCK_DEPRECATED_NO_WARNINGS
		$<$<CONFIG:Debug>:FULLDEBUG;_CRTDBG_MAP_ALLOC;_DEBUG>
		$<$<NOT:$<CONFIG:Debug,OptDebug>>:FINAL;NDEBUG>)
	target_compile_options(${target} PRIVATE /W3 /sdl /permissive- /wd4996
		$<$<COMPILE_LANGUAGE:C,CXX>:/MP>)
endfunction()
