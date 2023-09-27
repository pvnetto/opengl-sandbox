#pragma once

#include "shared/FreeCameraController.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>


class LOGL_13_Meshes : public Layer {
public:
	LOGL_13_Meshes();
	~LOGL_13_Meshes();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;
	virtual void OnEvent(const Event& evt) override;

	Camera& GetCamera() { return m_Camera; }

protected:
	Camera m_Camera;
	FreeCameraController m_Controller;

private:
	Utils::ModelInstance m_BackpackModel;
	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_PhongShaderProgram;
	spr::ProgramHandle m_LightSourceShaderProgram;

	spr::TextureHandle m_ContainerTexture;
	spr::TextureHandle m_ContainerSpecularTexture;

	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

	spr::UniformHandle m_LightSourceColorUniform;

	spr::UniformHandle m_ViewPositionUniform;
	spr::UniformHandle m_PointLightColorUniform;
	spr::UniformHandle m_PointLightPositionUniform;
	spr::UniformHandle m_PointLightConstantAttenuationUniform;
	spr::UniformHandle m_PointLightLinearAttenuationUniform;
	spr::UniformHandle m_PointLightQuadraticAttenuationUniform;

	spr::UniformHandle m_MaterialDiffuseMapUniform;
	spr::UniformHandle m_MaterialSpecularMapUniform;
	spr::UniformHandle m_MaterialAmbientStrengthUniform;
	spr::UniformHandle m_MaterialDiffuseStrengthUniform;
	spr::UniformHandle m_MaterialSpecularStrengthUniform;
	spr::UniformHandle m_MaterialShininessUniform;

	spr::UniformHandle m_TimeUniform;

};