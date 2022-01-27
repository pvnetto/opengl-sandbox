#pragma once

#include "shared/layers/Layer.h"

class LOGL_02_VAO : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_vaoA, m_vaoB;
    unsigned int m_shaderProgram, m_yellowShaderProgram;

    void DeclareBuffers();
    void DeclareShader(unsigned int& shaderId, const char* vertexShaderSource, const char* fragShaderSource);
};