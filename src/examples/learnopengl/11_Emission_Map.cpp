#include "11_Emission_Map.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spr/SimpleRenderer.h>
#include <spw/SimpleWindow.h>

#include <imgui.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

void LOGL_11_EmissionMap::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 5.f);
    m_LightColor = glm::vec3{ 1.f, 0.8f, 0.6f };

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ContainerTexture = Utils::LoadTexture("assets/container.png");
	m_ContainerSpecularTexture = Utils::LoadTexture("assets/container_specular.png");
	m_MatrixTexture = Utils::LoadTexture("assets/matrix.jpg");

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
	m_MaterialEmissionMapUniform = spr::createUniform("material.emissionMap", spr::UniformType::Sampler);

	m_TimeUniform = spr::createUniform("time", spr::UniformType::Float);

	m_EmissiveShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/09_emission_map.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");

}

void LOGL_11_EmissionMap::OnUpdate() {
    m_Controller.HandleKeyboardInput();

	glm::vec3 lightSourcePos = glm::vec3(std::cos(Runtime::get()->getTime()),
	                                     std::cos(Runtime::get()->getTime()) * std::sin(Runtime::get()->getTime()),
	                                     std::sin(Runtime::get()->getTime()) * 2.f);

	glm::mat4 sourceModel(1.0f);
	sourceModel = glm::translate(sourceModel, lightSourcePos);
	sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));


	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(sourceModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(m_LightColor));
		spr::submit(m_LightSourceShaderProgram);
	}


	glm::mat4 litModel(1.0f);
	litModel = glm::rotate(litModel, Runtime::get()->getTime() * 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
	litModel = glm::rotate(litModel, Runtime::get()->getTime() * 0.12f, glm::vec3(0.0f, 1.0f, 0.0f));


	{
		spr::setVertexBuffer(m_CubeVertexBuffer);

		const int containerUnit = 0, specularUnit = 1, matrixUnit = 2;
		spr::setTexture(containerUnit, m_ContainerTexture);
		spr::setTexture(specularUnit, m_ContainerSpecularTexture);
		spr::setTexture(matrixUnit, m_MatrixTexture);

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 12.f;
		const float time = Runtime::get()->getTime();
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_LightColorUniform, glm::value_ptr(m_LightColor));
		spr::setUniform(m_LightPositionUniform, glm::value_ptr(lightSourcePos));
		spr::setUniform(m_MaterialDiffuseMapUniform, &containerUnit);
		spr::setUniform(m_MaterialSpecularMapUniform, &specularUnit);
		spr::setUniform(m_MaterialEmissionMapUniform, &matrixUnit);
		spr::setUniform(m_MaterialAmbientStrengthUniform, &ambientStrength);
		spr::setUniform(m_MaterialDiffuseStrengthUniform, &diffuseStrength);
		spr::setUniform(m_MaterialSpecularStrengthUniform, &specularStrength);
		spr::setUniform(m_MaterialShininessUniform, &shininess);
		spr::setUniform(m_TimeUniform, &time);

		spr::submit(m_EmissiveShaderProgram);
	}

	spr::render();
	spr::clean();

}

void LOGL_11_EmissionMap::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_EmissiveShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ContainerTexture);
	spr::destroy(m_ContainerSpecularTexture);
	spr::destroy(m_MatrixTexture);
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
	spr::destroy(m_MaterialEmissionMapUniform);
	spr::destroy(m_TimeUniform);
}

void LOGL_11_EmissionMap::OnImGuiRender() {
    ImGui::SetNextWindowSize(ImVec2(450, 100));
    
    ImGui::Begin("Info");
    ImGui::TextWrapped("Emission maps will output colors regardless of light conditions, that is, they don't need to be lit to output a color");
    ImGui::End();
}

void LOGL_11_EmissionMap::OnEvent(const Event&evt) {
    m_Controller.HandleEvent(evt);
}
