#include "07_Camera.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>


void LOGL_07_Camera::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	m_CameraRotation = m_Camera.GetRotationEuler();
	m_ProjectionType = 0;

	spr::VertexAttributeLayout layout;
	layout.begin()
	    .add({"inPosition", spr::AttributeType::Float, 3})
	    .add({"inUV", spr::AttributeType::Float, 2})
	    .add({"inNormal", spr::AttributeType::Float, 3}) // Unused in this example
	    .end();

	Utils::PrimitiveData cube = Utils::GetCubeData();
	m_CubeVertexBuffer = spr::createVertexBuffer(cube.Vertices, cube.NumVertices, layout);

	m_TexUniform = spr::createUniform("tex", spr::UniformType::Sampler);
	m_AnotherTexUniform = spr::createUniform("anotherTex", spr::UniformType::Sampler);
	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_ShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/03_frag_tex.frag");
	m_BrickTexture = Utils::LoadTexture("assets/bricks.jpg");
	m_LadybugTexture = Utils::LoadTexture("assets/yps.png");
}

void LOGL_07_Camera::OnUpdate() {
	if(m_ProjectionType == 0) {
		auto [fov, near, far] = m_Perspective;
		m_Camera.SetPerspective(fov, spr::getWindowWidth() / spr::getWindowHeight(), near, far);
	}
	else if(m_ProjectionType == 1) {
		auto [near, far, size] = m_Orthographic;
		m_Camera.SetOrthographic(size, size, near, far);
	}

	m_Camera.SetRotation(m_CameraRotation);

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, (float) spr::runtime::getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, (float) spr::runtime::getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, (float) spr::runtime::getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));

	const int texUnit = 0, anotherTexUnit = 1;
	spr::setUniform(m_TexUniform, &texUnit);
	spr::setUniform(m_AnotherTexUniform, &anotherTexUnit);
	spr::setUniform(m_ModelUniform, glm::value_ptr(model));

	// 0. Passes the camera's View and Projection matrices as Uniforms to the Shader Program
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));

	spr::setTexture(0, m_BrickTexture, {});
	spr::setTexture(1, m_LadybugTexture, {});

	spr::setVertexBuffer(m_CubeVertexBuffer);

	spr::submit(m_ShaderProgram);

	spr::render();
	spr::clean();
}

void LOGL_07_Camera::OnImGuiRender() {
	ImGui::Begin("07 - Camera");
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::InputFloat3("Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::InputFloat3("Rotation", glm::value_ptr(m_CameraRotation));
		ImGui::Separator();
		ImGui::Combo("Projection", &m_ProjectionType,
		             "Perspective\0"
		             "Ortographic\0");

		if(m_ProjectionType == 0) {
			ImGui::InputFloat("FOV", &m_Perspective.FOV);
			ImGui::InputFloat("Near", &m_Perspective.Near);
			ImGui::InputFloat("Far", &m_Perspective.Far);
		}

		if(m_ProjectionType == 1) {
			ImGui::InputFloat("Near", &m_Orthographic.Near);
			ImGui::InputFloat("Far", &m_Orthographic.Far);
			ImGui::InputFloat("Size", &m_Orthographic.Size);
		}
	}
	ImGui::End();
}

void LOGL_07_Camera::OnDetach() {
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
