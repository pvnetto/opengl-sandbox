#pragma once

#include "shared/FreeCameraController.h"
#include "shared/Mesh.h"
#include "shared/Shader.h"
#include "shared/layers/Layer.h"

struct LOGL_10_PhongMat {
    glm::vec3 Color = glm::vec3(0.2f, 0.35f, 1.f);
    glm::vec3 SpecularColor = glm::vec3(0.5f, 0.5f, 0.5f);
    
    float AmbientStrength = 0.1f;
    float DiffuseStrength = 0.8f;
    float SpecularStrength = 1.f;
    float Shininess = 32.f;
};

class LOGL_10_Material : public Layer {
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

	glm::vec3 m_lightColor;

    LOGL_10_PhongMat m_material;
};
