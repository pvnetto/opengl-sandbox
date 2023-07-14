#pragma once

#include "shared/FreeCameraController.h"
#include "shared/Mesh.h"
#include "shared/Shader.h"
#include "shared/layers/Layer.h"

struct LOGL_12_PointType {
    glm::vec3 Color = glm::vec3{1.f, 0.8f, 0.6f};
	glm::vec3 Position = glm::vec3{0.f, 0.f, 0.f};

	float ConstantAttenuation = 1.f;
	float LinearAttenuation = 0.09f;
	float QuadraticAttenuation = 0.032f;
};

class LOGL_12_PointLight : public Layer {
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

    LOGL_12_PointType m_light;

	bool m_static; 
};