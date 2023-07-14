#include "10_Material.h"

#include "shared/Primitive.h"
#include "shared/renderer/SimpleRenderer.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_10_Material::OnAttach() {
	m_camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	m_cubeMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>()};
	m_lightSourceMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>()};

	m_phongShader = Shader("shaders/07_phong.vert", "shaders/08_material.frag");
	m_lightSourceShader = Shader("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");

    m_lightColor = glm::vec3{1.f, 0.8f, 0.6f};
}

void LOGL_10_Material::OnUpdate() {
	m_controller.HandleKeyboardInput();
	glm::vec3 lightSourcePos = glm::vec3(std::cos(spr::runtime::getTime()),
	                                     std::cos(spr::runtime::getTime()) * std::sin(spr::runtime::getTime()),
	                                     std::sin(spr::runtime::getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", sourceModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", m_lightColor);
	m_lightSourceMesh.Draw(m_lightSourceShader);

	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));

	m_phongShader.Use();
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());
	m_phongShader.SetVector3("material.color", m_material.Color);
	m_phongShader.SetFloat("material.ambientStrength", m_material.AmbientStrength);
	m_phongShader.SetFloat("material.diffuseStrength", m_material.DiffuseStrength);
	m_phongShader.SetFloat("material.specularStrength", m_material.SpecularStrength);
	m_phongShader.SetFloat("material.shininess", m_material.Shininess);
	m_phongShader.SetVector3("material.specularColor", m_material.SpecularColor);
	m_phongShader.SetVector3("light.color", m_lightColor);
	m_phongShader.SetVector3("light.position", lightSourcePos);
	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());

	m_cubeMesh.Draw(m_phongShader);
}

void LOGL_10_Material::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));
	ImGui::Begin("09 - Phong Shading");

	const auto pickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
	if (ImGui::TreeNode("Material")) {
		ImGui::ColorPicker3("Color", glm::value_ptr(m_material.Color), pickerFlags);
		ImGui::ColorPicker3("Specular Color", glm::value_ptr(m_material.SpecularColor), pickerFlags);
		ImGui::SliderFloat("Ambient Strength", &m_material.AmbientStrength, 0.f, 1.f);
		ImGui::SliderFloat("Diffuse Strength", &m_material.DiffuseStrength, 0.f, 1.f);
		ImGui::SliderFloat("Specular Strength", &m_material.SpecularStrength, 0.f, 1.f);
		ImGui::SliderFloat("Shininess", &m_material.Shininess, 0.f, 256.f);

        ImGui::TreePop();
	}

	if (ImGui::TreeNode("Light")) {
		ImGui::ColorPicker3("Color", glm::value_ptr(m_lightColor), pickerFlags);

        ImGui::TreePop();
	}

	ImGui::End();
}

void LOGL_10_Material::OnEvent(Event &evt) {
	m_controller.HandleEvent(evt);
}
