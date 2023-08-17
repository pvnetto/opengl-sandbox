#pragma once

#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"
#include "shared/renderer/SimpleRenderer.h"

class LOGL_12_SpotLight : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event &evt) override;

private:
	struct SpotLight {
		glm::vec3 Color = glm::vec3{1.f, 0.6f, 0.1f};
		glm::vec3 Position = glm::vec3{0.f, 5.f, 0.f};
		glm::vec3 Direction = glm::vec3{0.f, -1.f, 0.f};

		float CutoffAngle = 30.f;
		float ShadowSmoothAngle = 6.f;

		float ConstantAttenuation = 1.f;
		float LinearAttenuation = 0.09f;
		float QuadraticAttenuation = 0.032f;
	};

private:
	Camera m_Camera;
	FreeCameraController m_Controller;

    SpotLight m_Light;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_SpotLightShaderProgram;
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
	spr::UniformHandle m_LightDirectionUniform;
	spr::UniformHandle m_LightCutoffAngleUniform;
	spr::UniformHandle m_LightShadowSmoothAngleUniform;
	spr::UniformHandle m_LightConstantAttenuationUniform;
	spr::UniformHandle m_LightLinearAttenuationUniform;
	spr::UniformHandle m_LightQuadraticAttenuationUniform;

	spr::UniformHandle m_MaterialDiffuseMapUniform;
	spr::UniformHandle m_MaterialSpecularMapUniform;
	spr::UniformHandle m_MaterialAmbientStrengthUniform;
	spr::UniformHandle m_MaterialDiffuseStrengthUniform;
	spr::UniformHandle m_MaterialSpecularStrengthUniform;
	spr::UniformHandle m_MaterialShininessUniform;

	spr::UniformHandle m_TimeUniform;
};