#pragma once

#include "shared/layers/Layer.h"
#include "shared/Camera.h"
#include "shared/Shader.h"
#include "shared/Mesh.h"

struct PerspectiveProps {
    float FOV = 90.f, Near = 0.1f, Far = 100.f;
};

struct OrthographicProps {
    float Near = 0.1f, Far = 100.f, Size = 4.f;
};

class LOGL_07_Camera : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
    Camera m_camera;
    Shader m_shader;
    Mesh m_cube;

    glm::vec3 m_cameraRotation;
    int m_projectionType;

    PerspectiveProps m_perspective;
    OrthographicProps m_orthographic;
};