#pragma once

#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_04_Textures : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    spr::VertexBufferHandle m_vertexBufferHandle;
    spr::IndexBufferHandle m_indexBufferHandle;

    spr::UniformHandle m_texUniform0;
    spr::UniformHandle m_texUniform1;
    spr::ProgramHandle m_shaderHandle;
};