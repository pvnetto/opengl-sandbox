function(add_staging deps)
	add_custom_target(Staging
		DEPENDS "${deps}")
	
	get_target_property(DEPS_RUNTIME_OUTPUT_DIR "${deps}" RUNTIME_OUTPUT_DIRECTORY)
	set_target_properties(Staging PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${DEPS_RUNTIME_OUTPUT_DIR}")
endfunction()

function(stage_directory relative_path)
	# makes absolute path from input path
	set(staging_source_dir "${CMAKE_CURRENT_LIST_DIR}/${relative_path}")

	# gets path relative to root, so we can use the same filesystem on the output folder
	set(staging_source_dir_rel "")
	string(REPLACE "${CMAKE_SOURCE_DIR}/src" "" staging_source_dir_rel "${staging_source_dir}")
	get_target_property(STAGING_OUTPUT_DIR Staging RUNTIME_OUTPUT_DIRECTORY)
	set(staging_target_dir "${STAGING_OUTPUT_DIR}/${staging_source_dir_rel}")

	message("::STAGING: Added ${staging_source_dir} to staging (target: ${staging_target_dir})")
	add_custom_command(
		TARGET Sandbox
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${staging_source_dir}"	
			"${staging_target_dir}"
		COMMAND ${CMAKE_COMMAND} -E echo "::STAGING: Staged ${staging_source_dir} to ${staging_target_dir}" 
	)
endfunction()
