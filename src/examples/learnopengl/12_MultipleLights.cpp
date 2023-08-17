#include "12_MultipleLights.h"

#include "shared/renderer/SimpleRenderer.h"
#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_MultipleLights::OnAttach() {
	m_Camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);

    m_Spotlight = SpotLight{
	    glm::vec3{0.f, 5.f, 0.f},
	    glm::vec3{0.f, -1.f, 0.f},
	    glm::vec3{0.8f, 0.3f, 0.2f},
	    1.0f, 0.09f, 0.032f, 30.0f, 6.0f};

	m_PointLight = PointLight{
	    glm::vec3{0.0f, 2.0f, 3.f},
	    glm::vec3{1.f, 0.8f, 0.6f},
	    1.f, 0.09f, 0.032f};

	m_DirectionalLights = {
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, 0.3f}), {1.f, 0.8f, 0.6f}},
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, -0.3f}), {0.3f, 0.8f, 0.3f}}};

	//std::vector<Texture> textures = {
	//    TextureLoader::LoadTexture("assets/container.png"),
	//    TextureLoader::LoadTexture("assets/container_specular.png")};

	//m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
	//m_lightSourceMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>());

	//m_phongShader = Shader("shaders/09_specular_map.vert", "shaders/11_multiple_lights.frag");
	//m_lightSourceShader = Shader("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_12_MultipleLights::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	glm::mat4 spotlightModel(1.0f);
	spotlightModel = glm::translate(spotlightModel, m_Spotlight.Position);
	spotlightModel = glm::scale(spotlightModel, glm::vec3(0.3f, 0.3f, 0.3f));

	//m_lightSourceShader.Use();
	//m_lightSourceShader.SetMatrix("model", spotlightModel);
	//m_lightSourceShader.SetMatrix("view", m_Camera.GetView());
	//m_lightSourceShader.SetMatrix("projection", m_Camera.GetProjection());
	//m_lightSourceShader.SetVector3("lightColor", m_Spotlight.Color);
	//m_lightSourceMesh.Draw(m_lightSourceShader);

	glm::mat4 pointLightModel(1.0f);
	pointLightModel = glm::translate(pointLightModel, m_PointLight.Position);
	pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f, 0.3f, 0.3f));

	//m_lightSourceShader.Use();
	//m_lightSourceShader.SetMatrix("model", pointLightModel);
	//m_lightSourceShader.SetMatrix("view", m_Camera.GetView());
	//m_lightSourceShader.SetMatrix("projection", m_Camera.GetProjection());
	//m_lightSourceShader.SetVector3("lightColor", m_PointLight.Color);
	//m_lightSourceMesh.Draw(m_lightSourceShader);

	//m_phongShader.Use();

	//m_phongShader.SetInt("material.diffuseMap", 0);
	//m_phongShader.SetInt("material.specularMap", 1);
	//m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	//m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	//m_phongShader.SetFloat("material.specularStrength", 1.f);
	//m_phongShader.SetFloat("material.shininess", 32.f);

	//m_phongShader.SetVector3("spotlight.color", m_Spotlight.Color);
	//m_phongShader.SetVector3("spotlight.position", m_Spotlight.Position);
	//m_phongShader.SetVector3("spotlight.direction", m_Spotlight.Direction);
	//m_phongShader.SetFloat("spotlight.constantAttenuation", m_Spotlight.ConstantAttenuation);
	//m_phongShader.SetFloat("spotlight.linearAttenuation", m_Spotlight.LinearAttenuation);
	//m_phongShader.SetFloat("spotlight.quadraticAttenuation", m_Spotlight.QuadraticAttenuation);
	//m_phongShader.SetFloat("spotlight.cutoffAngle", glm::radians(m_Spotlight.CutoffAngle));
	//m_phongShader.SetFloat("spotlight.shadowSmoothAngle", glm::radians(m_Spotlight.ShadowSmoothAngle));

	//m_phongShader.SetVector3("pointLight.color", m_PointLight.Color);
	//m_phongShader.SetVector3("pointLight.position", m_PointLight.Position);
	//m_phongShader.SetFloat("pointLight.constantAttenuation", m_PointLight.ConstantAttenuation);
	//m_phongShader.SetFloat("pointLight.linearAttenuation", m_PointLight.LinearAttenuation);
	//m_phongShader.SetFloat("pointLight.quadraticAttenuation", m_PointLight.QuadraticAttenuation);

	for (int i = 0; i < m_DirectionalLights.size(); i++) {
		//m_phongShader.SetVector3("directionalLights[" + std::to_string(i) + "].direction", m_DirectionalLights[i].Direction);
		//m_phongShader.SetVector3("directionalLights[" + std::to_string(i) + "].color", m_DirectionalLights[i].Color);
	}

	//m_phongShader.SetVector3("viewPos", m_Camera.GetPosition());
	//m_phongShader.SetFloat("time", spr::runtime::getTime());

	int boxCount = 25;
	int rowSize = 5;
	float gridSize = 1.2f;
	for (int i = 0; i < boxCount; i++) {
		glm::mat4 litModel(1.0f);
		int rowIdx = (i / rowSize) - (rowSize / 2);
		int colIdx = (i % rowSize) - (rowSize / 2);
		litModel = glm::translate(litModel, glm::vec3(rowIdx * gridSize, 0.f, colIdx * gridSize));
		//m_phongShader.SetMatrix("model", litModel);
		//m_phongShader.SetMatrix("view", m_Camera.GetView());
		//m_phongShader.SetMatrix("projection", m_Camera.GetProjection());
		//m_cubeMesh.Draw(m_phongShader);
	}
}

void LOGL_12_MultipleLights::OnImGuiRender() {

}

void LOGL_12_MultipleLights::OnDetach() {

}

void LOGL_12_MultipleLights::OnEvent(Event &evt) {
    m_Controller.HandleEvent(evt);
}
