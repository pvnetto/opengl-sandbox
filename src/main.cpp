#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shared/Model.h"
#include "shared/Camera.h"
#include "shared/FreeCameraController.h"
#include "shared/Primitive.h"
#include "shared/Shader.h"
#include "shared/Light.h"
#include "shared/Window.h"

void OnWindowResize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void HandleWindowInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void UpdateMousePos(GLFWwindow* window, double mouseX, double mouseY);

glm::vec2 mousePos;
bool firstMouseMove = true;

float lastFrame = 0;
float deltaTime = 0;

Camera camera(
	glm::vec3(-3.8f, 1.3f, 6.2f),
	glm::vec3(-8.f, -46.5f, 0.f),
	glm::vec3(0.f, 1.f, 0.f));

FreeCameraController camController(camera, 4.5f);

int main() {
	Window window;

	TextureLoader textureLoader;
	Texture diffuseTex = textureLoader.LoadTexture("../../src/assets/container.png", 0);
	Texture specularTex = textureLoader.LoadTexture("../../src/assets/container_specular.png", 1);

	Mesh cubeMesh { PrimitiveShape::Cube(), std::vector<unsigned int>(), { diffuseTex, specularTex } };
	Model backpackModel { "../../src/assets/backpack/backpack.obj" };

	Shader phongShader("../../src/shaders/09_specular_map.vert", "../../src/shaders/12_default.frag");
	Shader lightSourceShader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");
	
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window.GetNativeWindow(), UpdateMousePos);

	while (!glfwWindowShouldClose(window.GetNativeWindow())) {
		// Calculates delta time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// === Processes inputs ===
		HandleWindowInput(window.GetNativeWindow());
		camController.HandleKeyboardInput(window.GetNativeWindow(), deltaTime);

		// === Rendering ===
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PointLight pointLight;
		pointLight.Position = { 0.0f, 2.0f, 7.f };
		pointLight.Color = { 1.f, 0.8f, 0.6f };
		pointLight.ConstantAttenuation = 1.f;
		pointLight.LinearAttenuation = 0.09f;
		pointLight.QuadraticAttenuation = 0.032f;

		glm::mat4 pointLightModel(1.0f);
		pointLightModel = glm::translate(pointLightModel, pointLight.Position);
		pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f, 0.3f, 0.3f));

		lightSourceShader.Use();
		lightSourceShader.SetMatrix("model", pointLightModel);
		lightSourceShader.SetMatrix("view", camera.GetView());
		lightSourceShader.SetMatrix("projection", camera.GetProjection());
		lightSourceShader.SetVector3("lightColor", pointLight.Color);
		cubeMesh.Draw(lightSourceShader);

		glm::mat4 model {1.0f};
		model = glm::translate(model, glm::vec3(0.0f, 0.f, 3.f));
		phongShader.Use();
		phongShader.SetMatrix("model", model);
		phongShader.SetMatrix("view", camera.GetView());
		phongShader.SetMatrix("projection", camera.GetProjection());
		phongShader.SetInt("material.diffuseMap", 0);
		phongShader.SetInt("material.specularMap", 1);
		phongShader.SetFloat("material.ambientStrength", 0.1f);
		phongShader.SetFloat("material.diffuseStrength", 0.5f);
		phongShader.SetFloat("material.specularStrength", 1.f);
		phongShader.SetFloat("material.shininess", 32.f);

		phongShader.SetVector3("pointLight.color", pointLight.Color);
		phongShader.SetVector3("pointLight.position", pointLight.Position);
		phongShader.SetFloat("pointLight.constantAttenuation", pointLight.ConstantAttenuation);
		phongShader.SetFloat("pointLight.linearAttenuation", pointLight.LinearAttenuation);
		phongShader.SetFloat("pointLight.quadraticAttenuation", pointLight.QuadraticAttenuation);

		phongShader.SetVector3("viewPos", camera.GetPosition());
		phongShader.SetFloat("time", (float) glfwGetTime());

		backpackModel.Draw(phongShader);

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(window.GetNativeWindow());
	}

	glfwTerminate();
	return 0;
}

void UpdateMousePos(GLFWwindow* window, double mouseX, double mouseY) {
	if(firstMouseMove) {
		mousePos = glm::vec2(mouseX, mouseY);
		firstMouseMove = false;
		return;
	}

	glm::vec2 newMousePos(mouseX, mouseY);
	glm::vec2 mouseDelta (newMousePos.x - mousePos.x, mousePos.y - newMousePos.y);
	mousePos = newMousePos;

	camController.HandleMouseInput(window, mouseDelta, deltaTime);
}