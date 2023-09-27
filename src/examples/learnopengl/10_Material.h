#pragma once

#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class LOGL_10_Material : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(const Event&evt) override;

private:
	struct PhongMat {
		glm::vec3 Color = glm::vec3(0.2f, 0.35f, 1.f);
		glm::vec3 SpecularColor = glm::vec3(0.5f, 0.5f, 0.5f);

		float AmbientStrength = 0.1f;
		float DiffuseStrength = 0.8f;
		float SpecularStrength = 1.f;
		float Shininess = 32.f;
	};

private:
	Camera m_Camera;
	FreeCameraController m_Controller;

    PhongMat m_Material;
	glm::vec3 m_LightColor;

private:
	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_PhongShaderProgram;
	spr::ProgramHandle m_LightSourceShaderProgram;

	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;

	spr::UniformHandle m_LightSourceColorUniform;

	spr::UniformHandle m_ViewPositionUniform;
	spr::UniformHandle m_LightColorUniform;
	spr::UniformHandle m_LightPositionUniform;
	
	spr::UniformHandle m_MaterialColorUniform;
	spr::UniformHandle m_MaterialAmbientStrengthUniform;
	spr::UniformHandle m_MaterialDiffuseStrengthUniform;
	spr::UniformHandle m_MaterialSpecularStrengthUniform;
	spr::UniformHandle m_MaterialShininessUniform;
	spr::UniformHandle m_MaterialSpecularColorUniform;
};
