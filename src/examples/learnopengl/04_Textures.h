#pragma once

#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_04_Textures : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;

private:
    spr::VertexBufferHandle m_VertexBufferHandle;
    spr::IndexBufferHandle m_IndexBufferHandle;

    spr::UniformHandle m_TexUniform0;
    spr::UniformHandle m_TexUniform1;
    spr::ProgramHandle m_ShaderHandle;

	unsigned int m_BrickTexture, m_OtherTexture;
	unsigned int m_OtherTextureSampler;
};
