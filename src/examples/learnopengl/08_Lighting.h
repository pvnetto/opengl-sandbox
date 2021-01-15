#pragma once

#include "shared/layers/Layer.h"
#include "shared/Camera.h"
#include "shared/Shader.h"
#include "shared/Mesh.h"

class LOGL_08_Lighting : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
    Camera m_camera;
    Mesh m_cubeMesh;
    Mesh m_lightSourceMesh;

    Shader m_litShader;
    Shader m_lightSourceShader;

    glm::vec3 m_cubeColor;
    glm::vec3 m_lightColor;
};