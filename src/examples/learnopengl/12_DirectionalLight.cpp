#include "12_DirectionalLight.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_DirectionalLight::OnAttach() {
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
	m_LightDirectionUniform = spr::createUniform("light.direction", spr::UniformType::Vec3);

	m_MaterialAmbientStrengthUniform = spr::createUniform("material.ambientStrength", spr::UniformType::Float);
	m_MaterialDiffuseStrengthUniform = spr::createUniform("material.diffuseStrength", spr::UniformType::Float);
	m_MaterialSpecularStrengthUniform = spr::createUniform("material.specularStrength", spr::UniformType::Float);
	m_MaterialShininessUniform = spr::createUniform("material.shininess", spr::UniformType::Float);
	m_MaterialDiffuseMapUniform = spr::createUniform("material.diffuseMap", spr::UniformType::Sampler);
	m_MaterialSpecularMapUniform = spr::createUniform("material.specularMap", spr::UniformType::Sampler);

	m_TimeUniform = spr::createUniform("time", spr::UniformType::Float);

	m_DirectionalLightShaderProgram = Utils::LoadShaderProgram("shaders/09_specular_map.vert", "shaders/10_directional_light.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_12_DirectionalLight::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	glm::mat4 litModel(1.0f);
	{
		spr::setVertexBuffer(m_CubeVertexBuffer);

		const int containerUnit = 0, specularUnit = 1;
		spr::setTexture(containerUnit, m_ContainerTexture);
		spr::setTexture(specularUnit, m_ContainerSpecularTexture);

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 32.f;
		const float time = spr::runtime::getTime();
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_Light.Color));
		spr::setUniform(m_LightDirectionUniform, glm::value_ptr(m_Light.Direction));
		spr::setUniform(m_MaterialDiffuseMapUniform, &containerUnit);
		spr::setUniform(m_MaterialSpecularMapUniform, &specularUnit);
		spr::setUniform(m_MaterialAmbientStrengthUniform, &ambientStrength);
		spr::setUniform(m_MaterialDiffuseStrengthUniform, &diffuseStrength);
		spr::setUniform(m_MaterialSpecularStrengthUniform, &specularStrength);
		spr::setUniform(m_MaterialShininessUniform, &shininess);
		spr::setUniform(m_TimeUniform, &time);

		spr::submit(m_DirectionalLightShaderProgram);
	}

	spr::render();
	spr::clean();
}

void LOGL_12_DirectionalLight::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_DirectionalLightShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ContainerTexture);
	spr::destroy(m_ContainerSpecularTexture);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightSourceColorUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightDirectionUniform);
	spr::destroy(m_MaterialDiffuseMapUniform);
	spr::destroy(m_MaterialSpecularMapUniform);
	spr::destroy(m_MaterialAmbientStrengthUniform);
	spr::destroy(m_MaterialDiffuseStrengthUniform);
	spr::destroy(m_MaterialSpecularStrengthUniform);
	spr::destroy(m_MaterialShininessUniform);
	spr::destroy(m_TimeUniform);
}

void LOGL_12_DirectionalLight::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(300, 300));
    ImGui::Begin("12 - Directional Light");

    ImGui::ColorPicker3("Color", glm::value_ptr(m_Light.Color));
    ImGui::Spacing();
    ImGui::SliderFloat3("Direction", glm::value_ptr(m_Light.Direction), -1.f, 1.f);

    ImGui::End();
}

void LOGL_12_DirectionalLight::OnEvent(Event &evt) {
	m_Controller.HandleEvent(evt);
}
