set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)

# Use a toolchain built with UCRT, including its runtime libraries.
set(_mingw_prefix i686-w64-mingw32)
set(CMAKE_C_COMPILER "${_mingw_prefix}-gcc")
set(CMAKE_CXX_COMPILER "${_mingw_prefix}-g++")
set(CMAKE_RC_COMPILER "${_mingw_prefix}-windres")

# Run cross-compiled build checks and CTest tests through Wine by default.
set(CMAKE_CROSSCOMPILING_EMULATOR wine CACHE STRING
	"Emulator used to run Windows executables on the build host")

if(NOT CMAKE_FIND_ROOT_PATH)
	execute_process(
		COMMAND "${CMAKE_C_COMPILER}" -print-sysroot
		OUTPUT_VARIABLE CMAKE_FIND_ROOT_PATH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		COMMAND_ERROR_IS_FATAL ANY
	)
endif()
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

unset(_mingw_prefix)
