#pragma once

#include "shared/Camera.h"
#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_08_Lighting : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
    Camera m_Camera;
    glm::vec3 m_CubeColor;
    glm::vec3 m_LightColor;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_LitShaderProgram;
	spr::ProgramHandle m_LightSourceShaderProgram;

	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_LightColorUniform;
	spr::UniformHandle m_ObjectColorUniform;

};