#pragma once

#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>
#include "shared/Camera.h"
#include "shared/FreeCameraController.h"

class SPRE_01_StencilWithRenderer : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	Camera m_Camera;

	glm::vec3 m_ObjectColor;
	glm::vec3 m_LightColor;

private:
	Utils::ModelInstance m_QuadModel;

	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::FramebufferHandle m_Framebuffer;
	spr::TextureHandle m_ColorBufferTexture;
	spr::TextureHandle m_StencilDepthBufferTexture;

	spr::ProgramHandle m_StencilPassShaderProgram;
	spr::UniformHandle m_ResolutionUniform;
	spr::UniformHandle m_TimeUniform;


	spr::ProgramHandle m_LightingShaderProgram;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_ViewPositionUniform;
	spr::UniformHandle m_LightColorUniform;
	spr::UniformHandle m_LightPositionUniform;
	spr::UniformHandle m_ObjectColorUniform;

	spr::ProgramHandle m_LightSourceShaderProgram;

};
