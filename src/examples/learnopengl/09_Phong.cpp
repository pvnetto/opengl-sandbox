#include "09_Phong.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_09_Phong::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

    m_ObjectColor = glm::vec3{ 0.2f, 0.35f, 1.f };
    m_LightColor = glm::vec3{ 0.8f, 0.8f, 1.f };

    m_Controller = FreeCameraController(m_Camera, 5.f);

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);
	m_LightColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_LightPositionUniform = spr::createUniform("lightPos", spr::UniformType::Vec3);
	m_ObjectColorUniform = spr::createUniform("objectColor", spr::UniformType::Vec3);

	m_PhongShaderProgram = Utils::LoadShaderProgram("shaders/07_phong.vert", "shaders/07_phong.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_09_Phong::OnUpdate() {
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
	spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
	spr::submit(m_LightSourceShaderProgram);


	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
	spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
	spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
	spr::setUniform(m_ObjectColorUniform, glm::value_ptr(m_ObjectColor));
	spr::submit(m_PhongShaderProgram);


	spr::render();
	spr::clean();
}

void LOGL_09_Phong::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_PhongShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_ObjectColorUniform);
}

void LOGL_09_Phong::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("09 - Phong Shading");

    const auto flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueWheel;
    ImGui::ColorPicker3("Object Color", glm::value_ptr(m_ObjectColor), flags);
    ImGui::Spacing();
    ImGui::ColorPicker3("Light Color", glm::value_ptr(m_LightColor), flags);

    ImGui::End();
}

void LOGL_09_Phong::OnEvent(Event& evt) {
    m_Controller.HandleEvent(evt);
}
