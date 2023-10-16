#pragma once

#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"

#include <spr/SimpleRenderer.h>

#include <glm/glm.hpp>

class AOGL_01_Framebuffers : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	void DrawScene();

	void CreateQuad();
	void CreateCube();

private:
	Utils::PrimitiveData m_QuadData;

	unsigned int m_QuadVertexArray;
	unsigned int m_QuadVertexBuffer;
	unsigned int m_QuadIndexBuffer;

	Utils::PrimitiveData m_CubeData;

	unsigned int m_CubeVertexArray;
	unsigned int m_CubeVertexBuffer;

	unsigned int m_BrickTexture;
	unsigned int m_LadybugTexture;

	unsigned int m_DefaultShaderProgram;
	unsigned int m_PostProcessShaderProgram;

	unsigned int m_Framebuffer;
	unsigned int m_ColorBufferTexture;
	unsigned int m_DepthBufferTexture;

private:
    glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_CameraPosition;

	float m_FieldOfView;

};
