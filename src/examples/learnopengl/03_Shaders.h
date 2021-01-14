#pragma once

#include "shared/layers/Layer.h"
#include "shared/Shader.h"

class LOGL_03_Shaders : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_vaoA, m_vaoB;
    Shader m_shader;
};