#include "12_PointLight.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/renderer/SimpleRenderer.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_PointLight::OnAttach() {
	m_camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	std::vector<Texture> textures = {
	    TextureLoader::LoadTexture("assets/container.png"),
	    TextureLoader::LoadTexture("assets/container_specular.png")};

	m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
	m_lightSourceMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>());

	m_phongShader = Shader("shaders/09_specular_map.vert", "shaders/10_point_light.frag");
	m_lightSourceShader = Shader("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");

	m_static = false;
}

void LOGL_12_PointLight::OnUpdate() {
	m_controller.HandleKeyboardInput();

	glm::mat4 sourceModel(1.0f);
	glm::vec3 lightSourcePos{0.f, 0.f, glm::cos(spr::runtime::getTime() * 1.5f) * -3.f};
    if(m_static)
        lightSourcePos = m_light.Position;
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", sourceModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", m_light.Color);
    m_lightSourceMesh.Draw(m_lightSourceShader);

	m_phongShader.Use();

	m_phongShader.SetInt("material.diffuseMap", 0);
	m_phongShader.SetInt("material.specularMap", 1);
	m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	m_phongShader.SetFloat("material.specularStrength", 1.f);
	m_phongShader.SetFloat("material.shininess", 32.f);

	m_phongShader.SetVector3("light.color", m_light.Color);
	m_phongShader.SetVector3("light.position", lightSourcePos);
	m_phongShader.SetFloat("light.constantAttenuation", m_light.ConstantAttenuation);
	m_phongShader.SetFloat("light.linearAttenuation", m_light.LinearAttenuation);
	m_phongShader.SetFloat("light.quadraticAttenuation", m_light.QuadraticAttenuation);

	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());
	m_phongShader.SetFloat("time", spr::runtime::getTime());

	glm::mat4 litModel(1.0f);
	litModel = glm::translate(litModel, glm::vec3(2.0f, 0.0f, 0.0f));
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());
    m_cubeMesh.Draw(m_phongShader);

	litModel = glm::mat4(1.0f);
	litModel = glm::translate(litModel, glm::vec3(-2.0f, 0.0f, 0.0f));
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());
    m_cubeMesh.Draw(m_phongShader);

	litModel = glm::mat4(1.0f);
	litModel = glm::translate(litModel, glm::vec3(0.0f, 2.0f, 0.0f));
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());
    m_cubeMesh.Draw(m_phongShader);

	litModel = glm::mat4(1.0f);
	litModel = glm::translate(litModel, glm::vec3(0.0f, -2.0f, 0.0f));
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());
    m_cubeMesh.Draw(m_phongShader);
}

void LOGL_12_PointLight::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

    ImGui::Begin("12 - Point Light");
	const auto pickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
	ImGui::ColorPicker3("Color", glm::value_ptr(m_light.Color), pickerFlags);
	ImGui::Spacing();
	ImGui::Checkbox("Static", &m_static);
	if (m_static)
		ImGui::InputFloat3("Position", glm::value_ptr(m_light.Position));
	ImGui::Spacing();
	ImGui::Text("Attenuation");
	ImGui::InputFloat("Constant", &m_light.ConstantAttenuation);
	ImGui::InputFloat("Linear", &m_light.LinearAttenuation);
	ImGui::InputFloat("Quadratic", &m_light.QuadraticAttenuation);
	ImGui::End();
}

void LOGL_12_PointLight::OnEvent(Event &evt) {
	m_controller.HandleEvent(evt);
}
