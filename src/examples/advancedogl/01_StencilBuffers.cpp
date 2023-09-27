#include "01_StencilBuffers.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void AOGL_01_StencilBuffers::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_ObjectColor = glm::vec3{0.2f, 0.35f, 1.f};
	m_LightColor = glm::vec3{0.8f, 0.8f, 1.f};


	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_ResolutionUniform = spr::createUniform("inResolution", spr::UniformType::Vec2);
	m_TimeUniform = spr::createUniform("inTime", spr::UniformType::Float);
	m_StencilPassShaderProgram = Utils::LoadShaderProgram("shaders/01_aogl_stencil_buffers_sdf.vert", "shaders/01_aogl_stencil_buffers_sdf.frag");

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);
	m_LightColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_LightPositionUniform = spr::createUniform("lightPos", spr::UniformType::Vec3);
	m_ObjectColorUniform = spr::createUniform("objectColor", spr::UniformType::Vec3);
	m_LightingShaderProgram = Utils::LoadShaderProgram("shaders/07_phong.vert", "shaders/07_phong.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");

	m_RenderTargetTextureUniform = spr::createUniform("renderTargetTexture", spr::UniformType::Sampler);
	m_RenderPassShaderProgram = Utils::LoadShaderProgram("shaders/draw_textured_quad.vert", "shaders/draw_textured_quad.frag");

	glCreateFramebuffers(1, &m_Framebuffer);

	const spw::Vec2i windowSize = spw::getWindowSize();
	const int windowWidth = windowSize.X, windowHeight = windowSize.Y;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferTexture);
	glTextureStorage2D(m_ColorBufferTexture, 1, GL_RGB8, windowWidth, windowHeight);

	// 0. Creates Texture Object to use as both a Stencil Buffer and Depth Buffer
	glCreateTextures(GL_TEXTURE_2D, 1, &m_StencilDepthBufferTexture);
	glTextureStorage2D(m_StencilDepthBufferTexture, 1, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);

	// 1. Attaches Texture Objects to Framebuffer Object, including the Stencil Texture we just created
	glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0, m_ColorBufferTexture, 0);
	glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, m_StencilDepthBufferTexture, 0);

	// (OPTIONAL) Checks the completeness state of the Framebuffer
	if (auto status = glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE) {
		assert(false && "::ERROR: Framebuffer is incomplete");
	}

	// 2. Enables stencil testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
}

void AOGL_01_StencilBuffers::OnUpdate() {
	/* Stencil Pass */
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

		// 3. Sets stencil testing parameters
		glStencilFunc(GL_ALWAYS, 1, 0xff);					// Sets Stencil Value to 1, comparison function is set to ALWAYS so all fragments pass the test and are written to the buffer.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xff);								// Determines which bits of the Stencil Buffer we're allowed to write to.
	
		// (OPTIONAL) Disables writing to the Color Buffer. We do this because the purpose of the first pass is to setup the stencil buffer.
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		// 4. Clears Stencil Buffer before writing to it. This will reset the Stencil Buffer to all 0s.
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		glClear(GL_STENCIL_BUFFER_BIT);

		// 5. Draws quad with circle hole to the screen to fill the stencil buffer with 1s
		// At this point, the Stencil Buffer will look something like this:
		// 1 1 1 1 1 1 1 1
		// 1 1 1 1 1 1 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 1 1 1 1 1 1
		// 1 1 1 1 1 1 1 1
		const spw::Vec2i windowSize = spw::getWindowSize();
		spr::setUniform(m_ResolutionUniform, &windowSize);

		const float currentTime = Runtime::get()->getTime();
		spr::setUniform(m_TimeUniform, &currentTime);

		const auto &quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);
		spr::submit(m_StencilPassShaderProgram);

		spr::clear();
		spr::render();
		spr::clean();
	}

	/* Render Pass */
	{
		// 6. Configures the Render Pass to use the Stencil Buffer as a difference mask (i.e., don't draw where the mask is 1)
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);				// Sets comparison function to NOTEQUAL, so that we only draw in places where the Stencil not 1 (or 0, pretty much a subtract mask).
		glStencilMask(0x00);								// Disables writing to the Stencil Buffer.

		// (OPTIONAL) Reenables writing to the Color Buffer.
		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glm::vec3 lightSourcePos = glm::vec3(std::cos(Runtime::get()->getTime()),
		                                     std::cos(Runtime::get()->getTime()) * std::sin(Runtime::get()->getTime()),
		                                     std::sin(Runtime::get()->getTime()) * 2.f);

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, lightSourcePos);
		sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
		spr::submit(m_LightSourceShaderProgram);

		glm::mat4 litModel(1.0f);
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
		spr::setUniform(m_ObjectColorUniform, glm::value_ptr(m_ObjectColor));
		spr::submit(m_LightingShaderProgram);

		glClear(GL_DEPTH_BUFFER_BIT);
		spr::render();
		spr::clean();
	}

	/* Draw Pass */
	{
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
}

void AOGL_01_StencilBuffers::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_LightingShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_ObjectColorUniform);
	spr::destroy(m_RenderPassShaderProgram);
	spr::destroy(m_RenderTargetTextureUniform);
	spr::destroy(m_StencilPassShaderProgram);
	spr::destroy(m_ResolutionUniform);
	spr::destroy(m_TimeUniform);

	glDeleteFramebuffers(1, &m_Framebuffer);
	glDeleteTextures(1, &m_ColorBufferTexture);
	glDeleteTextures(1, &m_StencilDepthBufferTexture);
}
