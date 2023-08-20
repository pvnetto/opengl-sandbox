#include "12_MultipleLights.h"

#include "shared/RenderUtils.h"
#include "shared/renderer/SimpleRenderer.h"

#include <iostream>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

static constexpr int s_MaxNumDirectionalLights = 4;			// This should match the value specified in the fragment shader

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
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, 0.3f}), {0.1f, 0.1f, 0.5f}},
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, -0.3f}), {0.1f, 0.5f, 0.1f}},
	    DirectionalLight{glm::normalize(glm::vec3{0.0f, -0.5f, -0.3f}), {0.1f, 0.1f, 0.1f}},
	};

	m_CubeVertexBuffer = Utils::LoadCube();

	m_ContainerTexture = Utils::LoadTexture("assets/container.png");
	m_ContainerSpecularTexture = Utils::LoadTexture("assets/container_specular.png");

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);

	m_LightSourceColorUniform = spr::createUniform("lightColor", spr::UniformType::Vec3);
	m_ViewPositionUniform = spr::createUniform("viewPos", spr::UniformType::Vec3);

	m_EnabledDirectionalLightsUniform = spr::createUniform("enabledDirectionalLights", spr::UniformType::Integer);
	m_DirectionalLightDirectionUniforms = spr::createUniform("directionalLights.direction", spr::UniformType::Vec3, s_MaxNumDirectionalLights);
	m_DirectionalLightColorUniforms = spr::createUniform("directionalLights.color", spr::UniformType::Vec3, s_MaxNumDirectionalLights);

	m_PointLightColorUniform = spr::createUniform("pointLight.color", spr::UniformType::Vec3);
	m_PointLightPositionUniform = spr::createUniform("pointLight.position", spr::UniformType::Vec3);
	m_PointLightConstantAttenuationUniform = spr::createUniform("pointLight.constantAttenuation", spr::UniformType::Float);
	m_PointLightLinearAttenuationUniform = spr::createUniform("pointLight.linearAttenuation", spr::UniformType::Float);
	m_PointLightQuadraticAttenuationUniform = spr::createUniform("pointLight.quadraticAttenuation", spr::UniformType::Float);

	m_SpotLightColorUniform = spr::createUniform("spotlight.color", spr::UniformType::Vec3);
	m_SpotLightPositionUniform = spr::createUniform("spotlight.position", spr::UniformType::Vec3);
	m_SpotLightDirectionUniform = spr::createUniform("spotlight.direction", spr::UniformType::Vec3);
	m_SpotLightCutoffAngleUniform = spr::createUniform("spotlight.cutoffAngle", spr::UniformType::Float);
	m_SpotLightShadowSmoothAngleUniform = spr::createUniform("spotlight.shadowSmoothAngle", spr::UniformType::Float);
	m_SpotLightConstantAttenuationUniform = spr::createUniform("spotlight.constantAttenuation", spr::UniformType::Float);
	m_SpotLightLinearAttenuationUniform = spr::createUniform("spotlight.linearAttenuation", spr::UniformType::Float);
	m_SpotLightQuadraticAttenuationUniform = spr::createUniform("spotlight.quadraticAttenuation", spr::UniformType::Float);

	m_MaterialAmbientStrengthUniform = spr::createUniform("material.ambientStrength", spr::UniformType::Float);
	m_MaterialDiffuseStrengthUniform = spr::createUniform("material.diffuseStrength", spr::UniformType::Float);
	m_MaterialSpecularStrengthUniform = spr::createUniform("material.specularStrength", spr::UniformType::Float);
	m_MaterialShininessUniform = spr::createUniform("material.shininess", spr::UniformType::Float);
	m_MaterialDiffuseMapUniform = spr::createUniform("material.diffuseMap", spr::UniformType::Sampler);
	m_MaterialSpecularMapUniform = spr::createUniform("material.specularMap", spr::UniformType::Sampler);

	m_TimeUniform = spr::createUniform("time", spr::UniformType::Float);

	m_LightingShaderProgram = Utils::LoadShaderProgram("shaders/09_specular_map.vert", "shaders/11_multiple_lights.frag");
	m_LightSourceShaderProgram = Utils::LoadShaderProgram("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
}

void LOGL_12_MultipleLights::OnUpdate() {
	m_Controller.HandleKeyboardInput();

	glm::mat4 spotlightModel(1.0f);
	spotlightModel = glm::translate(spotlightModel, m_Spotlight.Position);
	spotlightModel = glm::scale(spotlightModel, glm::vec3(0.3f, 0.3f, 0.3f));
	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(spotlightModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(m_Spotlight.Color));
		spr::submit(m_LightSourceShaderProgram);
	}


	glm::mat4 pointLightModel(1.0f);
	pointLightModel = glm::translate(pointLightModel, m_PointLight.Position);
	pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f, 0.3f, 0.3f));
	{
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(pointLightModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::setUniform(m_LightSourceColorUniform, glm::value_ptr(m_PointLight.Color));
		spr::submit(m_LightSourceShaderProgram);
	}


	std::vector<float> directionalLightDirections;
	std::vector<float> directionalLightColors;
	for (int i = 0; i < m_DirectionalLights.size(); i++) {
		directionalLightDirections.push_back(m_DirectionalLights[i].Direction.x);
		directionalLightDirections.push_back(m_DirectionalLights[i].Direction.y);
		directionalLightDirections.push_back(m_DirectionalLights[i].Direction.z);
		directionalLightColors.push_back(m_DirectionalLights[i].Color.r);
		directionalLightColors.push_back(m_DirectionalLights[i].Color.g);
		directionalLightColors.push_back(m_DirectionalLights[i].Color.b);
	}

	{
		const int containerUnit = 0, specularUnit = 1;
		spr::setTexture(containerUnit, m_ContainerTexture);
		spr::setTexture(specularUnit, m_ContainerSpecularTexture);

		const float ambientStrength = 0.1f, diffuseStrength = 0.5f, specularStrength = 1.f, shininess = 32.f;
		spr::setUniform(m_MaterialDiffuseMapUniform, &containerUnit);
		spr::setUniform(m_MaterialSpecularMapUniform, &specularUnit);
		spr::setUniform(m_MaterialAmbientStrengthUniform, &ambientStrength);
		spr::setUniform(m_MaterialDiffuseStrengthUniform, &diffuseStrength);
		spr::setUniform(m_MaterialSpecularStrengthUniform, &specularStrength);
		spr::setUniform(m_MaterialShininessUniform, &shininess);

		spr::setUniform(m_ViewPositionUniform, glm::value_ptr(m_Camera.GetPosition()));

		const std::size_t numActiveLights = m_DirectionalLights.size();
		spr::setUniform(m_EnabledDirectionalLightsUniform, &numActiveLights);
		spr::setUniform(m_DirectionalLightDirectionUniforms, directionalLightDirections.data(), numActiveLights);
		spr::setUniform(m_DirectionalLightColorUniforms, directionalLightColors.data(), numActiveLights);

		spr::setUniform(m_PointLightColorUniform, glm::value_ptr(m_PointLight.Color));
		spr::setUniform(m_PointLightPositionUniform, glm::value_ptr(m_PointLight.Position));
		spr::setUniform(m_PointLightConstantAttenuationUniform, &m_PointLight.ConstantAttenuation);
		spr::setUniform(m_PointLightLinearAttenuationUniform, &m_PointLight.LinearAttenuation);
		spr::setUniform(m_PointLightQuadraticAttenuationUniform, &m_PointLight.QuadraticAttenuation);

		const float cutoffAngleRadians = glm::radians(m_Spotlight.CutoffAngle);
		const float shadowSmoothAngleRadians = glm::radians(m_Spotlight.ShadowSmoothAngle);
		spr::setUniform(m_SpotLightColorUniform, glm::value_ptr(m_Spotlight.Color));
		spr::setUniform(m_SpotLightPositionUniform, glm::value_ptr(m_Spotlight.Position));
		spr::setUniform(m_SpotLightDirectionUniform, glm::value_ptr(m_Spotlight.Direction));
		spr::setUniform(m_SpotLightCutoffAngleUniform, &cutoffAngleRadians);
		spr::setUniform(m_SpotLightShadowSmoothAngleUniform, &shadowSmoothAngleRadians);
		spr::setUniform(m_SpotLightConstantAttenuationUniform, &m_Spotlight.ConstantAttenuation);
		spr::setUniform(m_SpotLightLinearAttenuationUniform, &m_Spotlight.LinearAttenuation);
		spr::setUniform(m_SpotLightQuadraticAttenuationUniform, &m_Spotlight.QuadraticAttenuation);

		const float time = spr::runtime::getTime();
		spr::setUniform(m_TimeUniform, &time);
	}

	int boxCount = 25;
	int rowSize = 5;
	float gridSize = 1.2f;
	for (int i = 0; i < boxCount; i++) {
		glm::mat4 litModel(1.0f);
		int rowIdx = (i / rowSize) - (rowSize / 2);
		int colIdx = (i % rowSize) - (rowSize / 2);
		litModel = glm::translate(litModel, glm::vec3(rowIdx * gridSize, 0.f, colIdx * gridSize));
		spr::setVertexBuffer(m_CubeVertexBuffer);
		spr::setUniform(m_ModelUniform, glm::value_ptr(litModel));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_Camera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_Camera.GetProjection()));
		spr::submit(m_LightingShaderProgram);
	}

	spr::render();
	spr::clean();
}

void LOGL_12_MultipleLights::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_LightingShaderProgram);
	spr::destroy(m_LightSourceShaderProgram);
	spr::destroy(m_ContainerTexture);
	spr::destroy(m_ContainerSpecularTexture);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_LightSourceColorUniform);
	spr::destroy(m_ViewPositionUniform);
	spr::destroy(m_DirectionalLightDirectionUniforms);
	spr::destroy(m_DirectionalLightColorUniforms);
	spr::destroy(m_PointLightColorUniform);
	spr::destroy(m_PointLightPositionUniform);
	spr::destroy(m_PointLightConstantAttenuationUniform);
	spr::destroy(m_PointLightLinearAttenuationUniform);
	spr::destroy(m_PointLightQuadraticAttenuationUniform);
	spr::destroy(m_SpotLightColorUniform);
	spr::destroy(m_SpotLightPositionUniform);
	spr::destroy(m_SpotLightDirectionUniform);
	spr::destroy(m_SpotLightCutoffAngleUniform);
	spr::destroy(m_SpotLightShadowSmoothAngleUniform);
	spr::destroy(m_SpotLightConstantAttenuationUniform);
	spr::destroy(m_SpotLightLinearAttenuationUniform);
	spr::destroy(m_SpotLightQuadraticAttenuationUniform);
	spr::destroy(m_MaterialDiffuseMapUniform);
	spr::destroy(m_MaterialSpecularMapUniform);
	spr::destroy(m_MaterialAmbientStrengthUniform);
	spr::destroy(m_MaterialDiffuseStrengthUniform);
	spr::destroy(m_MaterialSpecularStrengthUniform);
	spr::destroy(m_MaterialShininessUniform);
	spr::destroy(m_TimeUniform);
}

void LOGL_12_MultipleLights::OnImGuiRender() {
}

void LOGL_12_MultipleLights::OnEvent(Event &evt) {
    m_Controller.HandleEvent(evt);
}
