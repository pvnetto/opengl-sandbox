set(SANDBOX_SOURCES
		shared/Shader.cpp
		shared/Camera.cpp
		shared/FreeCameraController.cpp
		shared/Texture2D.cpp
		shared/Mesh.cpp
		shared/Model.cpp)

add_executable(Sandbox main.cpp ${SANDBOX_SOURCES})

target_link_libraries(Sandbox glfw)
target_link_libraries(Sandbox glad)
target_link_libraries(Sandbox glm)
target_link_libraries(Sandbox opengl32)
target_link_libraries(Sandbox assimp)
target_link_libraries(Sandbox stb)

target_compile_definitions(Sandbox PRIVATE GLFW_INCLUDE_NONE)

target_include_directories(Sandbox
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/glm
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include
)

set_target_properties(Sandbox PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release
)