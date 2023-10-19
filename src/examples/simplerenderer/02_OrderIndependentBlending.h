#pragma once

#include "shared/Camera.h"
#include "shared/FreeCameraController.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class SPRE_02_OrderIndependentBlending : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	Camera m_Camera;

	void SubmitOpaque();
	void SubmitTransparent();

private:
	Utils::ModelInstance m_QuadModel;

	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_UnlitShaderProgram;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_ColorUniform;

};
