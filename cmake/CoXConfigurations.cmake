include_guard(GLOBAL)

# Include after project(), before dependencies or targets. Keep existing build
# defaults; OptDebug is an additional optimized, non-FINAL configuration.
if(CMAKE_CONFIGURATION_TYPES AND NOT "OptDebug" IN_LIST CMAKE_CONFIGURATION_TYPES)
	list(APPEND CMAKE_CONFIGURATION_TYPES OptDebug)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
		"Available build configurations" FORCE)
endif()
if(DEFINED CACHE{CMAKE_BUILD_TYPE})
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		Debug Release RelWithDebInfo MinSizeRel OptDebug)
endif()

# CMake creates empty custom-configuration cache entries when a user selects
# OptDebug before it is supported. Initialize those as well as missing entries,
# but retain nonempty user overrides. Vendor targets need the compiler/CRT
# configuration too; engine feature macros remain target-scoped below.
foreach(language C CXX)
	set(flags "${CMAKE_${language}_FLAGS_RELEASE}")
	if(MSVC)
		string(APPEND flags " /Zi /Oy-")
	else()
		string(APPEND flags " -g -fno-omit-frame-pointer")
	endif()
	if("${CMAKE_${language}_FLAGS_OPTDEBUG}" STREQUAL "")
		set(CMAKE_${language}_FLAGS_OPTDEBUG "${flags}" CACHE STRING
			"${language} flags for OptDebug" FORCE)
	endif()
endforeach()
foreach(kind EXE SHARED MODULE STATIC)
	set(flags "${CMAKE_${kind}_LINKER_FLAGS_RELEASE}")
	if(MSVC AND NOT kind STREQUAL "STATIC")
		string(APPEND flags " /DEBUG /OPT:REF /OPT:ICF")
	endif()
	if("${CMAKE_${kind}_LINKER_FLAGS_OPTDEBUG}" STREQUAL "")
		set(CMAKE_${kind}_LINKER_FLAGS_OPTDEBUG "${flags}" CACHE STRING
			"${kind} linker flags for OptDebug" FORCE)
	endif()
endforeach()
unset(flags)
unset(language)
unset(kind)

# Use PUBLIC for libraries whose headers depend on the engine configuration.
# This intentionally does not change their existing Debug/Release contracts.
function(cox_target_optdebug target)
	set(visibility PRIVATE)
	if(ARGC GREATER 1)
		set(visibility "${ARGV1}")
	endif()
	target_compile_definitions(${target} ${visibility}
		$<$<CONFIG:OptDebug>:_OPTDEBUG=1;NDEBUG=1>)
endfunction()
