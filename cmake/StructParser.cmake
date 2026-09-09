include_guard()

# StructParse(target [NAME identity]) scans sources before compilation. NAME
# preserves one logical parser owner when alternate executables share sources.
# Call after assigning sources. The target's source directory must contain src/;
# Common/ lives at the top of the source tree. Generated files retain their
# existing locations in those directories. StructParser must be runnable on
# the build host. Errors from the parser fail the dependent target's build.
function(StructParse INPUT_TARGET)
	cmake_parse_arguments(PARSE_ARGV 1 parser "" "NAME" "")
	if(parser_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unexpected StructParse arguments: ${INPUT_TARGET}")
	endif()
	if(NOT parser_NAME)
		set(parser_NAME "${INPUT_TARGET}")
	endif()
	if(NOT parser_NAME MATCHES "^[A-Za-z0-9_]+$")
		message(FATAL_ERROR "Invalid StructParser identity: ${parser_NAME}")
	endif()
	get_target_property(_sources ${INPUT_TARGET} SOURCES)
	get_target_property(_source_dir ${INPUT_TARGET} SOURCE_DIR)
	get_target_property(_binary_dir ${INPUT_TARGET} BINARY_DIR)
	set(_source_list
		"${_binary_dir}/${parser_NAME}.$<CONFIG>.StructParser.sources")
	set(_content "")
	set(_outputs "")
	foreach(_source IN LISTS _sources)
		set(_absolute "$<PATH:ABSOLUTE_PATH,${_source},${_source_dir}>")
		string(TOLOWER "${_source}" _lower_source)
		if(_lower_source MATCHES "autogen")
			list(APPEND _outputs "${_absolute}")
			set_source_files_properties("${_source}"
				PROPERTIES GENERATED TRUE)
		endif()
		string(APPEND _content "$<$<BOOL:${_source}>:${_absolute}>\n")
	endforeach()
	file(GENERATE OUTPUT "${_source_list}" CONTENT "${_content}"
		TARGET ${INPUT_TARGET})

	get_target_property(_type ${INPUT_TARGET} TYPE)
	if(_type STREQUAL "EXECUTABLE")
		set(_kind executable)
	else()
		set(_kind library)
	endif()

	if(CMAKE_CONFIGURATION_TYPES)
		set(_configuration "$<CONFIG>")
	elseif(CMAKE_BUILD_TYPE)
		set(_configuration "${CMAKE_BUILD_TYPE}")
	else()
		set(_configuration "Default")
	endif()
	set(_int_dir "${_binary_dir}/${parser_NAME}.dir/${_configuration}")

	add_custom_target(${parser_NAME}_StructParser
		COMMAND $<TARGET_FILE:StructParser::Host>
			--target "${parser_NAME}"
			--source-dir "${_source_dir}/src"
			--common-dir "${CMAKE_SOURCE_DIR}/Common"
			--project-dir "${_source_dir}"
			--kind "${_kind}"
			--sources-file "${_source_list}"
			--state-dir "${_int_dir}/StructParser"
		BYPRODUCTS ${_outputs}
		DEPENDS "${_source_list}"
		COMMENT "Running StructParser on ${parser_NAME}"
		VERBATIM
	)
	add_dependencies(${parser_NAME}_StructParser StructParser)
	add_dependencies(${INPUT_TARGET} ${parser_NAME}_StructParser)
endfunction()
