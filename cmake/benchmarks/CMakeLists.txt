# The startup harness accepts this helper's path as its Capture argument.
add_executable(console_snapshot console-snapshot.c)
target_link_libraries(console_snapshot PRIVATE user32)

# Enable COX_BUILD_FILESYSTEM_BENCHMARKS in the normal Win32 CoX build. To run:
# cox_file_read <i24/data/sequencers> <sequencer-files.txt>
# piggle_source_walk <i24/data> <relative-file> <relative-directory>
# For a pre-Piggle checkout, copy this benchmark directory, add its
# add_subdirectory to that checkout, and enable COX_LEGACY. This adds only
# benchmark targets; the measured FolderCache library remains unchanged.
option(COX_LEGACY "Build the pre-Piggle FolderCache workload" OFF)

if(COX_LEGACY)
	add_executable(cox_file_read read-speed-legacy.c)
else()
	add_executable(cox_file_read read-speed.c)
	add_executable(piggle_source_walk source-speed.c)
	target_link_libraries(piggle_source_walk PRIVATE piggle)
	set_target_properties(piggle_source_walk PROPERTIES
		C_STANDARD 11 C_STANDARD_REQUIRED ON)
endif()
target_link_libraries(cox_file_read PRIVATE UtilitiesLib)
set_target_properties(cox_file_read PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED ON)
