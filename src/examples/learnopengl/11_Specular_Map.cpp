#include "11_Specular_Map.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_11_SpecularMap::OnAttach() {
	m_Camera = Camera(glm::vec3(-2.8f, 2.f, 2.8f),
	                  glm::vec3(-26.f, -42.f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ContainerTexture = Utils::LoadTexture("assets/container.png");
	m_ContainerSpecularTexture = Utils::LoadTexture("assets/container_specular.png");

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_LightSourceColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);

	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);
	m_LightColorUniform = spr::createUniform("light.color", spr::UniformType::Vec3);
	m_LightPositionUniform = spr::createUniform("light.position", spr::UniformType::Vec3);

	m_MaterialAmbientStrengthUniform = spr::createUniform("material.ambientStrength", spr::UniformType::Float);
	m_MaterialDiffuseStrengthUniform = spr::createUniform("material.diffuseStrength", spr::UniformType::Float);
	m_MaterialSpecularStrengthUniform = spr::createUniform("material.specularStrength", spr::UniformType::Float);
	m_MaterialShininessUniform = spr::createUniform("material.shininess", spr::UniformType::Float);
	m_MaterialDiffuseMapUniform = spr::createUniform("material.diffuseMap", spr::UniformType::Sampler);
	m_MaterialSpecularMapUniform = spr::createUniform("material.specularMap", spr::UniformType::Sampler);

	m_TimeUniform = spr::createUniform("time", spr::UniformType::Float);

	m_SpecularShaderProgram = Utils::LoadShaderProgram("shaders/09_specular_map.vert", "shaders/09_specular_map.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_11_SpecularMap::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	glm::vec3 lightColor{1.f, 0.8f, 0.6f};
	glm::vec3 lightSourcePos = glm::vec3(std::cos(spr::runtime::getTime()),
	                                     std::cos(spr::runtime::getTime()) * std::sin(spr::runtime::getTime()),
	                                     std::sin(spr::runtime::getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(lightColor));
		spr::submit(m_LightSourceShaderProgram);
	}


	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, spr::runtime::getTime() * 0.12f, glm::vec3(0.0f, 1.0f, 0.0f));

	{
		spr::setVertexBuffer(m_CubeVertexBuffer);

		const int containerUnit = 0, specularUnit = 1;
		spr::setTexture(containerUnit, m_ContainerTexture);
		spr::setTexture(specularUnit, m_ContainerSpecularTexture);

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 32.f;
		const float time = spr::runtime::getTime();
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(lightColor));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
		spr::setUniform(m_MaterialDiffuseMapUniform, &containerUnit);
		spr::setUniform(m_MaterialSpecularMapUniform, &specularUnit);
		spr::setUniform(m_MaterialAmbientStrengthUniform, &ambientStrength);
		spr::setUniform(m_MaterialDiffuseStrengthUniform, &diffuseStrength);
		spr::setUniform(m_MaterialSpecularStrengthUniform, &specularStrength);
		spr::setUniform(m_MaterialShininessUniform, &shininess);
		spr::setUniform(m_TimeUniform, &time);

		spr::submit(m_SpecularShaderProgram);
	}

	spr::render();
	spr::clean();
}

void LOGL_11_SpecularMap::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightSourceColorUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_LightColorUniform);
	spr::destroy(m_LightPositionUniform);
	spr::destroy(m_MaterialAmbientStrengthUniform);
	spr::destroy(m_MaterialDiffuseStrengthUniform);
	spr::destroy(m_MaterialSpecularStrengthUniform);
	spr::destroy(m_MaterialShininessUniform);
	spr::destroy(m_MaterialDiffuseMapUniform);
	spr::destroy(m_MaterialSpecularMapUniform);
	spr::destroy(m_TimeUniform);
}

void LOGL_11_SpecularMap::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(450, 100));

	ImGui::Begin("Info");
	ImGui::TextWrapped("Specular maps determine which pixels will have specular lighting. In this example, "
	                   "the box edges are metallic while the box itself is wood, so only the edges should have specular lighting.");
	ImGui::End();
}

void LOGL_11_SpecularMap::OnEvent(Event &evt) {
	m_Controller.HandleEvent(evt);
}
