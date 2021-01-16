#include "12_SpotLight.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_SpotLight::OnAttach() {
	m_camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	std::vector<Texture> textures = {
	    TextureLoader::LoadTexture("../../src/assets/container.png"),
	    TextureLoader::LoadTexture("../../src/assets/container_specular.png")};

	m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
	m_lightSourceMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>());

	m_phongShader = Shader("../../src/shaders/09_specular_map.vert", "../../src/shaders/10_spotlight.frag");
	m_lightSourceShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");
}

void LOGL_12_SpotLight::OnUpdate() {
        m_controller.HandleKeyboardInput();

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, m_light.Position);
		sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

		m_lightSourceShader.Use();
		m_lightSourceShader.SetMatrix("model", sourceModel);
		m_lightSourceShader.SetMatrix("view",m_camera.GetView());
		m_lightSourceShader.SetMatrix("projection",m_camera.GetProjection());
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
		m_phongShader.SetVector3("light.position", m_light.Position);
		m_phongShader.SetVector3("light.direction", m_light.Direction);
		m_phongShader.SetFloat("light.constantAttenuation", m_light.ConstantAttenuation);
		m_phongShader.SetFloat("light.linearAttenuation", m_light.LinearAttenuation);
		m_phongShader.SetFloat("light.quadraticAttenuation", m_light.QuadraticAttenuation);
		m_phongShader.SetFloat("light.cutoffAngle", glm::radians(m_light.CutoffAngle));
		m_phongShader.SetFloat("light.shadowSmoothAngle", glm::radians(m_light.ShadowSmoothAngle));

		m_phongShader.SetVector3("viewPos", m_camera.GetPosition());

		int boxCount = 25;
		int rowSize = 5;
		float gridSize = 1.2f;
		for(int i = 0; i < boxCount; i++) {
			glm::mat4 litModel(1.0f);
			int rowIdx = (i / rowSize) - (rowSize / 2);
			int colIdx = (i % rowSize) - (rowSize / 2);
			litModel = glm::translate(litModel, glm::vec3(rowIdx * gridSize, 0.f, colIdx * gridSize));
			m_phongShader.SetMatrix("model", litModel);
			m_phongShader.SetMatrix("view",m_camera.GetView());
			m_phongShader.SetMatrix("projection",m_camera.GetProjection());

            m_cubeMesh.Draw(m_phongShader);
		}
}

void LOGL_12_SpotLight::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

    ImGui::Begin("12 - Spotlight");
	const auto pickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
	ImGui::ColorPicker3("Color", glm::value_ptr(m_light.Color), pickerFlags);
	ImGui::Spacing();
    ImGui::InputFloat3("Position", glm::value_ptr(m_light.Position));
    ImGui::SliderFloat3("Direction", glm::value_ptr(m_light.Direction), -1.f, 1.f);
    ImGui::Spacing();
    ImGui::SliderFloat("Cutoff Angle", &m_light.CutoffAngle, 0.f, 180.f);
    ImGui::SliderFloat("Shadow smooth", &m_light.ShadowSmoothAngle, 0.f, 180.f);
	ImGui::Spacing();
	ImGui::Text("Attenuation");
	ImGui::SliderFloat("Constant", &m_light.ConstantAttenuation, 0.f, 1.f);
	ImGui::SliderFloat("Linear", &m_light.LinearAttenuation, 0.f, 1.f);
	ImGui::SliderFloat("Quadratic", &m_light.QuadraticAttenuation, 0.f, 1.f);
	ImGui::End();
}

void LOGL_12_SpotLight::OnEvent(Event &evt) {
    m_controller.HandleEvent(evt);
}
