#pragma once

#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_03_Shaders : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    unsigned int m_vaoA, m_vaoB;
    spr::ProgramHandle m_shaderProgramHandle;
    spr::UniformHandle m_uniformHandle;

    void DeclareBuffers();
    void DeclareShader();
};