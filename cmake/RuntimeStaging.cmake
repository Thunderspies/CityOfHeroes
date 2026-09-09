include_guard(GLOBAL)

# Register runtime files shared by a package's enabled consumers. RENAMED
# entries use source|filename for SDKs whose loader expects another name.
# DEPENDS names targets that produce runtime files inside this build.
# One producer stages each resolved output directory once per requested build.
function(cox_stage_runtime_files target package)
	cmake_parse_arguments(PARSE_ARGV 2 stage "" "" "FILES;RENAMED;DEPENDS")
	if(stage_UNPARSED_ARGUMENTS OR NOT package MATCHES "^[A-Za-z0-9_]+$")
		message(FATAL_ERROR "Invalid runtime staging package: ${package}")
	endif()
	set(key "COX_RUNTIME_${package}")
	set(manifest "${CMAKE_BINARY_DIR}/runtime/${package}.$<CONFIG>.cmake")
	if(NOT TARGET cox_stage_${package})
		set_property(GLOBAL APPEND PROPERTY COX_RUNTIME_PACKAGES "${package}")
		set_property(GLOBAL PROPERTY ${key}_FILES "${stage_FILES}")
		set_property(GLOBAL PROPERTY ${key}_RENAMED "${stage_RENAMED}")
		add_custom_target(cox_stage_${package}
			COMMAND "${CMAKE_COMMAND}" "-DCOX_STAGE_MANIFEST=${manifest}"
				-P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/StageRuntime.cmake"
			COMMENT "Staging ${package} runtime"
			VERBATIM)
	else()
		foreach(kind FILES RENAMED)
			get_property(previous GLOBAL PROPERTY ${key}_${kind})
			if(NOT "${previous}" STREQUAL "${stage_${kind}}")
				message(FATAL_ERROR "Conflicting runtime files for ${package}")
			endif()
		endforeach()
	endif()
	set_property(GLOBAL APPEND PROPERTY ${key}_DIRECTORIES
		"$<TARGET_FILE_DIR:${target}>")
	if(stage_DEPENDS)
		add_dependencies(cox_stage_${package} ${stage_DEPENDS})
	endif()
	add_dependencies(${target} cox_stage_${package})
	get_property(deferred GLOBAL PROPERTY COX_RUNTIME_FINALIZER)
	if(NOT deferred)
		set_property(GLOBAL PROPERTY COX_RUNTIME_FINALIZER TRUE)
		cmake_language(DEFER DIRECTORY "${CMAKE_SOURCE_DIR}"
			CALL cox_finalize_runtime_staging)
	endif()
endfunction()

function(cox_finalize_runtime_staging)
	get_property(packages GLOBAL PROPERTY COX_RUNTIME_PACKAGES)
	foreach(package IN LISTS packages)
		set(content "")
		foreach(kind FILES RENAMED DIRECTORIES)
			get_property(values GLOBAL PROPERTY COX_RUNTIME_${package}_${kind})
			string(APPEND content "set(stage_${kind} [==[${values}]==])\n")
		endforeach()
		file(GENERATE
			OUTPUT "${CMAKE_BINARY_DIR}/runtime/${package}.$<CONFIG>.cmake"
			CONTENT "${content}")
	endforeach()
endfunction()
