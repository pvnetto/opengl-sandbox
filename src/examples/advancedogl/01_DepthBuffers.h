#pragma once

#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"

#include <spr/SimpleRenderer.h>

#include <glm/glm.hpp>

class AOGL_01_DepthBuffers : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	void CreateShaderProgram();

private:
	Utils::PrimitiveData m_QuadData;

	unsigned int m_VertexArray;
	unsigned int m_QuadVertexBuffer;
	unsigned int m_QuadIndexBuffer;

	unsigned int m_DepthDrawShaderProgram;

	unsigned int m_Framebuffer;
	unsigned int m_ColorBufferTexture;
	unsigned int m_DepthBufferTexture;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_CameraPosition;

	float m_FieldOfView;
};
