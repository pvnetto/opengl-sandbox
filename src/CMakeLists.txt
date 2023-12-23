set(SANDBOX_SOURCES
		
		shared/Camera.cpp
		shared/FreeCameraController.cpp
		shared/ModelLoader.cpp
		shared/RenderUtils.cpp
		shared/Runtime.cpp
		shared/Utils.cpp
		shared/GlUtils.cpp

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
		examples/learnopengl/13_Meshes.cpp
		examples/advancedogl/01_Framebuffers.cpp
		examples/advancedogl/01_StencilBuffers.cpp
		examples/advancedogl/01_DepthBuffers.cpp
		examples/advancedogl/02_Blending.cpp
		examples/advancedogl/03_Antialiasing.cpp
		examples/advancedogl/04_Instancing.cpp
		examples/advancedogl/05_GeometryShading.cpp
		examples/advancedogl/06_Tessellation.cpp
		examples/simplerenderer/01_Stenciling.cpp
		examples/simplerenderer/02_OrderIndependentBlending.cpp
		examples/simplerenderer/03_Instancing.cpp
		examples/simplerenderer/04_HDR.cpp
		)


add_executable(Sandbox main.cpp ${SANDBOX_SOURCES})

target_link_libraries(Sandbox glm)
target_link_libraries(Sandbox imgui)
target_link_libraries(Sandbox assimp)
target_link_libraries(Sandbox stb)
target_link_libraries(Sandbox spw)
target_link_libraries(Sandbox spr)

target_compile_definitions(Sandbox PRIVATE GLFW_INCLUDE_NONE)

target_include_directories(Sandbox
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/spr
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/spw
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/glm
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include)

# consider hard-coding this or using an environment variable, so we can make it consistent with whatever is on the Python script
set(SANDBOX_BINARY_DIR "bin/${CMAKE_SYSTEM_NAME}")
message("::PACKAGING Set binary directory to ${SANDBOX_BINARY_DIR}")
set_target_properties(Sandbox PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${SANDBOX_BINARY_DIR}
                    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/${SANDBOX_BINARY_DIR}/debug
                    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/${SANDBOX_BINARY_DIR}/release)


# adds staging step to Sandbox target
include("${CMAKE_SOURCE_DIR}/cmake/staging.cmake")
add_staging(Sandbox)
stage_directory(assets)
stage_directory(shaders)

