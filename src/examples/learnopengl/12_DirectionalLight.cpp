#include "12_DirectionalLight.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/renderer/SimpleRenderer.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_12_DirectionalLight::OnAttach() {
	m_camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	std::vector<Texture> textures = {
	    TextureLoader::LoadTexture("assets/container.png"),
	    TextureLoader::LoadTexture("assets/container_specular.png")};

	m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
	m_phongShader = Shader("shaders/09_specular_map.vert", "shaders/10_directional_light.frag");
}

void LOGL_12_DirectionalLight::OnUpdate() {
	m_controller.HandleKeyboardInput();

	m_phongShader.Use();

	m_phongShader.SetInt("material.diffuseMap", 0);
	m_phongShader.SetInt("material.specularMap", 1);
	m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	m_phongShader.SetFloat("material.specularStrength", 1.f);
	m_phongShader.SetFloat("material.shininess", 32.f);

	m_phongShader.SetVector3("light.color", m_light.Color);
	m_phongShader.SetVector3("light.direction", glm::normalize(m_light.Direction));
	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());
	m_phongShader.SetFloat("time", spr::runtime::getTime());

	glm::mat4 litModel(1.0f);
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());

	m_cubeMesh.Draw(m_phongShader);
}

void LOGL_12_DirectionalLight::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(300, 300));
    ImGui::Begin("12 - Directional Light");

    ImGui::ColorPicker3("Color", glm::value_ptr(m_light.Color));
    ImGui::Spacing();
    ImGui::SliderFloat3("Direction", glm::value_ptr(m_light.Direction), -1.f, 1.f);

    ImGui::End();
}

void LOGL_12_DirectionalLight::OnEvent(Event &evt) {
	m_controller.HandleEvent(evt);
}
