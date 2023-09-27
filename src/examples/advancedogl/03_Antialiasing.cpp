#include "03_Antialiasing.h"

#include "shared/RenderUtils.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include <iostream>

void AOGL_03_Antialiasing::OnAttach() {
	m_Position = glm::vec3(0.f, 0.f, -5.0f);
	m_Scale = glm::vec3(3.f, 3.f, 3.f);
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 0.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_LastSampleCountOption = m_SampleCountOption;

	const spw::Vec2i windowSize = spw::getWindowSize();
	const float aspectRatio = windowSize.X / windowSize.Y;
	m_Camera.SetPerspective(90.f, aspectRatio, 0.1f, 100.f);

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_ColorUniform = spr::createUniform("color", spr::UniformType::Vec4);
	m_DefaultShaderProgram = Utils::LoadShaderProgram("shaders/default_unlit.vert", "shaders/default_unlit.frag");

	glClearColor(0xa1, 0xa1, 0xa1, 0xff);

	// 0. Creates Framebuffer and Multisampled Textures to use as attachments
	// NOTE: If you're using GLFW, you can configure the Default Framebuffer to use MSAA with glfwWindowHint(GLFW_SAMPLES, numSamples), without
	// needing to create an offscreen Framebuffer for this.
	CreateMSAAFramebuffer();
}

void AOGL_03_Antialiasing::CreateMSAAFramebuffer() {
	const int sampleCount = std::pow(2, m_SampleCountOption + 1);
	const spw::Vec2i resolution = spw::getWindowSize();

	// 1. Creates Multisampled Textures. Note that we're using the GL_TEXTURE_2D_MULTISAMPLE target instead of the usual GL_TEXTURE_2D.
	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_MSAAColorTexture);
	glTextureStorage2DMultisample(m_MSAAColorTexture, sampleCount, GL_RGB8, resolution.X, resolution.Y, GL_TRUE);

	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_MSAADepthStencilTexture);
	glTextureStorage2DMultisample(m_MSAADepthStencilTexture, sampleCount, GL_DEPTH24_STENCIL8, resolution.X, resolution.Y, GL_TRUE);
	
	// 2. Binds Multisampled Textures to the Framebuffer
	glCreateFramebuffers(1, &m_MSAAFramebuffer);
	glNamedFramebufferTexture(m_MSAAFramebuffer, GL_COLOR_ATTACHMENT0, m_MSAAColorTexture, 0);
	glNamedFramebufferTexture(m_MSAAFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, m_MSAADepthStencilTexture, 0);


	// (OPTIONAL) Gets the number of samples from OpenGL to guarantee we're sampling the correct amount
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSAAFramebuffer);
	int activeSamples;
	glGetIntegerv(GL_SAMPLES, &activeSamples);
	std::cout << "::DISPLAY: Created new MSAA Framebuffer with " << activeSamples << " samples." << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AOGL_03_Antialiasing::InvalidateMSAAFramebuffer() {
	glDeleteFramebuffers(1, &m_MSAAFramebuffer);
	glDeleteTextures(1, &m_MSAAColorTexture);
	glDeleteTextures(1, &m_MSAADepthStencilTexture);
}

void AOGL_03_Antialiasing::OnUpdate() {
	// 3. Enable multisampling. This is enabled by default in most modern OpenGL implementations.
	if (m_IsMultisamplingEnabled) {
		glEnable(GL_MULTISAMPLE);
	}
	else {
		glDisable(GL_MULTISAMPLE);
	}

	// (OPTIONAL) If the number of samples changes, we should invalidate the old Framebuffer and create a new one with the correct amount of samples
	if (m_SampleCountOption != m_LastSampleCountOption) {
		InvalidateMSAAFramebuffer();
		CreateMSAAFramebuffer();
		m_LastSampleCountOption = m_SampleCountOption;
	}

	// 4. Performs a render pass using the Multisampled Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSAAFramebuffer);

	glm::mat4 projection(1.0f);
	const float aspectRatio = (float)(spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f, far = 1000.f;
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));

	glm::mat4 model(1.0f);
	model = glm::translate(model, m_Position);
	model = glm::rotate(model, 45.f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, m_Scale);

	const glm::vec4 color{0.12f, 0.12f, 0.12f, 1.f};
	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(model));
	spr::setUniform(m_ColorUniform, glm::value_ptr(color));
	spr::clear();
	spr::submit(m_DefaultShaderProgram);
	spr::flush();

	// 5. Performs multisample resolve (i.e., blits from a Multisampled Framebuffer to a Non-Multisampled one, in this case the Default Framebuffer)
	const int defaultFramebuffer = 0;
	const spw::Vec2i resolution = spw::getWindowSize();
	glBlitNamedFramebuffer(m_MSAAFramebuffer, defaultFramebuffer,
	                       0, 0, resolution.X, resolution.Y,
	                       0, 0, resolution.X, resolution.Y,
	                       GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
	// 6. Binds resolved Default Framebuffer, as this is the one we want to draw
	// NOTE: We don't need another render pass to draw this Framebuffer, as GLFW swaps it to the front with glfwSwapBuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AOGL_03_Antialiasing::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_DefaultShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ColorUniform);
	InvalidateMSAAFramebuffer();
}

void AOGL_03_Antialiasing::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));
	ImGui::Begin("03 - Antialiasing");

	ImGui::Checkbox("Enabled", &m_IsMultisamplingEnabled);
	ImGui::Spacing();
	ImGui::Combo("MSAA", &m_SampleCountOption,
	             "2x\0"
	             "4x\0"
	             "8x\0");
	ImGui::Spacing();

	ImGui::End();
}
