#pragma once

#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class LOGL_03_Shaders : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnDetach() override;

private:
    spr::ProgramHandle m_ShaderProgram;
    spr::UniformHandle m_Uniform;
    spr::VertexBufferHandle m_TriangleVertexBuffer;

    void CreateBuffers();
    void CreateShaders();
};
