#pragma once

#include "shared/layers/Layer.h"

class LOGL_02_VAO : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_VAOa, m_VAOb;
    unsigned int m_ShaderProgram, m_YellowShaderProgram;

    void DeclareBuffers();
    void DeclareShader(unsigned int& shaderId, const char* vertexShaderSource, const char* fragShaderSource);
};
