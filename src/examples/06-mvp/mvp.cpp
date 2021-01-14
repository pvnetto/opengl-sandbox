#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "stb_image.h"

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

int main() {

	int width = 800, height = 600;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL Sandbox", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create an OpenGL window :(\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnWindowResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize Glad :(\n";
		return -1;
	}

	glViewport(0, 0, width, height);

	auto verticesVec = PrimitiveShape::Cube();
	float* vertices = verticesVec.data();

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

	Shader shader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/05_vertex_mvp.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/03_frag_tex.frag");

	int texWidth, texHeight, numOfChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *brickData = stbi_load("../../src/assets/bricks.jpg", &texWidth, &texHeight, &numOfChannels, 0);

	if (!brickData) {
		std::cout << "Couldn't load texture\n";
	}

	unsigned int brickTexture;
	glGenTextures(1, &brickTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, brickData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(brickTexture);

	stbi_image_free(brickData);

	// Loads another texture
	int tex2Width, tex2Height, tex2Channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *tex2Data = stbi_load("../../src/assets/yps.png", &tex2Width, &tex2Height, &tex2Channels, 0);

	if (!tex2Data) {
		std::cout << "Couldn't load texture\n";
	}

	unsigned int tex2;
	glGenTextures(1, &tex2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2Width, tex2Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2Data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(tex2);

	stbi_image_free(tex2Data);

	// 1) Enables depth test, so vertices are stored on a z-buffer before drawing
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		// === Processes inputs ===
		HandleWindowInput(window);

		// === Rendering ===
		glClearColor(0.5f, 0.2f, 0.1f, 1);
		// 2) Clears z-buffer before rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		shader.SetFloat("horizontalOffset", std::sin(glfwGetTime()) * 1.0f);
		shader.SetInt("tex", 0);
		shader.SetInt("anotherTex", 1);

		// 3) Creates model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.2f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));

		// 4) Creates view matrix
		glm::mat4 view(1.0f);
		glm::vec3 cameraPos (0.0f, 0.0f, -3.0f);
		view = glm::translate(view, -cameraPos);

		// 5) Creates projection matrix
		glm::mat4 projection(1.0f);
		float fov = 90.f, aspectRatio = width / height, near = 0.1f, far = 100.f;
		projection = glm::perspective(fov, aspectRatio, near, far);

		// 6) Passes MVP matrices as uniforms to shader
		shader.SetMatrix("model", model);
		shader.SetMatrix("view", view);
		shader.SetMatrix("projection", projection);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}