#include "08_Lighting.h"

#include "shared/Primitive.h"
#include "shared/renderer/SimpleRenderer.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_08_Lighting::OnAttach() {
	m_camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	m_cubeMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>()};
	m_lightSourceMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>()};

	m_litShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_basic_lighting.frag");
	m_lightSourceShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");

    m_cubeColor = glm::vec3(0.2f, 0.35f, 1.f);
    m_lightColor = glm::vec3{0.8f, 0.8f, 1.f};
}

void LOGL_08_Lighting::OnUpdate() {
	// 1) Draws light source
	glm::vec3 orbitPosition = glm::vec3(
	    std::cos(spr::runtime::getTime()),
	    std::cos(spr::runtime::getTime()) * std::sin(spr::runtime::getTime()),
	    std::sin(spr::runtime::getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, orbitPosition);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", sourceModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", m_lightColor);
	m_lightSourceMesh.Draw(m_lightSourceShader);

	// 2) Draws lit object
	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

	m_litShader.Use();
	m_litShader.SetMatrix("model", litModel);
	m_litShader.SetMatrix("view", m_camera.GetView());
	m_litShader.SetMatrix("projection", m_camera.GetProjection());
	m_litShader.SetVector3("objectColor", m_cubeColor);
	m_litShader.SetVector3("lightColor", m_lightColor);

	m_cubeMesh.Draw(m_litShader);
}

void LOGL_08_Lighting::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

    ImGui::Begin("08 - Basic Lighting");
    ImGui::ColorPicker3("Object Color", glm::value_ptr(m_cubeColor));
    ImGui::ColorPicker3("Light Color", glm::value_ptr(m_lightColor));
    ImGui::End();


    ImGui::Begin("Info");
    ImGui::TextWrapped("This example contains only solid colors. Object color is multiplied by light color, there are no shadows.");
    ImGui::End();
}
