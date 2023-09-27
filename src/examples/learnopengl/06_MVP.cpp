#include "06_MVP.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>
#include <spr/SimpleRenderer.h>

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

static spr::SamplerInfo GetDefaultSamplerInfo() {
	static spr::SamplerInfo defaultSampler;
	return defaultSampler;
}

void LOGL_06_MVP::OnAttach() {
	m_CubeVertexBuffer = Utils::LoadCube();

	// 0. Declares the Uniforms in the renderer, so that we can pass data to it later on
	m_TexUniform = spr::createUniform("tex", spr::UniformType::Sampler);
	m_AnotherTexUniform = spr::createUniform("anotherTex", spr::UniformType::Sampler);
	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	
	m_ShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/03_frag_tex.frag");
	m_BrickTexture = Utils::LoadTexture("assets/bricks.jpg");
	m_LadybugTexture = Utils::LoadTexture("assets/yps.png");

	m_Position = glm::vec3(0.5f, 0.2f, -2.0f);
	m_Scale = glm::vec3(1.2f, 1.2f, 1.2f);
	m_CameraPosition = glm::vec3(0, 0, 0);

    m_FieldOfView = 90.f;
}

void LOGL_06_MVP::OnUpdate() {
	// TODO: Move to spr
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. Creates Model matrix
	glm::mat4 model(1.0f);
	model = glm::translate(model, m_Position);
	model = glm::rotate(model, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, m_Scale);

	// 2. Creates View matrix
	glm::mat4 view(1.0f);
	view = glm::translate(view, -m_CameraPosition);

	// 3. Creates Projection matrix
	glm::mat4 projection(1.0f);
	const float aspectRatio = (float) (spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f;
	const float far = 100.f;
	projection = glm::perspective(m_FieldOfView, aspectRatio, near, far);

	// 4. Passes the matrices as Uniforms to the Shader Program
	const int texUnit = 0, anotherTexUnit = 1;
	spr::setUniform(m_TexUniform, &texUnit);
	spr::setUniform(m_AnotherTexUniform, &anotherTexUnit);
	spr::setUniform(m_ModelUniform, glm::value_ptr(model));
	spr::setUniform(m_ViewUniform, glm::value_ptr(view));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(projection));

	spr::setTexture(0, m_BrickTexture, GetDefaultSamplerInfo());
	spr::setTexture(1, m_LadybugTexture, GetDefaultSamplerInfo());

	spr::setVertexBuffer(m_CubeVertexBuffer);

	spr::submit(m_ShaderProgram);

	spr::render();
	spr::clean();
}

void LOGL_06_MVP::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(520, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("06 - MVP");
	if (ImGui::CollapsingHeader("Cube")) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_Position));
		ImGui::InputFloat3("Scale", glm::value_ptr(m_Scale));
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_CameraPosition));
        ImGui::InputFloat("Field of View", &m_FieldOfView);
	}
    ImGui::End();
}

void LOGL_06_MVP::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_BrickTexture);
	spr::destroy(m_LadybugTexture);
	spr::destroy(m_ShaderProgram);
	spr::destroy(m_TexUniform);
	spr::destroy(m_AnotherTexUniform);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
}
