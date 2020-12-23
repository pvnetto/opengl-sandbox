#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "stb_image.h"

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

	// 0) Declares VAO and VBOs + attributes
	float verticesA[] = {
	    // Triangle 1
	    -0.5f, -0.5f, 0.0f, 		0.0f, 0.0f, 	// left
	    0.5f, -0.5f, 0.0f, 			1.0f, 0.0f,  	// right
	    0.5f, 0.5f, 0.0f, 			1.0f, 1.0f,   	// top-right
	    -0.5f, 0.5f, 0.0f, 			0.0f, 1.0f,  	// top-left
	};

	unsigned int indices[] = {
	    0, 1, 2,
	    2, 3, 0,
	};

	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbinds buffers so they're not accidentally used
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 3) Declares shader program
	Shader shader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/03_vertex_tex.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/03_frag_tex.frag");

	// 4) Loads textures
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

	while (!glfwWindowShouldClose(window)) {
		// === Processes inputs ===
		HandleWindowInput(window);

		// === Rendering ===
		glClearColor(0.5f, 0.2f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// 5) Use shader program, bind VAO and draw the triangle
		shader.Use();
		shader.SetFloat("horizontalOffset", std::sin(glfwGetTime()) * 1.0f);
		shader.SetInt("tex", 0);
		shader.SetInt("anotherTex", 1);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}