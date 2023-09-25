#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class AOGL_05_GeometryShading : public Layer {
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

private:
	Camera m_Camera;
};
