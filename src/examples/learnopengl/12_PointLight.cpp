#include "12_PointLight.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_PointLight::OnAttach() {
	m_Camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);
	m_Static = false;


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

	m_PointLightShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/10_point_light.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_12_PointLight::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	glm::mat4 sourceModel(1.0f);
	glm::vec3 lightSourcePos{0.f, 0.f, glm::cos(Runtime::get()->getTime() * 1.5f) * -3.f};
	if (m_Static) {
        lightSourcePos = m_Light.Position;
	}
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(m_Light.Color));
		spr::submit(m_LightSourceShaderProgram);
	}

	// All lighting Uniforms are submitted here. The subsequent draw calls assume these are set correctly.
	glm::mat4 litModel(1.0f);
	litModel = glm::translate(litModel, glm::vec3(2.0f, 0.0f, 0.0f));
	{
		spr::setVertexBuffer(m_CubeVertexBuffer);

		const int containerUnit = 0, specularUnit = 1;
		spr::setTexture(containerUnit, m_ContainerTexture);
		spr::setTexture(specularUnit, m_ContainerSpecularTexture);

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 32.f;
		const float time = Runtime::get()->getTime();
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_Light.Color));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
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

		spr::submit(m_PointLightShaderProgram);
	}


	{
		litModel = glm::mat4(1.0f);
		litModel = glm::translate(litModel, glm::vec3(-2.0f, 0.0f, 0.0f));
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::submit(m_PointLightShaderProgram);
	}


	{
		litModel = glm::mat4(1.0f);
		litModel = glm::translate(litModel, glm::vec3(0.0f, 2.0f, 0.0f));
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::submit(m_PointLightShaderProgram);
	}

	{
		litModel = glm::mat4(1.0f);
		litModel = glm::translate(litModel, glm::vec3(0.0f, -2.0f, 0.0f));
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::submit(m_PointLightShaderProgram);
	}


	spr::render();
	spr::clean();
}

void LOGL_12_PointLight::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_PointLightShaderProgram);
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

void LOGL_12_PointLight::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

    ImGui::Begin("12 - Point Light");
	const auto pickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
	ImGui::ColorPicker3("Color", glm::value_ptr(m_Light.Color), pickerFlags);
	ImGui::Spacing();
	ImGui::Checkbox("Static", &m_Static);
	if (m_Static) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_Light.Position));
	}
	ImGui::Spacing();
	ImGui::Text("Attenuation");
	ImGui::InputFloat("Constant", &m_Light.ConstantAttenuation);
	ImGui::InputFloat("Linear", &m_Light.LinearAttenuation);
	ImGui::InputFloat("Quadratic", &m_Light.QuadraticAttenuation);
	ImGui::End();
}

void LOGL_12_PointLight::OnEvent(const Event&evt) {
	m_Controller.HandleEvent(evt);
}
