#pragma once

#include "shared/layers/Layer.h"
#include "shared/FreeCameraController.h"
#include "shared/Shader.h"
#include "shared/Mesh.h"

class LOGL_09_Phong : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& evt) override;

private:
    Camera m_camera;
    FreeCameraController m_controller;

    Mesh m_cubeMesh;
    Mesh m_lightSourceMesh;

    Shader m_phongShader;
    Shader m_lightSourceShader;

    glm::vec3 m_objectColor;
    glm::vec3 m_lightColor;

};