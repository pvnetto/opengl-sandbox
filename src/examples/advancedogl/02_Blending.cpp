#include "02_Blending.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#define MAKE_RGBA(r, g, b, a) glm::vec4 { r / 255.f, g / 255.f, b / 255.f, a / 255.f }

void AOGL_02_Blending::OnAttach() {
	m_Position = glm::vec3(0.f, 0.f, -5.0f);
	m_Scale = glm::vec3(1.5f, 1.5f, 1.5f);
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 0.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	OrthographicProps orthographicProperties;
	orthographicProperties.Size = 6;
	auto [near, far, size] = orthographicProperties;
	const spw::Vec2 windowSize = spw::getWindowSize();
	const float aspectRatio = (float) windowSize.X / windowSize.Y;
	m_Camera.SetOrthographic(size * aspectRatio, size, near, far);

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_ColorUniform = spr::createUniform("color", spr::UniformType::Vec4);
	m_DefaultShaderProgram = Utils::LoadShaderProgram("shaders/default_unlit.vert", "shaders/default_unlit.frag");

	glClearColor(0xaa, 0xaa, 0xaa, 0xff);
}

void AOGL_02_Blending::OnUpdate() {
	// 0. Renders a bunch of cubes at different depths and draws their depth values
	glm::mat4 projection(1.0f);
	const float aspectRatio = (float)(spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f, far = 1000.f;
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));

	// 1. Renders Opaque pass
	const float offsetMagnitude = 5.0f;
	auto renderOpaquePass = [this, offsetMagnitude]() {
		glDisable(GL_BLEND);
		const float zPosition = m_OpaquePosition == OpaquePosition_Front ? 0.5f : -0.5f;
		const glm::vec3 offset{
		    glm::cos(Runtime::get()->getTime()),
		    glm::sin(Runtime::get()->getTime()), zPosition};

		glm::mat4 model(1.0f);
		model = glm::translate(model, m_Position + offset * offsetMagnitude);
		model = glm::scale(model, m_Scale);

		const glm::vec4 color{0.12f, 0.12f, 0.12f, 1.f};
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(model));
		spr::setUniform(m_ColorUniform, glm::value_ptr(color));
		spr::submit(m_DefaultShaderProgram);
		spr::flush();
	};

	if (m_OpaqueFirst) {
		renderOpaquePass();
	}

	// 2. Transparency pass
	static constexpr int cubeCount = 10;
	const glm::vec4 colors[cubeCount] = {
	    MAKE_RGBA(249, 180, 182, 50),
	    MAKE_RGBA(193, 179, 215, 50),
	    MAKE_RGBA(117, 137, 191, 50),
	    MAKE_RGBA(72, 181, 163, 50),
	    MAKE_RGBA(255, 250, 129, 50),
	    MAKE_RGBA(0xcf, 0xc6, 0xd9, 50),
	    MAKE_RGBA(0xf1, 0x71, 0x40, 50),
	    MAKE_RGBA(0xd6, 0xe2, 0x86, 50),
	    MAKE_RGBA(0xdf, 0xac, 0xf6, 50),
	    MAKE_RGBA(0xcc, 0xed, 0xff, 50),
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < cubeCount; i++) {
		float progress = (float) i / (float) cubeCount;
		float angleRad = glm::radians(progress * 360.f);
		
		const glm::vec3 offset {
			glm::cos(angleRad),
			glm::sin(angleRad), 0.f};

		glm::mat4 model(1.0f);
		model = glm::translate(model, m_Position + offset * offsetMagnitude);
		model = glm::scale(model, m_Scale);

		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(model));
		spr::setUniform(m_ColorUniform, glm::value_ptr(colors[i]));
		spr::submit(m_DefaultShaderProgram);
	}
	spr::flush();

	// (OPTIONAL) Rendering Opaque objects first without ordering the draw calls by depth is usually wrong, but we're doing
	// it here for demonstration purposes.
	if (!m_OpaqueFirst) {
		renderOpaquePass();
	}
}

void AOGL_02_Blending::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_DefaultShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ColorUniform);
}

void AOGL_02_Blending::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));
	ImGui::Begin("02 - Blending");

	ImGui::Checkbox("Render Opaque first", &m_OpaqueFirst);
	ImGui::Spacing();
	ImGui::Combo("Opaque Position", &m_OpaquePosition,
	             "Front\0"
	             "Behind\0");

	ImGui::End();
}
