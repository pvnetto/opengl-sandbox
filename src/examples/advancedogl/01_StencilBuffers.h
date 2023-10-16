#pragma once

#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>
#include "shared/Camera.h"
#include "shared/FreeCameraController.h"

class AOGL_01_StencilBuffers : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	Camera m_Camera;

	glm::vec3 m_ObjectColor;
	glm::vec3 m_LightColor;

private:
	Utils::PrimitiveData m_QuadData;

	unsigned int m_QuadVertexArray;
	unsigned int m_QuadVertexBuffer;
	unsigned int m_QuadIndexBuffer;

	Utils::PrimitiveData m_CubeData;

	unsigned int m_CubeVertexArray;
	unsigned int m_CubeVertexBuffer;

	unsigned int m_LightingShaderProgram;
	unsigned int m_LightSourceShaderProgram;
	unsigned int m_StencilPassShaderProgram;

	unsigned int m_Framebuffer;
	unsigned int m_ColorBufferTexture;
	unsigned int m_StencilDepthBufferTexture;
};
