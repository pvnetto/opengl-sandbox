#include "01_DepthBuffers.h"

#include "shared/RenderUtils.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void AOGL_01_DepthBuffers::OnAttach() {
	m_Position = glm::vec3(-0.5f, 0.f, -2.0f);
	m_Scale = glm::vec3(1.2f, 1.2f, 1.2f);
	m_CameraPosition = glm::vec3(0, 0, 0);
	m_FieldOfView = 90.f;

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_NearPlaneUniform = spr::createUniform("nearPlane", spr::UniformType::Float);
	m_FarPlaneUniform = spr::createUniform("farPlane", spr::UniformType::Float);
	m_DepthDrawShaderProgram = Utils::LoadShaderProgram("shaders/01_aogl_depth_buffers.vert", "shaders/01_aogl_depth_buffers.frag");

	m_RenderTargetTextureUniform = spr::createUniform("renderTargetTexture", spr::UniformType::Sampler);
	m_RenderPassShaderProgram = Utils::LoadShaderProgram("shaders/draw_textured_quad.vert", "shaders/draw_textured_quad.frag");

	// 0. Creates FrameBuffer Object
	glCreateFramebuffers(1, &m_Framebuffer);

	const spw::Vec2 windowSize = spw::getWindowSize();
	const int windowWidth = windowSize.X, windowHeight = windowSize.Y;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferTexture);
	glTextureStorage2D(m_ColorBufferTexture, 1, GL_RGB8, windowWidth, windowHeight);

	// 1. Creates Texture Object to use as a Depth buffer
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferTexture);
	glTextureStorage2D(m_DepthBufferTexture, 1, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);

	// 2. Binds depth buffer Texture Object to Framebuffer Object in the depth attachment
	glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0, m_ColorBufferTexture, 0);
	glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_ATTACHMENT, m_DepthBufferTexture, 0);

	// (OPTIONAL) Checks the completeness state of the Framebuffer
	if (glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		assert(false && "::ERROR: Framebuffer is incomplete");
	}

	glClearColor(0xff, 0x00, 0xff, 0xff);
}

void AOGL_01_DepthBuffers::OnUpdate() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	// 3. Enables depth testing
	glEnable(GL_DEPTH_TEST);
	
	// 4. (OPTIONAL) Sets depth testing parameters. This already comes with sensible defaults though.
	glDepthMask(GL_TRUE);			// Defaults to GL_TRUE. This value is ANDed with depth values before they're written to the Depth Buffer.
	glDepthFunc(GL_LESS);			// Defaults to GL_LESS. Replaces depth value when it's smaller than the current (i.e., closer to the viewer).

	// 5. Renders a bunch of cubes at different depths and draws their depth values
	glm::mat4 view(1.0f);
	view = glm::translate(view, -m_CameraPosition);

	glm::mat4 projection(1.0f);
	const float aspectRatio = (float)(spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f, far = 1000.f;
	projection = glm::perspective(m_FieldOfView, aspectRatio, near, far);
	spr::setUniform(m_ViewUniform, glm::value_ptr(view));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(projection));
	spr::setUniform(m_NearPlaneUniform, &near);
	spr::setUniform(m_FarPlaneUniform, &far);

	const int cubeCount = 10;
	for (int i = 0; i < cubeCount; i++) {
		const float offsetMultiplier = (float) i - cubeCount / 2.f;
		const glm::vec3 offset = offsetMultiplier * glm::vec3{ 0.25f, 0.f, 0.25f };
		glm::mat4 model(1.0f);
		model = glm::translate(model, m_Position + offset);
		model = glm::scale(model, m_Scale);

		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(model));
		spr::submit(m_DepthDrawShaderProgram);
	}


	// 6. Clears values in the Depth Buffer before writing to it
	glClear(GL_DEPTH_BUFFER_BIT);
	spr::clear();
	spr::render();
	spr::clean();

	// 7. Performs another render pass to draw the result to a quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	assert(m_QuadModel.Meshes.size() > 0 && "::ERROR: Quad mesh wasn't properly loaded");
	const auto &mesh = m_QuadModel.Meshes[0];
	spr::setVertexBuffer(mesh.VertexBuffer);
	spr::setIndexBuffer(mesh.IndexBuffer);

	const int renderTargetTextureUnit = 0;
	glBindTextureUnit(renderTargetTextureUnit, m_ColorBufferTexture);
	spr::setUniform(m_RenderTargetTextureUniform, &renderTargetTextureUnit);
	spr::submit(m_RenderPassShaderProgram);

	spr::clear();
	spr::render();
	spr::clean();
}

void AOGL_01_DepthBuffers::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_DepthDrawShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_RenderPassShaderProgram);
	spr::destroy(m_RenderTargetTextureUniform);
	spr::destroy(m_NearPlaneUniform);
	spr::destroy(m_FarPlaneUniform);

	glDeleteFramebuffers(1, &m_Framebuffer);
	glDeleteTextures(1, &m_ColorBufferTexture);
	glDeleteTextures(1, &m_DepthBufferTexture);
}
