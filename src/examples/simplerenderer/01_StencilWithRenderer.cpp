#include "01_StencilWithRenderer.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void SPRE_01_StencilWithRenderer::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_ObjectColor = { 0.2f, 0.35f, 1.f };
	m_LightColor = { 0.8f, 0.8f, 1.f };

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
	m_ObjectColorUniform = spr::createUniform("objectColor", spr::UniformType::Vec4);
	m_ColorUniform = spr::createUniform("color", spr::UniformType::Vec4);
	
	m_LightingShaderProgram = Utils::LoadShaderProgram("shaders/07_phong.vert", "shaders/07_phong.frag");
	m_UnlitShaderProgram = Utils::LoadShaderProgram("shaders/default_unlit.vert", "shaders/default_unlit.frag");


	static const int msaaSampleCount = 8;
	const auto &[windowWidth, windowHeight] = spw::getWindowSize();
	spr::TextureInfo colorTextureInfo;
	colorTextureInfo.Width			= windowWidth;
	colorTextureInfo.Height			= windowHeight;
	colorTextureInfo.Format			= spr::TextureFormat::RGB8;
	colorTextureInfo.Flags			|= (uint8_t) spr::TextureFlags::IsRenderTargetTexture;
	colorTextureInfo.NumSamples		= msaaSampleCount; 
	m_ColorBufferTexture = spr::createTexture(colorTextureInfo, nullptr);

	spr::TextureInfo stencilDepthTextureInfo;
	stencilDepthTextureInfo.Width			= windowWidth;
	stencilDepthTextureInfo.Height			= windowHeight;
	stencilDepthTextureInfo.Format			= spr::TextureFormat::DEPTH_24_STENCIL_8;
	stencilDepthTextureInfo.Flags			|= (uint8_t)spr::TextureFlags::IsRenderTargetTexture;
	stencilDepthTextureInfo.NumSamples		= msaaSampleCount; 
	m_StencilDepthBufferTexture = spr::createTexture(stencilDepthTextureInfo, nullptr);

	m_Framebuffer = spr::createFramebuffer({
		{ spr::FramebufferAttachmentType::Color0,		m_ColorBufferTexture },
		{ spr::FramebufferAttachmentType::DepthStencil, m_StencilDepthBufferTexture },
	});

	// TODO: Move to RenderTarget clear config
	glClearColor(0.88f, 0.88f, 0.88f, 1.0f);
}

using ClearFlags = spr::FramebufferAttachmentFlags;

void SPRE_01_StencilWithRenderer::OnUpdate() {
	// 0. Sets up render targets
	const auto &[windowWidth, windowHeight] = spw::getWindowSize();

	const uint8_t stencilPassTarget = 0;
	spr::setRenderTargetFramebuffer(stencilPassTarget, m_Framebuffer);
	spr::setRenderTargetClear(stencilPassTarget, spr::AsFlag(ClearFlags::All));
	spr::setRenderTargetRect(stencilPassTarget, { 0, 0, (uint32_t) windowWidth, (uint32_t) windowHeight });

	const uint8_t renderPassTarget = 1;
	spr::setRenderTargetFramebuffer(renderPassTarget, m_Framebuffer);
	spr::setRenderTargetClear(renderPassTarget, 0);
	spr::setRenderTargetRect(renderPassTarget, { 0, 0, (uint32_t) windowWidth, (uint32_t) windowHeight });

	// 1. Stencil Pass:
	// - Performs a render pass on a framebuffer while writing only to the stencil buffer;
	// - The purpose of this pass is to fill the stencil buffer with values that can be used by another pass on the same framebuffer;
	{
		spr::FixedFunctionState state;
		state.SetStencilTestEnabled(true);
		state.SetStencilWriteMask(0xff);
		state.SetStencilTest(spr::StencilBufferState::TestFnAlways, 0x01, 0xff);
		state.SetStencilOp(spr::StencilBufferState::OpKeep, spr::StencilBufferState::OpKeep, spr::StencilBufferState::OpReplace);
		spr::setFixedFunctionState(state);

		const spw::Vec2 windowSize = spw::getWindowSize();
		spr::setUniform(m_ResolutionUniform, &windowSize);

		const float currentTime = Runtime::get()->getTime();
		spr::setUniform(m_TimeUniform, &currentTime);

		const auto &quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);
		spr::submit(stencilPassTarget, m_StencilPassShaderProgram);
	}

	// 2. Render Pass:
	// - Performs a Render Pass on the same Framebuffer that already contains Stencil values;
	// - The idea here is to disable Stencil writing and only use Stencil values for testing;
	// - We're only going to write to fragments whose Stencil values are 0 (i.e., subtract mask);
	{
		const glm::vec3 lightSourcePos = glm::vec3(std::cos(Runtime::get()->getTime()),
		                                     std::cos(Runtime::get()->getTime()) * std::sin(Runtime::get()->getTime()),
		                                     std::sin(Runtime::get()->getTime()) * 2.f);

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, lightSourcePos);
		sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

		spr::FixedFunctionState renderPassState;
		renderPassState.SetStencilTestEnabled(true);
		renderPassState.SetStencilWriteMask(0x00);
		renderPassState.SetStencilTest(spr::StencilBufferState::TestFnNotEqual, 0x01, 0xff);
		renderPassState.SetStencilOp(spr::StencilBufferState::OpKeep, spr::StencilBufferState::OpKeep, spr::StencilBufferState::OpKeep);

		const glm::vec4 lightSourceColor{m_LightColor.r, m_LightColor.g, m_LightColor.b, 1.f};
		spr::setFixedFunctionState(renderPassState);
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ColorUniform, glm::value_ptr(lightSourceColor));
		spr::submit(renderPassTarget, m_UnlitShaderProgram);

		glm::mat4 litModel(1.0f);
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

		spr::setFixedFunctionState(renderPassState);
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
		spr::setUniform(m_ObjectColorUniform, glm::value_ptr(m_ObjectColor));
		spr::submit(renderPassTarget, m_LightingShaderProgram);
	}

	// 3. Blit + Flip:
	// - Blits the contents from the Framebuffer we've used on the previous passes to the Default Framebuffer;
	// - We're doing this to force the windowing system (in our case GLFW) to swap it with the front buffer and draw it to the screen;
	// - Alternatively, we could also get the resulting Render Target Texture from the Render Pass and draw it to a quad;
	{
		const spr::FramebufferHandle defaultFramebuffer = spr::kInvalidHandle;
		const auto &[windowWidth, windowHeight] = spw::getWindowSize();

		spr::BlitParameters blitParameters;
		blitParameters.Source				= m_Framebuffer;
		blitParameters.Destination			= defaultFramebuffer;
		blitParameters.SourceRect			= { 0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight };
		blitParameters.DestinationRect		= { 0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight };
		blitParameters.BlitMask				|= spr::AsFlag(spr::FramebufferAttachmentFlags::Color);
		spr::blit(blitParameters);
	}

	spr::flush();
}

void SPRE_01_StencilWithRenderer::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_LightingShaderProgram);
	spr::destroy(m_UnlitShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_ObjectColorUniform);
	spr::destroy(m_ColorUniform);
	spr::destroy(m_StencilPassShaderProgram);
	spr::destroy(m_ResolutionUniform);
	spr::destroy(m_TimeUniform);
	spr::destroy(m_Framebuffer);
	spr::destroy(m_ColorBufferTexture);
	spr::destroy(m_StencilDepthBufferTexture);
}
