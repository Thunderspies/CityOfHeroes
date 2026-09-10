cmake_minimum_required(VERSION 3.24...3.31)

project(CoX LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CoXConfigurations)
include(CoXTargets)

set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)
option(COX_BUILD_UTILITIES_TESTS "Build UtilitiesLib integration checks" OFF)
option(COX_BUILD_PIG_TESTS "Build Pig command-line tests" OFF)
if(COX_BUILD_UTILITIES_TESTS OR COX_BUILD_PIG_TESTS)
	enable_testing()
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

if(MSVC)
	add_compile_options(
		$<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:/fp:precise>)
endif()

# Run this mandatory check before other targets, even with testing disabled.
add_executable(cox_precise_floating_point_test EXCLUDE_FROM_ALL
	cmake/tests/precise_floating_point.cpp)
target_compile_features(cox_precise_floating_point_test PRIVATE cxx_std_17)
if(MINGW)
	# Keep the check runnable without compiler runtime DLL copies.
	target_link_options(cox_precise_floating_point_test PRIVATE -static)
endif()
add_custom_target(verify_precise_floating_point ALL
	COMMAND cox_precise_floating_point_test
	DEPENDS cox_precise_floating_point_test
	COMMENT "Verifying precise floating-point behavior"
	VERBATIM)

add_subdirectory(Utilities/StructParser)

if(WIN32)
	if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
		message(FATAL_ERROR "CoX supports only 32-bit Windows builds")
	endif()
	enable_language(RC)
	include(dependencies/Zlib)
	include(dependencies/CryptoPP)
	add_subdirectory(libs/crashrpt)
	add_subdirectory(libs/UtilitiesLib)
	add_subdirectory(Utilities/pig)
	if(COX_MSVC_WIN32)
		include(dependencies/Lua)
		include(dependencies/PhysX)
		add_subdirectory(libs/AILib)
	endif()
endif()

cox_add_project(Common Common)


# Require verification for explicit target builds as well as the default build.
function(cox_require_precise_floating_point directory)
	get_property(targets DIRECTORY "${directory}" PROPERTY BUILDSYSTEM_TARGETS)
	foreach(target IN LISTS targets)
		if(target STREQUAL "cox_precise_floating_point_test" OR
		   target STREQUAL "verify_precise_floating_point")
			continue()
		endif()

		get_target_property(imported "${target}" IMPORTED)
		get_target_property(target_type "${target}" TYPE)
		if(NOT imported AND NOT target_type STREQUAL "INTERFACE_LIBRARY")
			add_dependencies("${target}" verify_precise_floating_point)
		endif()
	endforeach()

	get_property(subdirectories DIRECTORY "${directory}" PROPERTY SUBDIRECTORIES)
	foreach(subdirectory IN LISTS subdirectories)
		cox_require_precise_floating_point("${subdirectory}")
	endforeach()
endfunction()

cox_require_precise_floating_point("${CMAKE_CURRENT_SOURCE_DIR}")
