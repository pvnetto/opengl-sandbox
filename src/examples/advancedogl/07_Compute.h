#pragma once

#include "shared/layers/Layer.h"
#include "shared/RenderUtils.h"

#include <glm/glm.hpp>
#include <spr/SimpleRenderer.h>

class AOGL_07_Compute : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnUpdate() override;

private:
	glm::vec2 m_OutputSize;

private:
	Utils::PrimitiveData m_QuadData;

	unsigned int m_VertexArrayObjectID;
	unsigned int m_QuadVertexBufferID, m_QuadIndexBufferID;

	unsigned int m_OutputTextureID;

	unsigned int m_ComputeShaderProgram;
	unsigned int m_TexturedQuadShaderProgram;

	void CreateBufferObjects();
	void CreateTexturedQuadShaderProgram();
	unsigned int CreateShaderObject(unsigned int shaderType, const std::string& shaderSource) const;
};
