#pragma once

#include "shared/layers/Layer.h"
#include "shared/Shader.h"
#include "shared/Mesh.h"

class LOGL_06_MVP : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
    Shader m_shader;
    Mesh m_cube;

    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_cameraPosition;

    float m_fov;

};