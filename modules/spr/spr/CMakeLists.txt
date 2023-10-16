set(SPR_SOURCES 
		SimpleRenderer.cpp
		Context.cpp
		FrameDataManager.cpp
				
		Utils.cpp
		ResourceManager/ResourceManager.cpp
		ResourceManager/SamplerInfo.cpp
		ResourceManager/UniformManager.cpp
		ResourceManager/VertexAttributeLayout.cpp

		OpenGL/RendererContextGL.cpp
		OpenGL/Helpers.cpp
		OpenGL/ResourceManager/ResourceManagerGL.cpp
		OpenGL/ResourceManager/VertexBufferManagerGL.cpp
		OpenGL/ResourceManager/IndexBufferManagerGL.cpp
		OpenGL/ResourceManager/UniformManagerGL.cpp
		OpenGL/ResourceManager/ShaderManagerGL.cpp
		OpenGL/ResourceManager/ProgramManagerGL.cpp
		OpenGL/ResourceManager/TextureManagerGL.cpp
		OpenGL/ResourceManager/TextureUtilsGL.cpp)


add_library(spr STATIC ${SPR_SOURCES} "OpenGL/ResourceManager/FramebufferManagerGL.cpp" "OpenGL/ResourceManager/FramebufferUtilsGL.cpp")

# sets OpenGL policy to prioritize newer versions over legacy when both are found
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)			# check https://stackoverflow.com/a/67307380

target_link_libraries(spr glad)
target_link_libraries(spr OpenGL::GL)

target_include_directories(spr PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")