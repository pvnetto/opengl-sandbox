#pragma once

#include "shared/layers/Layer.h"

class LOGL_02_VBO : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_vao, m_vbo;
    unsigned int m_shaderProgram;

    void DeclareBuffers();
    void DeclareShader();
};