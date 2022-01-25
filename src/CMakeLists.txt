set(SANDBOX_SOURCES
		shared/Shader.cpp
		shared/Camera.cpp
		shared/FreeCameraController.cpp
		shared/Texture2D.cpp
		shared/Mesh.cpp
		shared/Model.cpp
		shared/Window.cpp
		shared/SimpleRenderer.cpp
		shared/layers/Layer.cpp
		shared/layers/CameraLayer.cpp
		shared/layers/SandboxLayer.cpp
		shared/layers/ExampleGUILayer.cpp
		shared/ImGui/ImGuiLayer.cpp
		shared/ImGui/ImGuiBuild.cpp
		examples/learnopengl/02_VBO.cpp
		examples/learnopengl/02_EBO.cpp
		examples/learnopengl/02_VAO.cpp
		examples/learnopengl/03_Shaders.cpp
		examples/learnopengl/04_Textures.cpp
		examples/learnopengl/05_Transform.cpp
		examples/learnopengl/06_MVP.cpp
		examples/learnopengl/07_Camera.cpp
		examples/learnopengl/08_Lighting.cpp
		examples/learnopengl/09_Phong.cpp
		examples/learnopengl/10_Material.cpp
		examples/learnopengl/11_Specular_Map.cpp
		examples/learnopengl/11_Emission_Map.cpp
		examples/learnopengl/12_DirectionalLight.cpp
		examples/learnopengl/12_PointLight.cpp
		examples/learnopengl/12_SpotLight.cpp
		examples/learnopengl/12_MultipleLights.cpp
		examples/learnopengl/13_Meshes.cpp)

add_executable(Sandbox main.cpp ${SANDBOX_SOURCES})

target_link_libraries(Sandbox glfw)
target_link_libraries(Sandbox glad)
target_link_libraries(Sandbox glm)
target_link_libraries(Sandbox imgui)
target_link_libraries(Sandbox opengl32)
target_link_libraries(Sandbox assimp)
target_link_libraries(Sandbox stb)

target_compile_definitions(Sandbox PRIVATE GLFW_INCLUDE_NONE)

target_include_directories(Sandbox
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/glm
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include)

set_target_properties(Sandbox PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)