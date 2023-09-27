#pragma once

#include "shared/Camera.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>


class LOGL_07_Camera : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
    Camera m_Camera;

    glm::vec3 m_CameraRotation;
    int m_ProjectionType;

    PerspectiveProps m_Perspective;
    OrthographicProps m_Orthographic;

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
};