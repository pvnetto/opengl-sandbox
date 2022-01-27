#include "06_MVP.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/SimpleRenderer.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_06_MVP::OnAttach() {
	std::vector<Texture> textures{
	    TextureLoader::LoadTexture("../../src/assets/bricks.jpg", 0),
	    TextureLoader::LoadTexture("../../src/assets/yps.png", 1),
	};

	m_cube = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), textures};
	m_shader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/03_frag_tex.frag");

	m_position = glm::vec3(0.5f, 0.2f, -2.0f);
	m_scale = glm::vec3(1.2f, 1.2f, 1.2f);
	m_cameraPosition = glm::vec3(0, 0, 0);

    m_fov = 90.f;
}

void LOGL_06_MVP::OnUpdate() {
	m_shader.Use();
	m_shader.SetInt("tex", 0);
	m_shader.SetInt("anotherTex", 1);

	// 1) Creates model matrix
	glm::mat4 model(1.0f);
	model = glm::translate(model, m_position);
	model = glm::rotate(model, spr::runtime::getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, spr::runtime::getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, spr::runtime::getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, m_scale);

	// 2) Creates view matrix
	glm::mat4 view(1.0f);
	view = glm::translate(view, -m_cameraPosition);

	// 3) Creates projection matrix
	glm::mat4 projection(1.0f);
	float aspectRatio = spr::getWindowWidth() / spr::getWindowHeight(), near = 0.1f, far = 100.f;
	projection = glm::perspective(m_fov, aspectRatio, near, far);

	// 4) Passes MVP matrices as uniforms to shader
	m_shader.SetMatrix("model", model);
	m_shader.SetMatrix("view", view);
	m_shader.SetMatrix("projection", projection);

	m_cube.Draw(m_shader);
}

void LOGL_06_MVP::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(520, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("06 - MVP");
	if (ImGui::CollapsingHeader("Cube")) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_position));
		ImGui::InputFloat3("Scale", glm::value_ptr(m_scale));
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_cameraPosition));
        ImGui::InputFloat("Field of View", &m_fov);
	}
    ImGui::End();
}