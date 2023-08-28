#pragma once

#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_06_MVP : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;
	spr::TextureHandle m_BrickTexture;
	spr::TextureHandle m_LadybugTexture;

	spr::ProgramHandle m_ShaderProgram;
	spr::UniformHandle m_TexUniform;
	spr::UniformHandle m_AnotherTexUniform;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_CameraPosition;

    float m_FieldOfView;

};