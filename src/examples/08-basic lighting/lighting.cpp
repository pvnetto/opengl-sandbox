#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "stb_image.h"

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

// 1) Instantiates camera and its controller
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader lightingShader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/05_vertex_mvp.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/06_frag_basic_lighting.frag");

	Shader lightSourceShader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/05_vertex_mvp.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/06_frag_light_source.frag");

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
		glClearColor(0.5f, 0.2f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1) Draws light source
		glm::vec3 lightColor{ 0.8f, 0.8f, 1.f };
		glm::vec3 orbitPosition = glm::vec3(
			std::cos((float) glfwGetTime()),
			std::cos((float) glfwGetTime()) * std::sin((float) glfwGetTime()),
			std::sin((float) glfwGetTime())) * 2.f;

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, orbitPosition);
		sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

		lightSourceShader.Use();
		lightSourceShader.SetMatrix("model", sourceModel);
		lightSourceShader.SetMatrix("view", camera.GetView());
		lightSourceShader.SetMatrix("projection", camera.GetProjection());
		lightSourceShader.SetVector3("lightColor", lightColor);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2) Draws lit object
		glm::mat4 litModel(1.0f);
		litModel = glm::rotate(litModel, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		litModel = glm::rotate(litModel, (float)glfwGetTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
		litModel = glm::rotate(litModel, (float)glfwGetTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

		lightingShader.Use();
		lightingShader.SetMatrix("model", litModel);
		lightingShader.SetMatrix("view", camera.GetView());
		lightingShader.SetMatrix("projection", camera.GetProjection());
		lightingShader.SetVector3("objectColor", glm::vec3(0.2f, 0.35f, 1.f));
		lightingShader.SetVector3("lightColor", lightColor);

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