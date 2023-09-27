#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class AOGL_06_Tessellation : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	void CreateShaderProgram();
	void CompileShaderAndAttach(const char *shaderPath, int shaderType);

private:
	Utils::PrimitiveData m_QuadData;
	unsigned int m_TessellatedShaderProgram;

	unsigned int m_QuadVertexBuffer;
	unsigned int m_QuadIndexBuffer;
	unsigned int m_VertexArray;

private:
	Camera m_Camera;
	bool m_bIsTessellationEnabled = true;

};
