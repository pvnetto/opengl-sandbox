#pragma once

#include "shared/Model.h"
#include "shared/Shader.h"
#include "Layer.h"

class SandboxLayer : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
    Model m_quad;
    Shader m_shader;

};