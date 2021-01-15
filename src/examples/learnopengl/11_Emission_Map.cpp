#include "11_Emission_Map.h"

#include "shared/Primitive.h"
#include "shared/Texture2D.h"
#include "shared/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_11_EmissionMap::OnAttach() {
	m_camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 5.f);

	m_phongShader = Shader("../../src/shaders/09_specular_map.vert", "../../src/shaders/09_emission_map.frag");
	m_lightSourceShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");

    std::vector<Texture> textures = {
        TextureLoader::LoadTexture("../../src/assets/container.png"),
        TextureLoader::LoadTexture("../../src/assets/container_specular.png"),
        TextureLoader::LoadTexture("../../src/assets/matrix.jpg"),
    };

    m_cubeMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), textures);
    m_lightSourceMesh = Mesh(PrimitiveShape::Cube(), std::vector<unsigned int>(), std::vector<Texture>());

    m_lightColor = glm::vec3{ 1.f, 0.8f, 0.6f };
}

void LOGL_11_EmissionMap::OnUpdate() {
    m_controller.HandleKeyboardInput();

	glm::vec3 lightSourcePos = glm::vec3(std::cos((float)glfwGetTime()),
	                                     std::cos((float)glfwGetTime()) * std::sin((float)glfwGetTime()),
	                                     std::sin((float)glfwGetTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", sourceModel);
	m_lightSourceShader.SetMatrix("view", m_camera.GetView());
	m_lightSourceShader.SetMatrix("projection", m_camera.GetProjection());
	m_lightSourceShader.SetVector3("lightColor", m_lightColor);

    m_lightSourceMesh.Draw(m_lightSourceShader);


	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.12f, glm::vec3(0.0f, 1.0f, 0.0f));

	m_phongShader.Use();
	m_phongShader.SetMatrix("model", litModel);
	m_phongShader.SetMatrix("view", m_camera.GetView());
	m_phongShader.SetMatrix("projection", m_camera.GetProjection());

	m_phongShader.SetInt("material.diffuseMap", 0);
	m_phongShader.SetInt("material.specularMap", 1);
	m_phongShader.SetInt("material.emissionMap", 2);
	m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	m_phongShader.SetFloat("material.specularStrength", 1.f);
	m_phongShader.SetFloat("material.shininess", 32.f);

	m_phongShader.SetVector3("light.color", m_lightColor);
	m_phongShader.SetVector3("light.position", lightSourcePos);
	m_phongShader.SetVector3("viewPos", m_camera.GetPosition());
	m_phongShader.SetFloat("time", (float)glfwGetTime());

    m_cubeMesh.Draw(m_phongShader);
}

void LOGL_11_EmissionMap::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(450, 100));
    
    ImGui::Begin("Info");
    ImGui::TextWrapped("Emission maps will output colors regardless of light conditions, that is, they don't need to be lit to output a color");
    ImGui::End();
}

void LOGL_11_EmissionMap::OnEvent(Event &evt) {
    m_controller.HandleEvent(evt);
}