add_library(Common INTERFACE)

target_include_directories(Common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if(COX_BUILD_UTILITIES_TESTS)
	add_executable(ContainerPackingTests
		tests/container_packing.c container/dbcontainerpack.c)
	cox_target_defaults(ContainerPackingTests)
	target_link_libraries(ContainerPackingTests PRIVATE Common UtilitiesLib)
	add_test(NAME ContainerPacking COMMAND ContainerPackingTests)
endif()
