#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class AOGL_03_Antialiasing : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

private:
	void CreateMSAAFramebuffer();
	void InvalidateMSAAFramebuffer();

private:
	void DrawScene();
	void CreateCube();

private:
	Utils::PrimitiveData m_CubeData;

	unsigned int m_CubeVertexArray;
	unsigned int m_CubeVertexBuffer;

	unsigned int m_DefaultShaderProgram;

	unsigned int m_MSAAFramebuffer;
	unsigned int m_MSAAColorTexture;
	unsigned int m_MSAADepthStencilTexture;

private:
	Camera m_Camera;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;

	bool m_IsMultisamplingEnabled = true;
	int m_SampleCountOption = 2;					// Number of Samples = 2 ^ (Sample Count Option + 1)
	int m_LastSampleCountOption; 

};
