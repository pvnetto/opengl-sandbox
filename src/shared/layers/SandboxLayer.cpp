#include "SandboxLayer.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

void SandboxLayer::OnAttach() {
    m_Model = Utils::LoadModel("assets/quad.obj");

	m_TimeUniform = spr::createUniform("iTime", spr::UniformType::Float);
	m_ResolutionUniform = spr::createUniform("iResolution", spr::UniformType::Vec2);
	m_ShaderProgram = Utils::LoadShaderProgram("shaders/sandbox_vertex.vert", "shaders/sandbox_fragment.frag");
}

void SandboxLayer::OnUpdate() {
	if (m_Model.Meshes.size() > 0) {
		const auto &mesh = m_Model.Meshes[0];
		spr::setVertexBuffer(mesh.VertexBuffer);
		spr::setIndexBuffer(mesh.IndexBuffer);

		const float currentTime = (float) glfwGetTime();
		spr::setUniform(m_TimeUniform, &currentTime);

		const glm::vec2 resolution = spr::getWindowSize();
		spr::setUniform(m_ResolutionUniform, glm::value_ptr(resolution));
		spr::submit(m_ShaderProgram);
	}
	spr::render();
	spr::clean();
}

void SandboxLayer::OnDetach() {
	Utils::UnloadModel(m_Model);
	spr::destroy(m_TimeUniform);
	spr::destroy(m_ResolutionUniform);
	spr::destroy(m_ShaderProgram);
}

void SandboxLayer::OnImGuiRender() {

}
