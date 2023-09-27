#include "13_Meshes.h"

#include "shared/Light.h"
#include "shared/Runtime.h"

#include <spr/SimpleRenderer.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

LOGL_13_Meshes::LOGL_13_Meshes() : Layer("Learn OpenGL 13 - Meshes") {
	m_Camera = Camera(glm::vec3(-3.8f, 1.3f, 6.2f),
	                  glm::vec3(-8.f, -46.5f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_Controller = FreeCameraController(m_Camera, 4.5f);
}

LOGL_13_Meshes::~LOGL_13_Meshes() {
}

void LOGL_13_Meshes::OnAttach() {
	m_BackpackModel = Utils::LoadModel("assets/backpack/backpack.obj");
	m_CubeVertexBuffer = Utils::LoadCube();

	m_ContainerTexture = Utils::LoadTexture("assets/container.png");
	m_ContainerSpecularTexture = Utils::LoadTexture("assets/container_specular.png");

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_LightSourceColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);

	m_PointLightColorUniform = spr::createUniform("pointLight.color", spr::UniformType::Vec3);
	m_PointLightPositionUniform = spr::createUniform("pointLight.position", spr::UniformType::Vec3);
	m_PointLightConstantAttenuationUniform = spr::createUniform("pointLight.constantAttenuation", spr::UniformType::Float);
	m_PointLightLinearAttenuationUniform = spr::createUniform("pointLight.linearAttenuation", spr::UniformType::Float);
	m_PointLightQuadraticAttenuationUniform = spr::createUniform("pointLight.quadraticAttenuation", spr::UniformType::Float);

	m_MaterialAmbientStrengthUniform = spr::createUniform("material.ambientStrength", spr::UniformType::Float);
	m_MaterialDiffuseStrengthUniform = spr::createUniform("material.diffuseStrength", spr::UniformType::Float);
	m_MaterialSpecularStrengthUniform = spr::createUniform("material.specularStrength", spr::UniformType::Float);
	m_MaterialShininessUniform = spr::createUniform("material.shininess", spr::UniformType::Float);
	m_MaterialDiffuseMapUniform = spr::createUniform("material.diffuseMap", spr::UniformType::Sampler);
	m_MaterialSpecularMapUniform = spr::createUniform("material.specularMap", spr::UniformType::Sampler);

	m_TimeUniform = spr::createUniform("time", spr::UniformType::Float);

	m_PhongShaderProgram = Utils::LoadShaderProgram("shaders/09_specular_map.vert", "shaders/12_default.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_13_Meshes::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	PointLight pointLight;
	pointLight.Position = {0.0f, 2.0f, 7.f};
	pointLight.Color = {1.f, 0.8f, 0.6f};
	pointLight.ConstantAttenuation = 1.f;
	pointLight.LinearAttenuation = 0.09f;
	pointLight.QuadraticAttenuation = 0.032f;

	glm::mat4 pointLightModel(1.0f);
	pointLightModel = glm::translate(pointLightModel, pointLight.Position);
	pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f, 0.3f, 0.3f));

	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(pointLightModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(pointLight.Color));
		spr::submit(m_LightSourceShaderProgram);
	}


	glm::mat4 model{1.0f};
	model = glm::translate(model, glm::vec3(0.0f, 0.f, 3.f));
	for (const auto &mesh : m_BackpackModel.Meshes) {
		spr::setVertexBuffer(mesh.VertexBuffer);
		spr::setIndexBuffer(mesh.IndexBuffer);
		
		if (mesh.Textures.size() > 0) {
			spr::setTexture(0, mesh.Textures[0]);
		}
		if (mesh.Textures.size() > 1) {
			spr::setTexture(0, mesh.Textures[1]);
		}

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 32.f;
		const float time = Runtime::get()->getTime();
		spr::setUniform(m_ModelUniform, glm::value_ptr(model));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));
		spr::setUniform(m_PointLightColorUniform, glm::value_ptr(pointLight.Color));
		spr::setUniform(m_PointLightPositionUniform, glm::value_ptr(pointLight.Position));
		spr::setUniform(m_PointLightConstantAttenuationUniform, &pointLight.ConstantAttenuation);
		spr::setUniform(m_PointLightLinearAttenuationUniform, &pointLight.LinearAttenuation);
		spr::setUniform(m_PointLightQuadraticAttenuationUniform, &pointLight.QuadraticAttenuation);
		spr::setUniform(m_MaterialAmbientStrengthUniform, &ambientStrength);
		spr::setUniform(m_MaterialDiffuseStrengthUniform, &diffuseStrength);
		spr::setUniform(m_MaterialSpecularStrengthUniform, &specularStrength);
		spr::setUniform(m_MaterialShininessUniform, &shininess);
		spr::setUniform(m_TimeUniform, &time);

		spr::submit(m_PhongShaderProgram);
	}

	spr::render();
	spr::clean();
}

void LOGL_13_Meshes::OnDetach() {
	Utils::UnloadModel(m_BackpackModel);
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_PhongShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ContainerTexture);
	spr::destroy(m_ContainerSpecularTexture);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightSourceColorUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_PointLightColorUniform);
	spr::destroy(m_PointLightPositionUniform);
	spr::destroy(m_PointLightConstantAttenuationUniform);
	spr::destroy(m_PointLightLinearAttenuationUniform);
	spr::destroy(m_PointLightQuadraticAttenuationUniform);
	spr::destroy(m_MaterialDiffuseMapUniform);
	spr::destroy(m_MaterialSpecularMapUniform);
	spr::destroy(m_MaterialAmbientStrengthUniform);
	spr::destroy(m_MaterialDiffuseStrengthUniform);
	spr::destroy(m_MaterialSpecularStrengthUniform);
	spr::destroy(m_MaterialShininessUniform);
	spr::destroy(m_TimeUniform);
}

void LOGL_13_Meshes::OnEvent(const Event&evt) {
	m_Controller.HandleEvent(evt);
}
