#pragma once

#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class AOGL_01_Framebuffers : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	Utils::ModelInstance m_QuadModel;

	spr::VertexBufferHandle m_CubeVertexBuffer;
	spr::TextureHandle m_BrickTexture;
	spr::TextureHandle m_LadybugTexture;

	spr::ProgramHandle m_DefaultShaderProgram;
	spr::UniformHandle m_TexUniform;
	spr::UniformHandle m_AnotherTexUniform;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

	spr::ProgramHandle m_PostProcessShaderProgram;
	spr::UniformHandle m_RenderTargetTextureUniform;

private:
	unsigned int m_Framebuffer;
	unsigned int m_ColorBufferTexture;
	unsigned int m_DepthBufferTexture;

    glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_CameraPosition;

	float m_FieldOfView;

};
