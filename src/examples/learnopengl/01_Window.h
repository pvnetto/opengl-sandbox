#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

int window_imp() {
	int width = 800, height = 600;

	// Step 1) Initialize window
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Specifies that OpenGL is using Core profile, instead of Immediate

	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL Sandbox", NULL, NULL); // Creates window
	if (!window) {
		std::cout << "Failed to create an OpenGL window :(\n";
		glfwTerminate();
		return -1;
	}

	// Step 2) Bind window to OpenGL Context
	// Makes context of the window the current OpenGL context, so any calls to OpenGL will be applied to this context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	// Step 3) Load modern OpenGL functions
	// Loads Glad by passing the extension loader function as parameter.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize Glad :(\n";
		return -1;
	}

	// Step 4) Use OpenGL :)
	// Before making any calls to OpenGL, the viewport must be initialized
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {
		// Processes inputs
		HandleWindowInput([](GLFWwindow *window) {
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
		});

		// Rendering
		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT); // Fills the color buffer with clearColor

		// Swaps buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}