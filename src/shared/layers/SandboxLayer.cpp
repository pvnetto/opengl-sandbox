#include "SandboxLayer.h"

#include "shared/RenderUtils.h"

#include <spw/SimpleWindow.h>
#include <spr/SimpleRenderer.h>

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

		const float currentTime = spw::getTime();
		spr::setUniform(m_TimeUniform, &currentTime);

		const auto& resolution = spw::getWindowSize();
		spr::setUniform(m_ResolutionUniform, &resolution);
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
