#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shared/Texture2D.h"
#include "shared/Camera.h"
#include "shared/FreeCameraController.h"
#include "shared/Primitive.h"
#include "shared/Shader.h"

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
	glm::vec3(-4.6f, 6.7f, 5.12f),
	glm::vec3(-41.f, -49.f, 0.f),
	glm::vec3(0.f, 1.f, 0.f));

FreeCameraController camController(camera, 4.5f);

struct DirectionalLight {
	glm::vec3 Direction;
	glm::vec3 Color;

	DirectionalLight(glm::vec3 direction, glm::vec3 color)
	    : Direction(direction),
	      Color(color) {}
};

struct PointLight {
	glm::vec3 Position;
	glm::vec3 Color;

	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;

	PointLight() { }

	PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic)
	    : Position(position),
	      Color(color),
	      ConstantAttenuation(constant),
	      LinearAttenuation(linear),
	      QuadraticAttenuation(quadratic) {}
};

struct SpotLight {
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Color;

	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;

	float CutoffAngle;
	float ShadowSmoothAngle;

	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float constant, float linear, float quadratic, float cutoff, float smoothing)
	    : Position(position),
		  Direction(direction),
	      Color(color),
	      ConstantAttenuation(constant),
	      LinearAttenuation(linear),
	      QuadraticAttenuation(quadratic),
		  CutoffAngle(cutoff),
		  ShadowSmoothAngle(smoothing) {}
};

int main() {
	int width = 800, height = 600;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *m_window = glfwCreateWindow(width, height, "OpenGL Sandbox", NULL, NULL);
	if (!m_window) {
		std::cout << "Failed to create an OpenGL window :(\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, OnWindowResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize Glad :(\n";
		return -1;
	}

	glViewport(0, 0, width, height);

	TextureLoader textureLoader;
	Texture diffuseTex = textureLoader.LoadTexture("../../src/assets/container.png", 0);
	Texture specularTex = textureLoader.LoadTexture("../../src/assets/container_specular.png", 1);
	Mesh mesh{ PrimitiveShape::Cube(), std::vector<unsigned int>(), { diffuseTex, specularTex } };

	Shader phongShader("../../src/shaders/09_specular_map.vert", "../../src/shaders/12_default.frag");
	Shader lightSourceShader("../../src/shaders/05_vertex_mvp.vert", "../../src/shaders/06_frag_light_source.frag");

	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPosCallback(m_window, UpdateMousePos);
	
	while (!glfwWindowShouldClose(m_window)) {
		// Calculates delta time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// === Processes inputs ===
		HandleWindowInput(m_window);
		camController.HandleKeyboardInput(m_window, deltaTime);

		// === Rendering ===
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PointLight pointLight;
		pointLight.Position = { 0.0f, 2.0f, 3.f };
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
		mesh.Draw(lightSourceShader);

		phongShader.Use();
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

		int boxCount = 25;
		int rowSize = 5;
		float gridSize = 1.2f;
		for(int i = 0; i < boxCount; i++) {
			glm::mat4 litModel(1.0f);
			int rowIdx = (i / rowSize) - (rowSize / 2);
			int colIdx = (i % rowSize) - (rowSize / 2);
			litModel = glm::translate(litModel, glm::vec3(rowIdx * gridSize, 0.f, colIdx * gridSize));
			phongShader.SetMatrix("model", litModel);
			phongShader.SetMatrix("view", camera.GetView());
			phongShader.SetMatrix("projection", camera.GetProjection());

			mesh.Draw(phongShader);
		}

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(m_window);
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