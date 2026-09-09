if(NOT EXISTS "${COX_ALIAS_EXE}")
	return()
endif()
get_filename_component(directory "${COX_ALIAS_EXE}" DIRECTORY)
file(COPY_FILE "${COX_ALIAS_EXE}"
	"${directory}/${COX_ALIAS_NAME}.exe" ONLY_IF_DIFFERENT)
if(EXISTS "${COX_ALIAS_PDB}" AND NOT IS_DIRECTORY "${COX_ALIAS_PDB}")
	file(COPY_FILE "${COX_ALIAS_PDB}"
		"${directory}/${COX_ALIAS_NAME}.pdb" ONLY_IF_DIFFERENT)
endif()
