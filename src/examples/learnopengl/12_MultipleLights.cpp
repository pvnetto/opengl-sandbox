#include "12_MultipleLights.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_MultipleLights::OnAttach() {
	m_camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	std::vector<Texture> textures = {
	    TextureLoader::LoadTexture("../../src/assets/container.png"),
	    TextureLoader::LoadTexture("../../src/assets/container_specular.png")};

	m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
	m_lightSourceMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>());

	m_phongShader = Shader("../../src/shaders/09_specular_map.vert", "../../src/shaders/11_multiple_lights.frag");
	m_lightSourceShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");

    m_spotlight = SpotLight {
        glm::vec3{ 0.f, 5.f, 0.f },
        glm::vec3{ 0.f, -1.f, 0.f },
        glm::vec3{ 0.8f, 0.3f, 0.2f },
        1.0f, 0.09f, 0.032f, 30.0f, 6.0f 
    };

    m_pointLight = PointLight {
        glm::vec3{ 0.0f, 2.0f, 3.f },
        glm::vec3{ 1.f, 0.8f, 0.6f },
        1.f, 0.09f, 0.032f
    };

	m_directionalLights = {
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, 0.3f}), {1.f, 0.8f, 0.6f}},
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, -0.3f}), {0.3f, 0.8f, 0.3f}}};
}

void LOGL_12_MultipleLights::OnUpdate() {
	m_controller.HandleKeyboardInput();

	glm::mat4 spotlightModel(1.0f);
	spotlightModel = glm::translate(spotlightModel, m_spotlight.Position);
	spotlightModel = glm::scale(spotlightModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", spotlightModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", m_spotlight.Color);
	m_lightSourceMesh.Draw(m_lightSourceShader);

	glm::mat4 pointLightModel(1.0f);
	pointLightModel = glm::translate(pointLightModel, m_pointLight.Position);
	pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", pointLightModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", m_pointLight.Color);
	m_lightSourceMesh.Draw(m_lightSourceShader);

	m_phongShader.Use();

	m_phongShader.SetInt("material.diffuseMap", 0);
	m_phongShader.SetInt("material.specularMap", 1);
	m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	m_phongShader.SetFloat("material.specularStrength", 1.f);
	m_phongShader.SetFloat("material.shininess", 32.f);

	m_phongShader.SetVector3("spotlight.color", m_spotlight.Color);
	m_phongShader.SetVector3("spotlight.position", m_spotlight.Position);
	m_phongShader.SetVector3("spotlight.direction", m_spotlight.Direction);
	m_phongShader.SetFloat("spotlight.constantAttenuation", m_spotlight.ConstantAttenuation);
	m_phongShader.SetFloat("spotlight.linearAttenuation", m_spotlight.LinearAttenuation);
	m_phongShader.SetFloat("spotlight.quadraticAttenuation", m_spotlight.QuadraticAttenuation);
	m_phongShader.SetFloat("spotlight.cutoffAngle", glm::radians(m_spotlight.CutoffAngle));
	m_phongShader.SetFloat("spotlight.shadowSmoothAngle", glm::radians(m_spotlight.ShadowSmoothAngle));

	m_phongShader.SetVector3("pointLight.color", m_pointLight.Color);
	m_phongShader.SetVector3("pointLight.position", m_pointLight.Position);
	m_phongShader.SetFloat("pointLight.constantAttenuation", m_pointLight.ConstantAttenuation);
	m_phongShader.SetFloat("pointLight.linearAttenuation", m_pointLight.LinearAttenuation);
	m_phongShader.SetFloat("pointLight.quadraticAttenuation", m_pointLight.QuadraticAttenuation);

	for (int i = 0; i < m_directionalLights.size(); i++) {
		m_phongShader.SetVector3("directionalLights[" + std::to_string(i) + "].direction", m_directionalLights[i].Direction);
		m_phongShader.SetVector3("directionalLights[" + std::to_string(i) + "].color", m_directionalLights[i].Color);
	}

	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());
	m_phongShader.SetFloat("time", (float)glfwGetTime());

	int boxCount = 25;
	int rowSize = 5;
	float gridSize = 1.2f;
	for (int i = 0; i < boxCount; i++) {
		glm::mat4 litModel(1.0f);
		int rowIdx = (i / rowSize) - (rowSize / 2);
		int colIdx = (i % rowSize) - (rowSize / 2);
		litModel = glm::translate(litModel, glm::vec3(rowIdx * gridSize, 0.f, colIdx * gridSize));
		m_phongShader.SetMatrix("model", litModel);
		m_phongShader.SetMatrix("view", m_camera.GetView());
		m_phongShader.SetMatrix("projection", m_camera.GetProjection());
		m_cubeMesh.Draw(m_phongShader);
	}
}

void LOGL_12_MultipleLights::OnImGuiRender() {

}

void LOGL_12_MultipleLights::OnEvent(Event &evt) {
    m_controller.HandleEvent(evt);
}
