#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class SPRE_03_Instancing : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;
	spr::VertexBufferHandle m_InstancedColorBuffer;
	spr::VertexBufferHandle m_InstancedModelBuffer;

	spr::ProgramHandle m_InstancedShaderProgram;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

private:
	Camera m_Camera;

	int m_CubeCount = 256;
};
