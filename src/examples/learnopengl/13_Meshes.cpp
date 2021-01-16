#include "13_Meshes.h"

#include "shared/Light.h"
#include "shared/Primitive.h"

#include <GLFW/glfw3.h>

LOGL_13_Meshes::LOGL_13_Meshes() : Layer("Learn OpenGL 13 - Meshes") {
	m_camera = Camera(glm::vec3(-3.8f, 1.3f, 6.2f),
	                  glm::vec3(-8.f, -46.5f, 0.f),
	                  glm::vec3(0.f, 1.f, 0.f));
	m_controller = FreeCameraController(m_camera, 4.5f);
}

LOGL_13_Meshes::~LOGL_13_Meshes() {
}

void LOGL_13_Meshes::OnAttach() {
	std::vector<Texture> textures = {
	    TextureLoader::LoadTexture("../../src/assets/container.png", 0),
	    TextureLoader::LoadTexture("../../src/assets/container_specular.png", 1)};

	m_cubeMesh = Mesh{PrimitiveShape::Cube(), std::vector<unsigned int>(), textures};
	m_backpackModel = Model{"../../src/assets/backpack/backpack.obj"};

	m_phongShader = Shader("../../src/shaders/09_specular_map.vert", "../../src/shaders/12_default.frag");
	m_lightSourceShader = Shader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");
}

void LOGL_13_Meshes::OnUpdate() {
	m_controller.HandleKeyboardInput();

	PointLight pointLight;
	pointLight.Position = {0.0f, 2.0f, 7.f};
	pointLight.Color = {1.f, 0.8f, 0.6f};
	pointLight.ConstantAttenuation = 1.f;
	pointLight.LinearAttenuation = 0.09f;
	pointLight.QuadraticAttenuation = 0.032f;

	glm::mat4 pointLightModel(1.0f);
	pointLightModel = glm::translate(pointLightModel, pointLight.Position);
	pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f, 0.3f, 0.3f));

	m_lightSourceShader.Use();
	m_lightSourceShader.SetMatrix("model", pointLightModel);
	m_lightSourceShader.SetMatrix("view", GetCamera().GetView());
	m_lightSourceShader.SetMatrix("projection", GetCamera().GetProjection());
	m_lightSourceShader.SetVector3("lightColor", pointLight.Color);
	m_cubeMesh.Draw(m_lightSourceShader);

	glm::mat4 model{1.0f};
	model = glm::translate(model, glm::vec3(0.0f, 0.f, 3.f));
	m_phongShader.Use();
	m_phongShader.SetMatrix("model", model);
	m_phongShader.SetMatrix("view", GetCamera().GetView());
	m_phongShader.SetMatrix("projection", GetCamera().GetProjection());
	m_phongShader.SetInt("material.diffuseMap", 0);
	m_phongShader.SetInt("material.specularMap", 1);
	m_phongShader.SetFloat("material.ambientStrength", 0.1f);
	m_phongShader.SetFloat("material.diffuseStrength", 0.5f);
	m_phongShader.SetFloat("material.specularStrength", 1.f);
	m_phongShader.SetFloat("material.shininess", 32.f);

	m_phongShader.SetVector3("pointLight.color", pointLight.Color);
	m_phongShader.SetVector3("pointLight.position", pointLight.Position);
	m_phongShader.SetFloat("pointLight.constantAttenuation", pointLight.ConstantAttenuation);
	m_phongShader.SetFloat("pointLight.linearAttenuation", pointLight.LinearAttenuation);
	m_phongShader.SetFloat("pointLight.quadraticAttenuation", pointLight.QuadraticAttenuation);

	m_phongShader.SetVector3("viewPos", GetCamera().GetPosition());
	m_phongShader.SetFloat("time", (float)glfwGetTime());

	m_backpackModel.Draw(m_phongShader);
}

void LOGL_13_Meshes::OnEvent(Event &evt) {
	m_controller.HandleEvent(evt);
}