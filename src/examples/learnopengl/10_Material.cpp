#include "10_Material.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_10_Material::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);
    m_LightColor = glm::vec3{1.f, 0.8f, 0.6f};

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_LightSourceColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);

	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);
	m_LightColorUniform = spr::createUniform("light.color", spr::UniformType::Vec3);
	m_LightPositionUniform = spr::createUniform("light.position", spr::UniformType::Vec3);
	m_MaterialColorUniform = spr::createUniform("material.color", spr::UniformType::Vec3);

	m_MaterialAmbientStrengthUniform = spr::createUniform("material.ambientStrength", spr::UniformType::Float);
	m_MaterialDiffuseStrengthUniform = spr::createUniform("material.diffuseStrength", spr::UniformType::Float);
	m_MaterialSpecularStrengthUniform = spr::createUniform("material.specularStrength", spr::UniformType::Float);
	m_MaterialShininessUniform = spr::createUniform("material.shininess", spr::UniformType::Float);
	m_MaterialSpecularColorUniform = spr::createUniform("material.specularColor", spr::UniformType::Vec3);

	m_PhongShaderProgram = Utils::LoadShaderProgram("shaders/07_phong.vert", "shaders/08_material.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_10_Material::OnUpdate() {
	m_Controller.HandleKeyboardInput();
	glm::vec3 lightSourcePos = glm::vec3(std::cos(spr::runtime::getTime()),
	                                     std::cos(spr::runtime::getTime()) * std::sin(spr::runtime::getTime()),
	                                     std::sin(spr::runtime::getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(m_LightColor));
	spr::submit(m_LightSourceShaderProgram);


	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));

	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
	spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
	spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
	spr::setUniform(m_MaterialColorUniform, glm::value_ptr(m_Material.Color));
	spr::setUniform(m_MaterialAmbientStrengthUniform, &m_Material.AmbientStrength);
	spr::setUniform(m_MaterialDiffuseStrengthUniform, &m_Material.DiffuseStrength);
	spr::setUniform(m_MaterialSpecularStrengthUniform, &m_Material.SpecularStrength);
	spr::setUniform(m_MaterialShininessUniform, &m_Material.Shininess);
	spr::setUniform(m_MaterialSpecularColorUniform, glm::value_ptr(m_Material.SpecularColor));
	spr::submit(m_PhongShaderProgram);


	spr::render();
	spr::clean();
}

void LOGL_10_Material::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_PhongShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightSourceColorUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_MaterialColorUniform);
	spr::destroy(m_MaterialAmbientStrengthUniform);
	spr::destroy(m_MaterialDiffuseStrengthUniform);
	spr::destroy(m_MaterialSpecularStrengthUniform);
	spr::destroy(m_MaterialShininessUniform);
	spr::destroy(m_MaterialSpecularColorUniform);
}

void LOGL_10_Material::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));
	ImGui::Begin("09 - Phong Shading");

	const auto pickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
	if (ImGui::TreeNode("Material")) {
		ImGui::ColorPicker3("Color", glm::value_ptr(m_Material.Color), pickerFlags);
		ImGui::ColorPicker3("Specular Color", glm::value_ptr(m_Material.SpecularColor), pickerFlags);
		ImGui::SliderFloat("Ambient Strength", &m_Material.AmbientStrength, 0.f, 1.f);
		ImGui::SliderFloat("Diffuse Strength", &m_Material.DiffuseStrength, 0.f, 1.f);
		ImGui::SliderFloat("Specular Strength", &m_Material.SpecularStrength, 0.f, 1.f);
		ImGui::SliderFloat("Shininess", &m_Material.Shininess, 0.f, 256.f);

        ImGui::TreePop();
	}

	if (ImGui::TreeNode("Light")) {
		ImGui::ColorPicker3("Color", glm::value_ptr(m_LightColor), pickerFlags);

        ImGui::TreePop();
	}

	ImGui::End();
}

void LOGL_10_Material::OnEvent(Event &evt) {
	m_Controller.HandleEvent(evt);
}
