#pragma once

#include "shared/layers/Layer.h"
#include "shared/Shader.h"

class LOGL_04_Textures : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_vao;
    Shader m_shader;
};