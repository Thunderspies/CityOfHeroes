add_executable(PigTests main.cpp)
set_target_properties(PigTests PROPERTIES
	CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON
	FOLDER "Utilities")
add_dependencies(PigTests pig)
if(MINGW)
	target_link_options(PigTests PRIVATE -static)
endif()

foreach(case IN ITEMS PigUsage PigRoundTrip PigIncremental)
	add_test(NAME ${case} COMMAND PigTests ${case})
	set_tests_properties(${case} PROPERTIES LABELS Pig TIMEOUT 180)
endforeach()
