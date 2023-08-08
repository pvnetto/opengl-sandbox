function(add_staging dependency)
	add_custom_target(Staging DEPENDS "${dependency}")
	

	# Now you can use CMAKE_RUNTIME_OUTPUT_DIRECTORY_<CONFIG> to determine the output directory
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		get_target_property(CURRENT_RUNTIME_OUTPUT_DIRECTORY "${dependency}" RUNTIME_OUTPUT_DIRECTORY_DEBUG)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		get_target_property(CURRENT_RUNTIME_OUTPUT_DIRECTORY "${dependency}" RUNTIME_OUTPUT_DIRECTORY_RELEASE)
	else()
		get_target_property(CURRENT_RUNTIME_OUTPUT_DIRECTORY "${dependency}" RUNTIME_OUTPUT_DIRECTORY)
	endif()

	set_target_properties(Staging PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CURRENT_RUNTIME_OUTPUT_DIRECTORY}")
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
