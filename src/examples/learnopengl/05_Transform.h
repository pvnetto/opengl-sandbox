#pragma once

#include "shared/layers/Layer.h"
#include "shared/Mesh.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_05_Transform : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;

private:
	spr::TextureHandle LoadTexture(const char *path);

private:
    Mesh m_cube;

    spr::VertexBufferHandle m_CubeVertexBuffer;
    spr::IndexBufferHandle m_CubeIndexBuffer;

    spr::ProgramHandle m_ShaderProgram;
	spr::TextureHandle m_BrickTexture;
	spr::TextureHandle m_LadybugTexture;

	spr::UniformHandle m_TexUniform;
	spr::UniformHandle m_AnotherTexUniform;
	spr::UniformHandle m_TransformUniform;

};