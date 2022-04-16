#pragma once

#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_03_Shaders : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnDettach() override;

private:
    spr::ProgramHandle m_shaderProgramHandle;
    spr::UniformHandle m_uniformHandle;
    spr::VertexBufferHandle m_triangleVBHandle;

    void CreateBuffers();
    void CreateShaders();
};