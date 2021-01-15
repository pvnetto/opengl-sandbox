#pragma once

#include "shared/FreeCameraController.h"
#include "shared/Mesh.h"
#include "shared/Shader.h"
#include "shared/layers/Layer.h"

class LOGL_11_SpecularMap : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event &evt) override;

private:
	Camera m_camera;
	FreeCameraController m_controller;

	Mesh m_cubeMesh;
	Mesh m_lightSourceMesh;

	Shader m_phongShader;
	Shader m_lightSourceShader;
};