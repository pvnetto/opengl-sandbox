#include "11_Specular_Map.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/renderer/SimpleRenderer.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_11_SpecularMap::OnAttach() {
	m_camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	std::vector<Texture> textures = {
	    TextureLoader::LoadTexture("../../src/assets/container.png"),
	    TextureLoader::LoadTexture("../../src/assets/container_specular.png")};

	m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
	m_lightSourceMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>());

	m_phongShader = Shader("../../src/shaders/09_specular_map.vert", "../../src/shaders/09_specular_map.frag");
	m_lightSourceShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");
}

void LOGL_11_SpecularMap::OnUpdate() {
	m_controller.HandleKeyboardInput();

	glm::vec3 lightColor{1.f, 0.8f, 0.6f};
	glm::vec3 lightSourcePos = glm::vec3(std::cos(spr::runtime::getTime()),
	                                     std::cos(spr::runtime::getTime()) * std::sin(spr::runtime::getTime()),
	                                     std::sin(spr::runtime::getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", sourceModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", lightColor);
	m_lightSourceMesh.Draw(m_lightSourceShader);

	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.12f, glm::vec3(0.0f, 1.0f, 0.0f));

	m_phongShader.Use();
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());

	m_phongShader.SetInt("material.diffuseMap", 0);
	m_phongShader.SetInt("material.specularMap", 1);
	m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	m_phongShader.SetFloat("material.specularStrength", 1.f);
	m_phongShader.SetFloat("material.shininess", 32.f);

	m_phongShader.SetVector3("light.color", lightColor);
	m_phongShader.SetVector3("light.position", lightSourcePos);
	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());

	m_cubeMesh.Draw(m_phongShader);
}

void LOGL_11_SpecularMap::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(450, 100));

	ImGui::Begin("Info");
	ImGui::TextWrapped("Specular maps determine which pixels will have specular lighting. In this example, "
	                   "the box edges are metallic while the box itself is wood, so only the edges should have specular lighting.");
	ImGui::End();
}

void LOGL_11_SpecularMap::OnEvent(Event &evt) {
	m_controller.HandleEvent(evt);
}
