#include "04_HDR.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

void SPRE_04_HDR::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, -10.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 15.f);

	// Sets up HDR colors
	m_BaseLightColor = { 1.f, 0.1f, 0.6f };

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

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
	m_ToneMappingTextureUniform = spr::createUniform("renderTargetTexture", spr::UniformType::Sampler);
	m_ToneMappingTypeUniform = spr::createUniform("tonemappingType", spr::UniformType::Integer);
	m_ToneMappingExposureUniform = spr::createUniform("exposure", spr::UniformType::Integer);
	m_GammaCorrectionRatioUniform = spr::createUniform("gammaCorrectionRatio", spr::UniformType::Integer);
	m_AlbedoTexture = Utils::LoadTexture("assets/bricks.jpg");

	m_LitShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/default_lit.frag");
	m_UnlitShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/default_unlit.frag");
	m_ToneMappingShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/postprocess_tonemapping.frag");

	const auto &[windowWidth, windowHeight] = spw::getWindowSize();
	spr::TextureInfo hdrColorTextureInfo;
	hdrColorTextureInfo.Width = std::round(windowWidth);
	hdrColorTextureInfo.Height = std::round(windowHeight);
	hdrColorTextureInfo.Format = spr::TextureFormat::RGBA16F;
	hdrColorTextureInfo.Flags |= (uint8_t)spr::TextureFlags::IsRenderTargetTexture;
	m_HDRColorBufferTexture = spr::createTexture(hdrColorTextureInfo, nullptr);

	spr::TextureInfo stencilDepthTextureInfo;
	stencilDepthTextureInfo.Width = windowWidth;
	stencilDepthTextureInfo.Height = windowHeight;
	stencilDepthTextureInfo.Format = spr::TextureFormat::DEPTH_24_STENCIL_8;
	stencilDepthTextureInfo.Flags |= (uint8_t)spr::TextureFlags::IsRenderTargetTexture;
	m_HDRStencilDepthBufferTexture = spr::createTexture(stencilDepthTextureInfo, nullptr);
	m_HDRFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0,		m_HDRColorBufferTexture},
	    {spr::FramebufferAttachmentType::DepthStencil,	m_HDRStencilDepthBufferTexture},
	});

	spr::TextureInfo postProcessColorTextureInfo;
	postProcessColorTextureInfo.Width = std::round(windowWidth);
	postProcessColorTextureInfo.Height = std::round(windowHeight);
	postProcessColorTextureInfo.Format = spr::TextureFormat::RGBA16F;
	postProcessColorTextureInfo.Flags |= (uint8_t)spr::TextureFlags::IsRenderTargetTexture;
	m_ToneMappingColorBufferTexture = spr::createTexture(postProcessColorTextureInfo, nullptr);
	m_ToneMappingFramebuffer = spr::createFramebuffer({
	    {spr::FramebufferAttachmentType::Color0, m_ToneMappingColorBufferTexture},
	});

	// TODO: Move to RenderTarget clear config
	glClearColor(0.04f, 0.04f, 0.04f, 1.0f);
}

void SPRE_04_HDR::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	// 1. Sets up render targets
	const auto &[windowWidth, windowHeight] = spw::getWindowSize();

	const uint8_t hdrRenderPassTarget = 0;
	spr::setRenderTargetFramebuffer(hdrRenderPassTarget, m_HDRFramebuffer);
	spr::setRenderTargetClear(hdrRenderPassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(hdrRenderPassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});

	const uint8_t postProcessRenderPassTarget = 1;
	spr::setRenderTargetFramebuffer(postProcessRenderPassTarget, m_ToneMappingFramebuffer);
	spr::setRenderTargetClear(postProcessRenderPassTarget, spr::AsFlag(spr::FramebufferAttachmentFlags::All));
	spr::setRenderTargetRect(postProcessRenderPassTarget, {0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight});

	// 2. Render Pass
	{
		// Draws the light source
		const float tunnelDepth = 50.f;
		const float currentTime = Runtime::get()->getTime();
		//const glm::vec3 lightSourcePos = glm::vec3(0.f, 0.f, 2.f - (std::cos(currentTime) + 1.f) * 0.4f * tunnelDepth);
		const glm::vec3 lightSourcePos = glm::vec3(0.f, 0.f, 2.f - 0.85f * tunnelDepth);

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, lightSourcePos);
		sourceModel = glm::rotate(sourceModel, 0.f, glm::vec3(0.0f, 0.0f, 1.0f));
		sourceModel = glm::scale(sourceModel, glm::vec3(0.1f, 0.1f, 0.1f));

		const glm::vec4 lightSourceColor{m_BaseLightColor.r, m_BaseLightColor.g, m_BaseLightColor.b, 1.f};
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ColorUniform, glm::value_ptr(lightSourceColor));
		spr::submit(hdrRenderPassTarget, m_UnlitShaderProgram);


		// Draws 4 walls, forming a tunnel
		const float tunnelSize = 3.f;
		const int albedoTextureUnit = 0;
		const float ambientIntensity = 0.01f;
		const float specularShininess = std::pow(2.0f, (float) m_SelectedShininess);
		const float specularStrength = m_bSpecularEnabled ? 1.f : 0.f;
		const glm::vec3 lightColor = m_BaseLightColor * m_HDRIntensity;
		for (int i = 0; i < 4; i++) {
			const glm::vec2 offsets[4] =
			{
				{  1.f,  0.f },
				{ -1.f,  0.f },
				{  0.f,  1.f },
				{  0.f, -1.f }
			};

			const glm::vec2 offset = offsets[i];
			const glm::vec3 position { offset.x * tunnelSize, offset.y * tunnelSize, -tunnelDepth * 0.4f };

			const float horizontalScale = offset.x == 0.f ? tunnelDepth : 1.f;
			const float verticalScale = offset.y == 0.f ? tunnelDepth : 1.f;
			const glm::vec3 scale{horizontalScale, verticalScale, tunnelDepth};

			glm::mat4 litModel(1.0f);
			litModel = glm::translate(litModel, position);
			litModel = glm::rotate(litModel, 0.f, glm::vec3(0.0f, 0.0f, 1.0f));
			litModel = glm::scale(litModel, scale);

			const float textureTiling = tunnelDepth * 0.2f;

			spr::SamplerInfo brickSampler;
			brickSampler.WrappingVertical = spr::WrappingMethod::Repeat;
			brickSampler.WrappingHorizontal = spr::WrappingMethod::Repeat;
			brickSampler.MinificationFilter = spr::FilterMethod::Linear;
			spr::setVertexBuffer(m_CubeVertexBuffer);
			spr::setTexture(albedoTextureUnit, m_AlbedoTexture, brickSampler);
			spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
			spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
			spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
			spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
			spr::setUniform(m_LightColorUniform, glm::value_ptr(lightColor));
			spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
			spr::setUniform(m_AlbedoUniform, &albedoTextureUnit);
			spr::setUniform(m_TilingUniform, &textureTiling);
			spr::setUniform(m_AmbientIntensityUniform, &ambientIntensity);
			spr::setUniform(m_SpecularShininessUniform, &specularShininess);
			spr::setUniform(m_SpecularStrengthUniform, &specularStrength);
			spr::submit(hdrRenderPassTarget, m_LitShaderProgram);
		}


		// Draws wall at the end of the tunnel
		const float wallTiling = tunnelSize * 0.5f;

		const glm::vec3 position{ 0.f, 0.f, -tunnelDepth * 0.9f };
		const glm::vec3 scale{ tunnelSize * 2.f, tunnelSize * 2.f, 0.3f };

		glm::mat4 litModel(1.0f);
		litModel = glm::translate(litModel, position);
		litModel = glm::rotate(litModel, 0.f, glm::vec3(0.0f, 0.0f, 1.0f));
		litModel = glm::scale(litModel, scale);

		spr::SamplerInfo brickSampler;
		brickSampler.WrappingVertical = spr::WrappingMethod::Repeat;
		brickSampler.WrappingHorizontal = spr::WrappingMethod::Repeat;
		brickSampler.MinificationFilter = spr::FilterMethod::Linear;
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setTexture(albedoTextureUnit, m_AlbedoTexture, brickSampler);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(lightColor));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
		spr::setUniform(m_AlbedoUniform, &albedoTextureUnit);
		spr::setUniform(m_TilingUniform, &wallTiling);
		spr::setUniform(m_AmbientIntensityUniform, &ambientIntensity);
		spr::setUniform(m_SpecularShininessUniform, &specularShininess);
		spr::setUniform(m_SpecularStrengthUniform, &specularStrength);
		spr::submit(hdrRenderPassTarget, m_LitShaderProgram);
	}

	// 3. Tone Mapping pass
	{
		const int tonemappingTextureUnit = 0;
		const float gammaCorrectionRatio = m_bEnableGammaCorrection ? 2.2f : 1.f;
		spr::setTexture(tonemappingTextureUnit, m_HDRColorBufferTexture);
		spr::setUniform(m_ToneMappingTextureUniform, &tonemappingTextureUnit);
		spr::setUniform(m_ToneMappingTypeUniform, &m_TonemappingType);
		spr::setUniform(m_ToneMappingExposureUniform, &m_Exposure);
		spr::setUniform(m_GammaCorrectionRatioUniform, &gammaCorrectionRatio);

		const glm::mat4 identity(1.0f);
		const auto& quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(identity));
		spr::setUniform(m_ViewUniform, glm::value_ptr(identity));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(identity));
		spr::submit(postProcessRenderPassTarget, m_ToneMappingShaderProgram);
	}

	// 4. Blit + Flip:
	{
		const spr::FramebufferHandle defaultFramebuffer = spr::k_InvalidHandle;
		const auto& [windowWidth, windowHeight] = spw::getWindowSize();

		spr::BlitParameters blitParameters;
		blitParameters.Source = m_ToneMappingFramebuffer;
		blitParameters.Destination = defaultFramebuffer;
		blitParameters.SourceRect = { 0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight };
		blitParameters.DestinationRect = { 0, 0, (uint32_t)windowWidth, (uint32_t)windowHeight };
		blitParameters.BlitMask |= spr::AsFlag(spr::FramebufferAttachmentFlags::Color);
		spr::blit(blitParameters);
	}

	spr::flush();
}

void SPRE_04_HDR::OnEvent(const Event& evt) {
	m_Controller.HandleEvent(evt);
}

void SPRE_04_HDR::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

	ImGui::Begin("04 - HDR");

	if (ImGui::TreeNode("Lighting")) {
		ImGui::ColorPicker3("Light Color", glm::value_ptr(m_BaseLightColor), ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Checkbox("Specular Enabled", &m_bSpecularEnabled);

		if (m_bSpecularEnabled) {
			static const char *shininessValues[] = {"1.0", "2.0", "4.0", "8.0", "16.0", "32.0", "64.0", "128.0", "256.0"};
			ImGui::ListBox("Specular Shininess", &m_SelectedShininess, shininessValues, IM_ARRAYSIZE(shininessValues), 4);		
		}

		ImGui::InputFloat("HDR Intensity", &m_HDRIntensity);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Tone mapping")) {
		ImGui::Combo("Tone mapping algorithm", &m_TonemappingType,
					 "None\0"
					 "Reinhard\0"
					 "Exposure\0");
		if (m_TonemappingType == 2) {
			ImGui::InputFloat("Exposure", &m_Exposure);
		}
		ImGui::Checkbox("Enable Gamma Correction", &m_bEnableGammaCorrection);
		ImGui::TreePop();
	}

	ImGui::End();
}

void SPRE_04_HDR::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_HDRFramebuffer);
	spr::destroy(m_HDRColorBufferTexture);
	spr::destroy(m_HDRStencilDepthBufferTexture);
	spr::destroy(m_ToneMappingFramebuffer);
	spr::destroy(m_ToneMappingColorBufferTexture);
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
	spr::destroy(m_ToneMappingShaderProgram);
	spr::destroy(m_ToneMappingTextureUniform);
	spr::destroy(m_ToneMappingTypeUniform);
	spr::destroy(m_ToneMappingExposureUniform);
	spr::destroy(m_GammaCorrectionRatioUniform);
	spr::destroy(m_AlbedoTexture);
}
