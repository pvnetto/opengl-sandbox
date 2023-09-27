#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class AOGL_04_Instancing : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	void CreateShaderProgram();

private:
	Utils::PrimitiveData m_CubeData;
	unsigned int m_ShaderProgram;

	unsigned int m_VertexArray;
	unsigned int m_MeshVertexBuffer;
	unsigned int m_PerInstanceColorVertexBuffer;
	unsigned int m_PerInstanceModelVertexBuffer;

private:
	Camera m_Camera;

	int m_CubeCount = 256;
};
