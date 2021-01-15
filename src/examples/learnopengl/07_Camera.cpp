#include "07_Camera.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_07_Camera::OnAttach() {
	m_camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	std::vector<Texture> textures{
	    TextureLoader::LoadTexture("../../src/assets/bricks.jpg", 0),
	    TextureLoader::LoadTexture("../../src/assets/yps.png", 1),
	};

	m_cube = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), textures};
	m_shader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/03_frag_tex.frag");
	m_cameraRotation = m_camera.GetRotationEuler();
	m_projectionType = 0;
}

void LOGL_07_Camera::OnUpdate() {
	if(m_projectionType == 0) {
		auto [fov, near, far] = m_perspective;
		m_camera.SetPerspective(fov, Window::Get()->GetWidth() / Window::Get()->GetHeight(), near, far);
	}
	else if(m_projectionType == 1) {
		auto [near, far, size] = m_orthographic;
		m_camera.SetOrthographic(size, size, near, far);
	}

	m_camera.SetRotation(m_cameraRotation);

	m_shader.Use();
	m_shader.SetInt("tex", 0);
	m_shader.SetInt("anotherTex", 1);

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, (float)glfwGetTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));

	m_shader.SetMatrix("model", model);
	m_shader.SetMatrix("view", m_camera.GetView());
	m_shader.SetMatrix("projection", m_camera.GetProjection());

	m_cube.Draw(m_shader);
}

void LOGL_07_Camera::OnImGuiRender() {
	ImGui::Begin("07 - Camera");
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_camera.GetPosition()));
		ImGui::InputFloat3("Rotation", glm::value_ptr(m_cameraRotation));
		ImGui::Separator();
		ImGui::Combo("Projection", &m_projectionType,
		             "Perspective\0"
		             "Ortographic\0");

		if(m_projectionType == 0) {
			ImGui::InputFloat("FOV", &m_perspective.FOV);
			ImGui::InputFloat("Near", &m_perspective.Near);
			ImGui::InputFloat("Far", &m_perspective.Far);
		}

		if(m_projectionType == 1) {
			ImGui::InputFloat("Near", &m_orthographic.Near);
			ImGui::InputFloat("Far", &m_orthographic.Far);
			ImGui::InputFloat("Size", &m_orthographic.Size);
		}
	}
	ImGui::End();
}