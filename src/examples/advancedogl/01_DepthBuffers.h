#pragma once

#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class AOGL_01_DepthBuffers : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	Utils::ModelInstance m_QuadModel;

	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_DepthDrawShaderProgram;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

	spr::ProgramHandle m_RenderPassShaderProgram;
	spr::UniformHandle m_RenderTargetTextureUniform;
	spr::UniformHandle m_NearPlaneUniform;
	spr::UniformHandle m_FarPlaneUniform;

private:
	unsigned int m_Framebuffer;
	unsigned int m_ColorBufferTexture;
	unsigned int m_DepthBufferTexture;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_CamerinPosition;

	float m_FieldOfView;
};
