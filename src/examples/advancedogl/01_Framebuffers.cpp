#include "01_Framebuffers.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void AOGL_01_Framebuffers::OnAttach() {
	m_Position = glm::vec3(-0.2f, 0.3f, -2.0f);
	m_Scale = glm::vec3(1.2f, 1.2f, 1.2f);
	m_CameraPosition = glm::vec3(0, 0, 0);
	m_FieldOfView = 90.f;

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_TexUniform = spr::createUniform("tex", spr::UniformType::Sampler);
	m_AnotherTexUniform = spr::createUniform("anotherTex", spr::UniformType::Sampler);
	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_DefaultShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/03_frag_tex.frag");
	m_BrickTexture = Utils::LoadTexture("assets/bricks.jpg");
	m_LadybugTexture = Utils::LoadTexture("assets/yps.png");

	m_RenderTargetTextureUniform = spr::createUniform("renderTargetTexture", spr::UniformType::Sampler);
	m_PostProcessShaderProgram = Utils::LoadShaderProgram("shaders/01_aogl_color_buffers.vert", "shaders/01_aogl_color_buffers.frag");

	// 0. Creates FrameBuffer Object
	glCreateFramebuffers(1, &m_Framebuffer);

	// 1. Creates Texture Object to use as a Color Buffer
	const spw::Vec2 windowSize = spw::getWindowSize();
	const int windowWidth = windowSize.X, windowHeight = windowSize.Y;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferTexture);
	glTextureStorage2D(m_ColorBufferTexture, 1, GL_RGB8, windowWidth, windowHeight);

	// 2. Creates Texture Object to use as a Depth buffer (more on this later)
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferTexture);
	glTextureStorage2D(m_DepthBufferTexture, 1, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);

	// 3. Binds Texture Object to Framebuffer Object in one of the color attachments
	glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0, m_ColorBufferTexture, 0);
	glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_ATTACHMENT, m_DepthBufferTexture, 0);

	// (OPTIONAL) Checks the completeness state of the Framebuffer
	if (glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		assert(false && "::ERROR: Framebuffer is incomplete");
	}
}

void AOGL_01_Framebuffers::OnUpdate() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, m_Position);
	model = glm::rotate(model, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, m_Scale);

	glm::mat4 view(1.0f);
	view = glm::translate(view, -m_CameraPosition);

	glm::mat4 projection(1.0f);
	const float aspectRatio = (float)(spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f;
	const float far = 100.f;
	projection = glm::perspective(m_FieldOfView, aspectRatio, near, far);

	// 4. Binds Framebuffer Object to GL context
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	glEnable(GL_DEPTH_TEST);

	// 5. Performs a render pass on the Framebuffer
	const int texUnit = 0, anotherTexUnit = 1;
	spr::setUniform(m_TexUniform, &texUnit);
	spr::setUniform(m_AnotherTexUniform, &anotherTexUnit);
	spr::setUniform(m_ModelUniform, glm::value_ptr(model));
	spr::setUniform(m_ViewUniform, glm::value_ptr(view));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(projection));

	spr::setTexture(0, m_BrickTexture);
	spr::setTexture(1, m_LadybugTexture);

	spr::setVertexBuffer(m_CubeVertexBuffer);

	spr::submit(m_DefaultShaderProgram);

	spr::clear();
	spr::render();
	spr::clean();

	// 6. Binds default Framebuffer (this is the one provided by the windowing system)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 7. Gets the Framebuffer Object color attachment and draws it in another render pass
	assert(m_QuadModel.Meshes.size() > 0 && "::ERROR: Quad mesh wasn't properly loaded");
	const auto &mesh = m_QuadModel.Meshes[0];
	spr::setVertexBuffer(mesh.VertexBuffer);
	spr::setIndexBuffer(mesh.IndexBuffer);

	const int renderTargetTextureUnit = 2;
	glBindTextureUnit(renderTargetTextureUnit, m_ColorBufferTexture);
	spr::setUniform(m_RenderTargetTextureUniform, &renderTargetTextureUnit);
	spr::submit(m_PostProcessShaderProgram);

	spr::clear();
	spr::render();
	spr::clean();
}

void AOGL_01_Framebuffers::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_BrickTexture);
	spr::destroy(m_LadybugTexture);
	spr::destroy(m_DefaultShaderProgram);
	spr::destroy(m_TexUniform);
	spr::destroy(m_AnotherTexUniform);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_PostProcessShaderProgram);
	spr::destroy(m_RenderTargetTextureUniform);

	glDeleteFramebuffers(1, &m_Framebuffer);
	glDeleteTextures(1, &m_ColorBufferTexture);
	glDeleteTextures(1, &m_DepthBufferTexture);
}
