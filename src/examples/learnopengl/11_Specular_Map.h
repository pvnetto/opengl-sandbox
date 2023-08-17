#pragma once

#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_11_SpecularMap : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event &evt) override;

private:
	Camera m_Camera;
	FreeCameraController m_Controller;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_SpecularShaderProgram;
	spr::ProgramHandle m_LightSourceShaderProgram;

	spr::TextureHandle m_ContainerTexture;
	spr::TextureHandle m_ContainerSpecularTexture;

	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

	spr::UniformHandle m_LightSourceColorUniform;

	spr::UniformHandle m_ViewPositionUniform;
	spr::UniformHandle m_LightColorUniform;
	spr::UniformHandle m_LightPositionUniform;

	spr::UniformHandle m_MaterialDiffuseMapUniform;
	spr::UniformHandle m_MaterialSpecularMapUniform;
	spr::UniformHandle m_MaterialAmbientStrengthUniform;
	spr::UniformHandle m_MaterialDiffuseStrengthUniform;
	spr::UniformHandle m_MaterialSpecularStrengthUniform;
	spr::UniformHandle m_MaterialShininessUniform;

	spr::UniformHandle m_TimeUniform;

};