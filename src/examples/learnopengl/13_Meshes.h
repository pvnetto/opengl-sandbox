#pragma once

#include "shared/Model.h"
#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"


class LOGL_13_Meshes : public Layer {
public:
	LOGL_13_Meshes();
	~LOGL_13_Meshes();

	virtual void OnAttach() override;
    virtual void OnUpdate() override;
	virtual void OnEvent(Event& evt) override;

	Camera& GetCamera() { return m_camera; }

protected:
	Camera m_camera;
	FreeCameraController m_controller;

    Mesh m_cubeMesh;
    Model m_backpackModel;

    Shader m_phongShader, m_lightSourceShader;
};