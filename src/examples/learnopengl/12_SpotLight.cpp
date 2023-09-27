#include "12_SpotLight.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_SpotLight::OnAttach() {
	m_Camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ContainerTexture = Utils::LoadTexture("assets/container.png");
	m_ContainerSpecularTexture = Utils::LoadTexture("assets/container_specular.png");

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_LightSourceColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);

	m_LightColorUniform = spr::createUniform("light.color", spr::UniformType::Vec3);
	m_LightPositionUniform = spr::createUniform("light.position", spr::UniformType::Vec3);
	m_LightDirectionUniform = spr::createUniform("light.direction", spr::UniformType::Vec3);
	m_LightCutoffAngleUniform = spr::createUniform("light.cutoffAngle", spr::UniformType::Float);
	m_LightShadowSmoothAngleUniform = spr::createUniform("light.shadowSmoothAngle", spr::UniformType::Float);
	m_LightConstantAttenuationUniform = spr::createUniform("light.constantAttenuation", spr::UniformType::Float);
	m_LightLinearAttenuationUniform = spr::createUniform("light.linearAttenuation", spr::UniformType::Float);
	m_LightQuadraticAttenuationUniform = spr::createUniform("light.quadraticAttenuation", spr::UniformType::Float);

	m_MaterialAmbientStrengthUniform = spr::createUniform("material.ambientStrength", spr::UniformType::Float);
	m_MaterialDiffuseStrengthUniform = spr::createUniform("material.diffuseStrength", spr::UniformType::Float);
	m_MaterialSpecularStrengthUniform = spr::createUniform("material.specularStrength", spr::UniformType::Float);
	m_MaterialShininessUniform = spr::createUniform("material.shininess", spr::UniformType::Float);
	m_MaterialDiffuseMapUniform = spr::createUniform("material.diffuseMap", spr::UniformType::Sampler);
	m_MaterialSpecularMapUniform = spr::createUniform("material.specularMap", spr::UniformType::Sampler);

	m_TimeUniform = spr::createUniform("time", spr::UniformType::Float);

	m_SpotLightShaderProgram = Utils::LoadShaderProgram("shaders/09_specular_map.vert", "shaders/10_spotlight.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_12_SpotLight::OnUpdate() {
    m_Controller.HandleKeyboardInput();

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, m_Light.Position);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(m_Light.Color));
		spr::submit(m_LightSourceShaderProgram);
	}

	// All lighting Uniforms are set here, so the first draw call will submit them, and the rest will assume they're correctly set.
	// NOTE: If we only did this once outside the loop, it would probably work as well.
	glm::mat4 litModel(1.0f);
	litModel = glm::translate(litModel, glm::vec3(2.0f, 0.0f, 0.0f));
	{
		const int containerUnit = 0, specularUnit = 1;
		spr::setTexture(containerUnit, m_ContainerTexture);
		spr::setTexture(specularUnit, m_ContainerSpecularTexture);

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 32.f;
		const float time = Runtime::get()->getTime();
		const float cutoffAngleRadians = glm::radians(m_Light.CutoffAngle);
		const float shadowSmoothAngleRadians = glm::radians(m_Light.ShadowSmoothAngle);
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_Light.Color));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(m_Light.Position));
		spr::setUniform(m_LightDirectionUniform, glm::value_ptr(m_Light.Direction));
		spr::setUniform(m_LightCutoffAngleUniform, &cutoffAngleRadians);
		spr::setUniform(m_LightShadowSmoothAngleUniform, &shadowSmoothAngleRadians);
		spr::setUniform(m_LightConstantAttenuationUniform, &m_Light.ConstantAttenuation);
		spr::setUniform(m_LightLinearAttenuationUniform, &m_Light.LinearAttenuation);
		spr::setUniform(m_LightQuadraticAttenuationUniform, &m_Light.QuadraticAttenuation);
		spr::setUniform(m_MaterialDiffuseMapUniform, &containerUnit);
		spr::setUniform(m_MaterialSpecularMapUniform, &specularUnit);
		spr::setUniform(m_MaterialAmbientStrengthUniform, &ambientStrength);
		spr::setUniform(m_MaterialDiffuseStrengthUniform, &diffuseStrength);
		spr::setUniform(m_MaterialSpecularStrengthUniform, &specularStrength);
		spr::setUniform(m_MaterialShininessUniform, &shininess);
		spr::setUniform(m_TimeUniform, &time);
	}

	int boxCount = 25;
	int rowSize = 5;
	float gridSize = 1.2f;
	for(int i = 0; i < boxCount; i++) {
		glm::mat4 litModel(1.0f);
		int rowIdx = (i / rowSize) - (rowSize / 2);
		int colIdx = (i % rowSize) - (rowSize / 2);
		litModel = glm::translate(litModel, glm::vec3(rowIdx * gridSize, 0.f, colIdx * gridSize));
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::submit(m_SpotLightShaderProgram);
	}

	spr::render();
	spr::clean();
}

void LOGL_12_SpotLight::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_SpotLightShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ContainerTexture);
	spr::destroy(m_ContainerSpecularTexture);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightSourceColorUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_LightDirectionUniform);
	spr::destroy(m_LightCutoffAngleUniform);
	spr::destroy(m_LightShadowSmoothAngleUniform);
	spr::destroy(m_LightConstantAttenuationUniform);
	spr::destroy(m_LightLinearAttenuationUniform);
	spr::destroy(m_LightQuadraticAttenuationUniform);
	spr::destroy(m_MaterialDiffuseMapUniform);
	spr::destroy(m_MaterialSpecularMapUniform);
	spr::destroy(m_MaterialAmbientStrengthUniform);
	spr::destroy(m_MaterialDiffuseStrengthUniform);
	spr::destroy(m_MaterialSpecularStrengthUniform);
	spr::destroy(m_MaterialShininessUniform);
	spr::destroy(m_TimeUniform);
}

void LOGL_12_SpotLight::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

    ImGui::Begin("12 - Spotlight");
	const auto pickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
	ImGui::ColorPicker3("Color", glm::value_ptr(m_Light.Color), pickerFlags);
	ImGui::Spacing();
    ImGui::InputFloat3("Position", glm::value_ptr(m_Light.Position));
    ImGui::SliderFloat3("Direction", glm::value_ptr(m_Light.Direction), -1.f, 1.f);
    ImGui::Spacing();
    ImGui::SliderFloat("Cutoff Angle", &m_Light.CutoffAngle, 0.f, 180.f);
    ImGui::SliderFloat("Shadow smooth", &m_Light.ShadowSmoothAngle, 0.f, 180.f);
	ImGui::Spacing();
	ImGui::Text("Attenuation");
	ImGui::SliderFloat("Constant", &m_Light.ConstantAttenuation, 0.f, 1.f);
	ImGui::SliderFloat("Linear", &m_Light.LinearAttenuation, 0.f, 1.f);
	ImGui::SliderFloat("Quadratic", &m_Light.QuadraticAttenuation, 0.f, 1.f);
	ImGui::End();
}

void LOGL_12_SpotLight::OnEvent(const Event&evt) {
    m_Controller.HandleEvent(evt);
}
