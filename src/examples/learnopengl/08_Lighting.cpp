#include "08_Lighting.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_08_Lighting::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

    m_CubeColor = glm::vec3(0.2f, 0.35f, 1.f);
    m_LightColor = glm::vec3{0.8f, 0.8f, 1.f};

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	// 0. Declares Lighting Uniforms
	m_LightColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_ObjectColorUniform = spr::createUniform("objectColor", spr::UniformType::Vec3);

	m_LitShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_basic_lighting.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_08_Lighting::OnUpdate() {
	// 1. Draws light source
	glm::vec3 orbitPosition = glm::vec3(
	    std::cos(spr::runtime::getTime()),
	    std::cos(spr::runtime::getTime()) * std::sin(spr::runtime::getTime()),
	    std::sin(spr::runtime::getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, orbitPosition);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
	spr::submit(m_LightSourceShaderProgram);

	// 2. Draws lit object
	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_ObjectColorUniform, glm::value_ptr(m_CubeColor));
	spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
	spr::submit(m_LitShaderProgram);

	spr::render();
	spr::clean();
}

void LOGL_08_Lighting::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_LitShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_ObjectColorUniform);
}

void LOGL_08_Lighting::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

    ImGui::Begin("08 - Basic Lighting");
    ImGui::ColorPicker3("Object Color", glm::value_ptr(m_CubeColor));
    ImGui::ColorPicker3("Light Color", glm::value_ptr(m_LightColor));
    ImGui::End();


    ImGui::Begin("Info");
    ImGui::TextWrapped("This example contains only solid colors. Object color is multiplied by light color, there are no shadows.");
    ImGui::End();
}
