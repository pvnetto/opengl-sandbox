#include "03_Instancing.h"

#include "shared/RenderUtils.h"
#include "shared/Utils.h"

#include <spr/OpenGL/Helpers.h>
#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include <iostream>

static std::vector<float> GetColors(int amount) {
	std::vector<float> colors;
	for (int i = 0; i < amount; i++) {
		const float progress = i / (float)amount;
		const float r = glm::mix(0.f, 1.f, progress);
		const float g = 0.f;
		const float b = 0.f;
		const float a = 1.f;
		colors.push_back(r);
		colors.push_back(g);
		colors.push_back(b);
		colors.push_back(a);
	}
	return colors;
}

static std::vector<float> GetModels(int amount) {
	const int rowSize = sqrt(amount);
	const float gridSize = 2.f;
	std::vector<float> models;
	for (int cubeIndex = 0; cubeIndex < amount; cubeIndex++) {
		const int rowIdx = (cubeIndex / rowSize) - (rowSize / 2);
		const int colIdx = (cubeIndex % rowSize) - (rowSize / 2);
		const float rotationAngle = glm::sin((float)cubeIndex) * 360.f;

		const glm::vec3 worldPosition{rowIdx * gridSize, colIdx * gridSize, -10.f};
		glm::mat4 model(1.0f);
		model = glm::translate(model, worldPosition);
		model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		for (int modelIndex = 0; modelIndex < 16; modelIndex++) {
			const int modelRow = modelIndex / 4, modelCol = modelIndex - (modelRow * 4);
			models.push_back(model[modelRow][modelCol]);
		}
	}

	return models;
}

void SPRE_03_Instancing::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	const spw::Vec2 windowSize = spw::getWindowSize();
	const float aspectRatio = (float)windowSize.X / windowSize.Y;
	m_Camera.SetPerspective(90.f, aspectRatio, 0.1f, 100.f);

	m_CubeVertexBuffer = Utils::LoadCube();
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_InstancedShaderProgram = Utils::LoadShaderProgram("shaders/04_aogl_instanced.vert", "shaders/04_aogl_instanced.frag");

	// 0. Creates Instanced Vertex Buffer to store per-instance color data
	const std::vector<float> colors = GetColors(m_CubeCount);
	spr::VertexAttributeLayout colorsLayout;
	colorsLayout.begin()
				.add({ "inColor", spr::AttributeType::Float, 4 })
				.end();
	m_InstancedColorBuffer = spr::createInstancedVertexBuffer(colors.data(), colors.size() * sizeof(float), m_CubeCount, colorsLayout);

	// 1. Creates Instanced Vertex Buffer to store per-instance world positions
	// NOTE: We could have used the same Vertex Buffer that we use for colors, but we created a separate one to make it easier to use different divisors
	const std::vector<float> worldPositions = GetModels(m_CubeCount);
	spr::VertexAttributeLayout modelsLayout;
	modelsLayout.begin()
	    .add({"inModel", spr::AttributeType::Float, 16})
	    .end();
	m_InstancedModelBuffer = spr::createInstancedVertexBuffer(worldPositions.data(), worldPositions.size() * sizeof(float), m_CubeCount, modelsLayout);

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}

void SPRE_03_Instancing::OnUpdate() {
	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setVertexBuffer(m_InstancedColorBuffer);
	spr::setVertexBuffer(m_InstancedModelBuffer);
	spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
	spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
	spr::submit(m_InstancedShaderProgram);
	spr::flush();
}

void SPRE_03_Instancing::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_InstancedColorBuffer);
	spr::destroy(m_InstancedModelBuffer);
	spr::destroy(m_InstancedShaderProgram);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
}
