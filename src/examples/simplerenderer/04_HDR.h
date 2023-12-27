#pragma once

#include "shared/Camera.h"
#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"
#include "shared/RenderUtils.h"
#include <spr/SimpleRenderer.h>

class SPRE_04_HDR : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnEvent(const Event &evt) override;
	virtual void OnImGuiRender() override;

private:
	Camera m_Camera;
	FreeCameraController m_Controller;

	glm::vec3 m_BaseLightColor;

private:
	Utils::ModelInstance m_QuadModel;

	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::FramebufferHandle m_HDRFramebuffer;
	spr::TextureHandle m_HDRColorBufferTexture;
	spr::TextureHandle m_HDRStencilDepthBufferTexture;

	spr::FramebufferHandle m_ToneMappingFramebuffer;
	spr::TextureHandle m_ToneMappingColorBufferTexture;

	spr::ProgramHandle m_LitShaderProgram;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_ViewPositionUniform;
	spr::UniformHandle m_LightColorUniform;
	spr::UniformHandle m_LightPositionUniform;
	spr::UniformHandle m_AlbedoUniform;
	spr::UniformHandle m_TilingUniform;
	spr::UniformHandle m_AmbientIntensityUniform;
	spr::UniformHandle m_SpecularShininessUniform;
	spr::UniformHandle m_SpecularStrengthUniform;

	spr::ProgramHandle m_UnlitShaderProgram;
	spr::UniformHandle m_ColorUniform;

	spr::ProgramHandle m_ToneMappingShaderProgram;
	spr::UniformHandle m_ToneMappingTextureUniform;
	spr::UniformHandle m_ToneMappingTypeUniform;
	spr::UniformHandle m_ToneMappingExposureUniform;
	spr::UniformHandle m_GammaCorrectionRatioUniform;

	spr::TextureHandle m_AlbedoTexture;

private:
	// 0 = disabled
	// 1 = reinhard
	// 2 = exposure
	int m_TonemappingType = 1;
	float m_Exposure = 0.1f;

	float m_HDRIntensity = 100.f;
	bool m_bSpecularEnabled = true;
	int m_SelectedShininess = 1;

	// NOTE: We're applying gamma correction on the fragment shader, but we could instead use sRGB textures and framebuffers
	bool m_bEnableGammaCorrection = true;
};
