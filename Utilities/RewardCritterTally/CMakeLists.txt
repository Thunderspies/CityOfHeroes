set(rewardcrittertally_SOURCES
	src/main.c
)

add_executable(rewardcrittertally ${rewardcrittertally_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
	${rewardcrittertally_SOURCES})
cox_target_defaults(rewardcrittertally)

# Preserve the executable name from the fork's Visual Studio project.
set_target_properties(rewardcrittertally PROPERTIES OUTPUT_NAME RewardCritterTally)
target_compile_definitions(rewardcrittertally PRIVATE _CONSOLE UNICODE _UNICODE)

target_compile_options(rewardcrittertally PRIVATE
	$<$<CONFIG:Release,OptDebug>:/Gy;/Oi;/Zi>)
target_link_options(rewardcrittertally PRIVATE
	$<$<CONFIG:Release,OptDebug>:/DEBUG;/OPT:REF;/OPT:ICF>)
