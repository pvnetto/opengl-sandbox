#pragma once

#include "shared/layers/Layer.h"

// Info on how to use Element Buffer Objects (aka Index Buffers)
class LOGL_02_EBO : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_VAO, m_VBO, m_EBO;
    unsigned int m_ShaderProgram;

    void DeclareBufferObjects();
    void DeclareShaderProgram();
};

