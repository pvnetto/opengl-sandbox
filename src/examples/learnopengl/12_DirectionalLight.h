#pragma once

#include "shared/FreeCameraController.h"
#include "shared/Mesh.h"
#include "shared/Shader.h"
#include "shared/layers/Layer.h"

struct LOGL_12_DirectionalType {
    glm::vec3 Color = glm::vec3{ 1.f, 0.8f, 0.6f };
    glm::vec3 Direction = glm::vec3{0.f, -1.0f, 0.f};
};

class LOGL_12_DirectionalLight : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event &evt) override;

private:
	Camera m_camera;
	FreeCameraController m_controller;

	Mesh m_cubeMesh;
	Shader m_phongShader;

    LOGL_12_DirectionalType m_light;
};