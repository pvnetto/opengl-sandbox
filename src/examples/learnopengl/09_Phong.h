#pragma once

#include "shared/layers/Layer.h"
#include "shared/FreeCameraController.h"
#include <spr/SimpleRenderer.h>

class LOGL_09_Phong : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(const Event& evt) override;

private:
    Camera m_Camera;
    FreeCameraController m_Controller;

    glm::vec3 m_ObjectColor;
    glm::vec3 m_LightColor;

private:
    spr::VertexBufferHandle m_CubeVertexBuffer;

    spr::ProgramHandle m_PhongShaderProgram;
	spr::ProgramHandle m_LightSourceShaderProgram;

    spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_ViewPositionUniform;
	spr::UniformHandle m_LightColorUniform;
	spr::UniformHandle m_LightPositionUniform;
	spr::UniformHandle m_ObjectColorUniform;
};