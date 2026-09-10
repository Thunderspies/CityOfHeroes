# Compile the fork's complete MapServer manifest with beacon behavior enabled.
# MapServer's single parser owner supplies the shared generated sources.
add_executable(BeaconServer
	${MapServer_SOURCES} ${MapServer_HEADERS}
	${MapServer_RESOURCES} ${MapServer_IMAGES} ${MapServer_GENERATED})
cox_configure_map_server(BeaconServer)
target_compile_definitions(BeaconServer PRIVATE BEACONIZER=1)
add_dependencies(BeaconServer MapServer_StructParser)

cox_stage_physx(BeaconServer)
cox_stage_executable_alias(BeaconServer BeaconClient)
