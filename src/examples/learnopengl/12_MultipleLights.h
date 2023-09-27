#pragma once

#include "shared/Light.h"
#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class LOGL_12_MultipleLights : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(const Event&evt) override;

private:
	Camera m_Camera;
	FreeCameraController m_Controller;

    std::vector<DirectionalLight> m_DirectionalLights;
    PointLight m_PointLight;
    SpotLight m_Spotlight;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_LightingShaderProgram;
	spr::ProgramHandle m_LightSourceShaderProgram;

	spr::TextureHandle m_ContainerTexture;
	spr::TextureHandle m_ContainerSpecularTexture;

	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

	spr::UniformHandle m_LightSourceColorUniform;
	spr::UniformHandle m_ViewPositionUniform;

	spr::UniformHandle m_EnabledDirectionalLightsUniform;
	spr::UniformHandle m_DirectionalLightDirectionUniforms;
	spr::UniformHandle m_DirectionalLightColorUniforms;

	spr::UniformHandle m_PointLightColorUniform;
	spr::UniformHandle m_PointLightPositionUniform;
	spr::UniformHandle m_PointLightConstantAttenuationUniform;
	spr::UniformHandle m_PointLightLinearAttenuationUniform;
	spr::UniformHandle m_PointLightQuadraticAttenuationUniform;

	spr::UniformHandle m_SpotLightColorUniform;
	spr::UniformHandle m_SpotLightPositionUniform;
	spr::UniformHandle m_SpotLightDirectionUniform;
	spr::UniformHandle m_SpotLightCutoffAngleUniform;
	spr::UniformHandle m_SpotLightShadowSmoothAngleUniform;
	spr::UniformHandle m_SpotLightConstantAttenuationUniform;
	spr::UniformHandle m_SpotLightLinearAttenuationUniform;
	spr::UniformHandle m_SpotLightQuadraticAttenuationUniform;

	spr::UniformHandle m_MaterialDiffuseMapUniform;
	spr::UniformHandle m_MaterialSpecularMapUniform;
	spr::UniformHandle m_MaterialAmbientStrengthUniform;
	spr::UniformHandle m_MaterialDiffuseStrengthUniform;
	spr::UniformHandle m_MaterialSpecularStrengthUniform;
	spr::UniformHandle m_MaterialShininessUniform;

	spr::UniformHandle m_TimeUniform;

};