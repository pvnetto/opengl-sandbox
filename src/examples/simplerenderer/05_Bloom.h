#pragma once

#include "shared/Camera.h"
#include "shared/FreeCameraController.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class SPRE_05_Bloom : public Layer {
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
	/* Resources */
	Utils::ModelInstance m_QuadModel;
	spr::VertexBufferHandle m_CubeVertexBuffer;
	spr::TextureHandle m_AlbedoTexture;

	/* Lit Shader */
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

	/* Unlit Shader */
	spr::ProgramHandle m_UnlitShaderProgram;
	spr::UniformHandle m_ColorUniform;

	/* HDR Pass */
	spr::FramebufferHandle m_HDRFramebuffer;
	spr::TextureHandle m_HDRColorBufferTexture;
	spr::TextureHandle m_HDRStencilDepthBufferTexture;

	/* Brightness Filter Pass */
	spr::ProgramHandle m_BrightnessFilterShaderProgram;
	spr::UniformHandle m_BrightnessFilterThresholdUniform;
	spr::UniformHandle m_RenderTargetTextureUniform;

	spr::FramebufferHandle m_BrightnessFilterFramebuffer;
	spr::TextureHandle m_BrightnessFilterColorTexture;
	spr::TextureHandle m_BrightnessFilterBloomColorTexture;

	/* Gaussian Blur Pass */
	spr::ProgramHandle m_GaussianBlurShaderProgram;
	spr::UniformHandle m_GaussianBlurHorizontalUniform;
	spr::UniformHandle m_GaussianBlurPrimaryTextureUniform;
	spr::UniformHandle m_GaussianBlurSecondaryTextureUniform;

	spr::FramebufferHandle m_GaussianBlurFramebuffer;
	spr::TextureHandle m_GaussianBlurColorBufferTextureA;
	spr::TextureHandle m_GaussianBlurColorBufferTextureB;

	/* Additive Pass */
	spr::ProgramHandle m_AdditiveShaderProgram;
	spr::UniformHandle m_AdditiveTextureUniformA;
	spr::UniformHandle m_AdditiveTextureUniformB;

	spr::FramebufferHandle m_AdditiveFramebuffer;
	spr::TextureHandle m_AdditiveColorBufferTexture;

	/* Tone Mapping Pass */
	spr::ProgramHandle m_ToneMappingShaderProgram;
	spr::UniformHandle m_ToneMappingTypeUniform;
	spr::UniformHandle m_ToneMappingExposureUniform;
	spr::UniformHandle m_GammaCorrectionRatioUniform;

	spr::FramebufferHandle m_ToneMappingFramebuffer;
	spr::TextureHandle m_ToneMappingColorBufferTexture;

private:
	float m_HDRIntensity = 5.f;
	float m_Exposure = 0.5f;
};
