#include "09_Phong.h"

#include "shared/Primitive.h"
#include "shared/renderer/SimpleRenderer.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_09_Phong::OnAttach() {
	// 1) Declares mesh with normal data, as Phong shading uses it to determine how lit the object is
	m_camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
    m_controller = FreeCameraController(m_camera, 5.f);

	m_cubeMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>()};
	m_lightSourceMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>()};

	m_phongShader = Shader("shaders/07_phong.vert", "shaders/07_phong.frag");
	m_lightSourceShader = Shader("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");

    m_objectColor = glm::vec3{ 0.2f, 0.35f, 1.f };
    m_lightColor = glm::vec3{ 0.8f, 0.8f, 1.f };
}

void LOGL_09_Phong::OnUpdate() {
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
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

	m_phongShader.Use();
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());
	m_phongShader.SetVector3("objectColor", m_objectColor);
	m_phongShader.SetVector3("lightColor", m_lightColor);
	m_phongShader.SetVector3("lightPos", lightSourcePos);
	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());
    m_cubeMesh.Draw(m_phongShader);
}

void LOGL_09_Phong::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("09 - Phong Shading");

    const auto flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
    ImGui::ColorPicker3("Object Color", glm::value_ptr(m_objectColor), flags);
    ImGui::Spacing();
    ImGui::ColorPicker3("Light Color", glm::value_ptr(m_lightColor), flags);

    ImGui::End();
}

void LOGL_09_Phong::OnEvent(Event& evt) {
    m_controller.HandleEvent(evt);
}
