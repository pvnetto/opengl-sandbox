#include "05_Bloom.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

void SPRE_05_Bloom::OnAttach() {
	m_Camera = Camera(glm::vec3(-6.f, 6.f, 6.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);

	m_BaseLightColor = {1.f, 0.1f, 0.6f};

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	// Uniforms
	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);
	m_LightColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_LightPositionUniform = spr::createUniform("lightPos", spr::UniformType::Vec3);
	m_AlbedoUniform = spr::createUniform("albedo", spr::UniformType::Sampler);
	m_TilingUniform = spr::createUniform("tiling", spr::UniformType::Float);
	m_AmbientIntensityUniform = spr::createUniform("ambientIntensity", spr::UniformType::Float);
	m_SpecularShininessUniform = spr::createUniform("specularShininess", spr::UniformType::Float);
	m_SpecularStrengthUniform = spr::createUniform("specularStrength", spr::UniformType::Float);
	m_ColorUniform = spr::createUniform("color", spr::UniformType::Vec4);
	m_RenderTargetTextureUniform = spr::createUniform("renderTargetTexture", spr::UniformType::Sampler);
	m_BrightnessFilterThresholdUniform = spr::createUniform("brightnessThreshold", spr::UniformType::Float);
	m_RenderTargetTextureUniform = spr::createUniform("renderTargetTexture", spr::UniformType::Sampler);
	m_GaussianBlurHorizontalUniform = spr::createUniform("bIsHorizontal", spr::UniformType::Integer);
	m_GaussianBlurPrimaryTextureUniform = spr::createUniform("primaryTexture", spr::UniformType::Sampler);
	m_GaussianBlurSecondaryTextureUniform = spr::createUniform("secondaryTexture", spr::UniformType::Sampler);
	m_AdditiveTextureUniformA = spr::createUniform("factorTextureA", spr::UniformType::Sampler);
	m_AdditiveTextureUniformB = spr::createUniform("factorTextureB", spr::UniformType::Sampler);
	m_ToneMappingTypeUniform = spr::createUniform("tonemappingType", spr::UniformType::Integer);
	m_ToneMappingExposureUniform = spr::createUniform("exposure", spr::UniformType::Integer);
	m_GammaCorrectionRatioUniform = spr::createUniform("gammaCorrectionRatio", spr::UniformType::Integer);

	// Textures
	m_AlbedoTexture = Utils::LoadTexture("assets/bricks.jpg");

	// Shaders
	m_LitShaderProgram					= Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/default_lit.frag");
	m_UnlitShaderProgram				= Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/default_unlit.frag");
	m_BrightnessFilterShaderProgram		= Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/postprocess_brightness_filter.frag");
	m_GaussianBlurShaderProgram			= Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/postprocess_gaussian_blur.frag");
	m_AdditiveShaderProgram				= Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/postprocess_additive_pass.frag");
	m_ToneMappingShaderProgram			= Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/postprocess_tonemapping.frag");

	// Framebuffers
	const auto &[windowWidth, windowHeight] = spw::getWindowSize();
	spr::TextureInfo hdrRenderTargetTextureInfo;
	hdrRenderTargetTextureInfo.Width = std::round(windowWidth);
	hdrRenderTargetTextureInfo.Height = std::round(windowHeight);
	hdrRenderTargetTextureInfo.Format = spr::TextureFormat::RGBA16F;
	hdrRenderTargetTextureInfo.Flags |= (uint8_t)spr::TextureFlags::IsRenderTargetTexture;
	m_HDRColorBufferTexture = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);

	spr::TextureInfo stencilDepthTextureInfo;
	stencilDepthTextureInfo.Width = windowWidth;
	stencilDepthTextureInfo.Height = windowHeight;
	stencilDepthTextureInfo.Format = spr::TextureFormat::DEPTH_24_STENCIL_8;
	stencilDepthTextureInfo.Flags |= (uint8_t)spr::TextureFlags::IsRenderTargetTexture;
	m_HDRStencilDepthBufferTexture = spr::createTexture(stencilDepthTextureInfo, nullptr);
	m_HDRFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0, m_HDRColorBufferTexture},
	    {spr::FramebufferAttachmentType::DepthStencil, m_HDRStencilDepthBufferTexture},
	});

	m_BrightnessFilterColorTexture = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);
	m_BrightnessFilterBloomColorTexture = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);
	m_BrightnessFilterFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0, m_BrightnessFilterColorTexture},
	    {spr::FramebufferAttachmentType::Color1, m_BrightnessFilterBloomColorTexture},
	});

	m_GaussianBlurColorBufferTextureA = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);
	m_GaussianBlurColorBufferTextureB = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);
	m_GaussianBlurFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0, m_GaussianBlurColorBufferTextureA},
	    {spr::FramebufferAttachmentType::Color1, m_GaussianBlurColorBufferTextureB},
	});

	m_AdditiveColorBufferTexture = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);
	m_AdditiveFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0, m_AdditiveColorBufferTexture},
	});

	m_ToneMappingColorBufferTexture = spr::createTexture(hdrRenderTargetTextureInfo, nullptr);
	m_ToneMappingFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0, m_ToneMappingColorBufferTexture},
	});

	// TODO: Move to RenderTarget clear config
	glClearColor(0.04f, 0.04f, 0.04f, 1.0f);
}

void SPRE_05_Bloom::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	// 1. Sets up render targets
	const auto &[windowWidth, windowHeight] = spw::getWindowSize();

	const uint8_t defaultPassTarget = 0;
	spr::setRenderTargetFramebuffer(defaultPassTarget, m_HDRFramebuffer);
	spr::setRenderTargetClear(defaultPassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(defaultPassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});

	const uint8_t brightnessFilterPassTarget = 1;
	spr::setRenderTargetFramebuffer(brightnessFilterPassTarget, m_BrightnessFilterFramebuffer);
	spr::setRenderTargetClear(brightnessFilterPassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(brightnessFilterPassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});
	
	const uint8_t gaussianBlurPassTarget = 2;
	spr::setRenderTargetFramebuffer(gaussianBlurPassTarget, m_GaussianBlurFramebuffer);
	spr::setRenderTargetClear(gaussianBlurPassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(gaussianBlurPassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});
	
	const uint8_t additivePassTarget = 3;
	spr::setRenderTargetFramebuffer(additivePassTarget, m_AdditiveFramebuffer);
	spr::setRenderTargetClear(additivePassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(additivePassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});
	
	const uint8_t toneMappingPassTarget = 4;
	spr::setRenderTargetFramebuffer(toneMappingPassTarget, m_ToneMappingFramebuffer);
	spr::setRenderTargetClear(toneMappingPassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(toneMappingPassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});

	// 2. Render Pass
	{
		// Draws the light source
		const float currentTimeScaled = Runtime::get()->getTime() * 0.8f;
		const glm::vec3 lightSourcePos = glm::vec3(std::cos(currentTimeScaled) * 1.5f,
		                                           3.f + (std::cos(currentTimeScaled) * std::sin(currentTimeScaled)),
		                                           std::sin(currentTimeScaled) * 1.5f);

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, lightSourcePos);
		sourceModel = glm::rotate(sourceModel, 0.f, glm::vec3(0.0f, 0.0f, 1.0f));
		sourceModel = glm::scale(sourceModel, glm::vec3(0.6f, 0.6f, 0.6f));

		const glm::vec3 lightColorRGB = m_BaseLightColor * m_HDRIntensity;
		const glm::vec4 lightColorRGBA { lightColorRGB.r, lightColorRGB.g, lightColorRGB.b, 1.f };
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ColorUniform, glm::value_ptr(lightColorRGBA));
		spr::submit(defaultPassTarget, m_UnlitShaderProgram);

		// Draws circle of cubes
		auto drawCube = [this, defaultPassTarget](const glm::vec3 &position, const glm::vec3& scale, const glm::vec3 &lightPosition, const glm::vec3 &lightColorRGB)
		{
			const int albedoTextureUnit = 0;
			const float ambientIntensity = 0.05f;
			const float specularShininess = 2.0f;
			const float specularStrength = 1.f;
			const float textureTiling = 1.f;

			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, 0.f, {0.f, 1.f, 0.f});
			model = glm::scale(model, scale);

			spr::SamplerInfo brickSampler;
			brickSampler.WrappingVertical = spr::WrappingMethod::Repeat;
			brickSampler.WrappingHorizontal = spr::WrappingMethod::Repeat;
			brickSampler.MinificationFilter = spr::FilterMethod::Linear;
			spr::setVertexBuffer(m_CubeVertexBuffer);
			spr::setTexture(albedoTextureUnit, m_AlbedoTexture, brickSampler);

			spr::setUniform(m_ModelUniform, glm::value_ptr(model));
			spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
			spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
			spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
			spr::setUniform(m_LightColorUniform, glm::value_ptr(lightColorRGB));
			spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightPosition));
			spr::setUniform(m_AlbedoUniform, &albedoTextureUnit);
			spr::setUniform(m_TilingUniform, &textureTiling);
			spr::setUniform(m_AmbientIntensityUniform, &ambientIntensity);
			spr::setUniform(m_SpecularShininessUniform, &specularShininess);
			spr::setUniform(m_SpecularStrengthUniform, &specularStrength);
			spr::submit(defaultPassTarget, m_LitShaderProgram);
		};
		const int cubeCount = 8;
		for (int i = 0; i < cubeCount; i++) {

			const float progress = static_cast<float>(i) / static_cast<float>(cubeCount);
			const float angleRad = glm::radians(progress * 360.f);

			const float circleRadius = 3.f;
			const glm::vec3 position{
			    glm::cos(angleRad) * circleRadius,
			    3.f,
			    glm::sin(angleRad) * circleRadius};
			const glm::vec3 scale{ 1.f };

			drawCube(position, scale, lightSourcePos, lightColorRGB);
		}

		// Draw floor
		const glm::vec3 position{0.f, 0.f, 0.f};
		const glm::vec3 scale{ 50.f, 0.01f, 50.f };
		drawCube(position, scale, lightSourcePos, lightColorRGB);
	}

	// 3. Brightness filter pass
	{
		const int renderPassTextureUnit = 0;
		spr::setTexture(renderPassTextureUnit, m_HDRColorBufferTexture);
		spr::setUniform(m_RenderTargetTextureUniform, &renderPassTextureUnit);

		const glm::mat4 identity(1.0f);
		const float brightnessThreshold = 1.f;
		const auto &quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(identity));
		spr::setUniform(m_ViewUniform, glm::value_ptr(identity));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(identity));
		spr::setUniform(m_BrightnessFilterThresholdUniform, &brightnessThreshold);
		spr::submit(brightnessFilterPassTarget, m_BrightnessFilterShaderProgram);
	}

	// 4. Gaussian Blur pass
	{
		const int gaussianBlurPassCount = 10;
		for (int i = 0; i < gaussianBlurPassCount; i++) {
			const int primaryTextureUnit = 0;
			spr::TextureHandle primaryTexture = i == 0 ? m_BrightnessFilterBloomColorTexture : m_GaussianBlurColorBufferTextureA;
			spr::setTexture(primaryTextureUnit, primaryTexture);
			spr::setUniform(m_GaussianBlurPrimaryTextureUniform, &primaryTextureUnit);
			
			const int secondaryTextureUnit = 1;
			spr::setTexture(secondaryTextureUnit, m_GaussianBlurColorBufferTextureB);
			spr::setUniform(m_GaussianBlurSecondaryTextureUniform, &secondaryTextureUnit);

			const int bIsHorizontal = i % 2;
			const auto &quadMesh = m_QuadModel.Meshes[0];
			spr::setVertexBuffer(quadMesh.VertexBuffer);
			spr::setIndexBuffer(quadMesh.IndexBuffer);
			spr::setUniform(m_GaussianBlurHorizontalUniform, &bIsHorizontal);
			spr::submit(gaussianBlurPassTarget, m_GaussianBlurShaderProgram);		
		}
	}

	// 5. Additive pass
	{
		const int additiveTextureUnitA = 0;
		spr::setTexture(additiveTextureUnitA, m_BrightnessFilterColorTexture);
		spr::setUniform(m_AdditiveTextureUniformA, &additiveTextureUnitA);
		
		const int additiveTextureUnitB = 1;
		spr::setTexture(additiveTextureUnitB, m_GaussianBlurColorBufferTextureA);
		spr::setUniform(m_AdditiveTextureUniformB, &additiveTextureUnitB);

		const auto &quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);
		spr::submit(additivePassTarget, m_AdditiveShaderProgram);
	}

	// 6. Tone Mapping pass
	{
		const int tonemappingType = 2;			// exposure tone mapping
		const int renderPassTextureUnit = 0;
		const float gammaCorrectionRatio = 2.2f;
		spr::setTexture(renderPassTextureUnit, m_AdditiveColorBufferTexture);
		spr::setUniform(m_RenderTargetTextureUniform, &renderPassTextureUnit);
		spr::setUniform(m_ToneMappingTypeUniform, &tonemappingType);
		spr::setUniform(m_GammaCorrectionRatioUniform, &gammaCorrectionRatio);
		spr::setUniform(m_ToneMappingExposureUniform, &m_Exposure);

		const auto &quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);
		spr::submit(toneMappingPassTarget, m_ToneMappingShaderProgram);
	}

	// 7. Blit + Double buffering flip:
	{
		const spr::FramebufferHandle defaultFramebuffer = spr::k_InvalidHandle;
		const auto &[windowWidth, windowHeight] = spw::getWindowSize();

		spr::BlitParameters blitParameters;
		blitParameters.Source = m_ToneMappingFramebuffer;
		blitParameters.Destination = defaultFramebuffer;
		blitParameters.SourceRect = {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight};
		blitParameters.DestinationRect = {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight};
		blitParameters.BlitMask |= spr::AsFlag(spr::FramebufferAttachmentFlags::Color);
		spr::blit(blitParameters);
	}

	spr::flush();
}

void SPRE_05_Bloom::OnEvent(const Event &evt) {
	m_Controller.HandleEvent(evt);
}

void SPRE_05_Bloom::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

	ImGui::Begin("05 - Bloom");

	if (ImGui::TreeNode("Lighting")) {
		ImGui::ColorPicker3("Light Color", glm::value_ptr(m_BaseLightColor), ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel);
		ImGui::InputFloat("HDR Intensity", &m_HDRIntensity);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Tone mapping")) {
		ImGui::InputFloat("Exposure", &m_Exposure);
		ImGui::TreePop();
	}

	ImGui::End();
}

void SPRE_05_Bloom::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_AlbedoTexture);
	spr::destroy(m_LitShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_AlbedoUniform);
	spr::destroy(m_TilingUniform);
	spr::destroy(m_AmbientIntensityUniform);
	spr::destroy(m_SpecularShininessUniform);
	spr::destroy(m_SpecularStrengthUniform);
	spr::destroy(m_UnlitShaderProgram);
	spr::destroy(m_ColorUniform);
	spr::destroy(m_HDRFramebuffer);
	spr::destroy(m_HDRColorBufferTexture);
	spr::destroy(m_HDRStencilDepthBufferTexture);
	spr::destroy(m_BrightnessFilterShaderProgram);
	spr::destroy(m_BrightnessFilterThresholdUniform);
	spr::destroy(m_RenderTargetTextureUniform);
	spr::destroy(m_BrightnessFilterFramebuffer);
	spr::destroy(m_BrightnessFilterColorTexture);
	spr::destroy(m_BrightnessFilterBloomColorTexture);
	spr::destroy(m_GaussianBlurShaderProgram);
	spr::destroy(m_GaussianBlurHorizontalUniform);
	spr::destroy(m_GaussianBlurPrimaryTextureUniform);
	spr::destroy(m_GaussianBlurSecondaryTextureUniform);
	spr::destroy(m_GaussianBlurFramebuffer);
	spr::destroy(m_GaussianBlurColorBufferTextureA);
	spr::destroy(m_GaussianBlurColorBufferTextureB);
	spr::destroy(m_AdditiveShaderProgram);
	spr::destroy(m_AdditiveTextureUniformA);
	spr::destroy(m_AdditiveTextureUniformB);
	spr::destroy(m_AdditiveFramebuffer);
	spr::destroy(m_AdditiveColorBufferTexture);
	spr::destroy(m_ToneMappingShaderProgram);
	spr::destroy(m_ToneMappingTypeUniform);
	spr::destroy(m_ToneMappingExposureUniform);
	spr::destroy(m_GammaCorrectionRatioUniform);
	spr::destroy(m_ToneMappingFramebuffer);
	spr::destroy(m_ToneMappingColorBufferTexture);
}
