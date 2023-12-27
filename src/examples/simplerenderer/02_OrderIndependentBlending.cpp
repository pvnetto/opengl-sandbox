#include "02_OrderIndependentBlending.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void SPRE_02_OrderIndependentBlending::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	m_QuadModel = Utils::LoadModel("assets/quad.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_ColorUniform = spr::createUniform("color", spr::UniformType::Vec4);

	m_UnlitShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/default_unlit.frag");

	// TODO: Move to RenderTarget clear config
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
}

static const uint8_t kRenderPassTarget = 0;

void SPRE_02_OrderIndependentBlending::SubmitOpaque() {
	const glm::vec3 opaqueObjectPos = m_Camera.GetPosition() + m_Camera.GetForward() * 5.f;
	glm::mat4 opaqueModel(1.0f);
	opaqueModel = glm::translate(opaqueModel, opaqueObjectPos);
	opaqueModel = glm::rotate(opaqueModel, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	opaqueModel = glm::rotate(opaqueModel, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	opaqueModel = glm::rotate(opaqueModel, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	opaqueModel = glm::scale(opaqueModel, glm::vec3{1.25f, 1.25f, 1.25f});

	spr::FixedFunctionState opaquePassState;
	opaquePassState.SetBlendingEnabled(false);

	const glm::vec4 opaqueColor{1.0f, 1.0f, 1.0f, 1.0f};
	spr::setFixedFunctionState(opaquePassState);
	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setUniform(m_ModelUniform, glm::value_ptr(opaqueModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_ColorUniform, glm::value_ptr(opaqueColor));
	spr::submit(kRenderPassTarget, m_UnlitShaderProgram);
}

void SPRE_02_OrderIndependentBlending::SubmitTransparent() {
	const glm::vec3 transparentObjectPos = m_Camera.GetPosition() + m_Camera.GetForward() * 2.f;
	glm::mat4 transparentModel(1.0f);
	transparentModel = glm::translate(transparentModel, transparentObjectPos);
	transparentModel = glm::scale(transparentModel, glm::vec3(0.3f, 0.3f, 0.3f));

	spr::FixedFunctionState transparentPassState;
	transparentPassState.SetBlendingEnabled(true);

	const glm::vec4 transparentColor{0.8f, 0.2f, 0.85f, 0.35f};
	const auto &quadMesh = m_QuadModel.Meshes[0];
	spr::setFixedFunctionState(transparentPassState);
	spr::setVertexBuffer(quadMesh.VertexBuffer);
	if (quadMesh.IndexBuffer.isValid()) {
		spr::setIndexBuffer(quadMesh.IndexBuffer);
	}
	spr::setUniform(m_ModelUniform, glm::value_ptr(transparentModel));
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::setUniform(m_ColorUniform, glm::value_ptr(transparentColor));
	spr::submit(kRenderPassTarget, m_UnlitShaderProgram);
}

void SPRE_02_OrderIndependentBlending::OnUpdate() {
	int val = std::rand() % 100;
	if (val > 50) {
		SubmitOpaque();
		SubmitTransparent();
	}
	else {
		// Submits transparent object first. If the renderer was order-dependent, that would be wrong, as opaque objects must be
		// drawn first or blending doesn't work
		SubmitTransparent();	
		SubmitOpaque();
	}

	spr::flush();
}

void SPRE_02_OrderIndependentBlending::OnDetach() {
	Utils::UnloadModel(m_QuadModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_UnlitShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_ColorUniform);
}
