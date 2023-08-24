#pragma once

#include "shared/layers/Layer.h"

class LOGL_02_VBO : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_VAO, m_VBO;
    unsigned int m_ShaderProgram;

    void DeclareBuffers();
    void DeclareShader();
};