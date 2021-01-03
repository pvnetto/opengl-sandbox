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
	glm::vec3(0.f, 0.f, 5.f),
	glm::vec3(0.f, -90.f, 0.f),
	glm::vec3(0.f, 1.f, 0.f));

FreeCameraController camController(camera, 4.5f);

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

	auto verticesVec = PrimitiveShape::Cube();
	float *vertices = verticesVec.data();

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesVec.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader phongShader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/09_specular_map.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/09_emission_map.frag");

	Shader lightSourceShader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/05_vertex_mvp.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/06_frag_light_source.frag");

	Texture2D::LoadTexture(GL_TEXTURE0, "../../src/assets/container.png");
	Texture2D::LoadTexture(GL_TEXTURE1, "../../src/assets/container_specular.png");
	Texture2D::LoadTexture(GL_TEXTURE2, "../../src/assets/matrix.jpg");

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

		auto normalizeTrig = [](float val) { return (val + 1.f) / 2.f; };
		glm::vec3 lightColor{ 1.f, 0.8f, 0.6f };
		glm::vec3 lightSourcePos = glm::vec3(
			std::cos((float) glfwGetTime()),
			std::cos((float) glfwGetTime()) * std::sin((float) glfwGetTime()),
			std::sin((float) glfwGetTime())) * 2.f;

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, lightSourcePos);
		sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

		lightSourceShader.Use();
		lightSourceShader.SetMatrix("model", sourceModel);
		lightSourceShader.SetMatrix("view", camera.GetView());
		lightSourceShader.SetMatrix("projection", camera.GetProjection());
		lightSourceShader.SetVector3("lightColor", lightColor);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 litModel(1.0f);
		litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.12f, glm::vec3(0.0f, 1.0f, 0.0f));

		phongShader.Use();
		phongShader.SetMatrix("model", litModel);
		phongShader.SetMatrix("view", camera.GetView());
		phongShader.SetMatrix("projection", camera.GetProjection());

		phongShader.SetInt("material.diffuseMap", 0);
		phongShader.SetInt("material.specularMap", 1);
		phongShader.SetInt("material.emissionMap", 2);
		phongShader.SetFloat("material.ambientStrength", 0.1f);
		phongShader.SetFloat("material.diffuseStrength", 0.5f);
		phongShader.SetFloat("material.specularStrength", 1.f);
		phongShader.SetFloat("material.shininess", 32.f);

		phongShader.SetVector3("light.color", lightColor);
		phongShader.SetVector3("light.position", lightSourcePos);
		phongShader.SetVector3("viewPos", camera.GetPosition());
		phongShader.SetFloat("time", (float) glfwGetTime());

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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