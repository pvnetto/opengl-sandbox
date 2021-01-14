#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shared/Window.h"
#include "examples/learnopengl/02_EBO.h"
#include "examples/learnopengl/13_Meshes.h"


int main() {
	Window window;
	window.AttachLayer(new LOGL_02_EBO());

	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window.GetNativeWindow())) {
		// === Rendering ===
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// === Update ===
		window.OnUpdate();

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(window.GetNativeWindow());
	}

	glfwTerminate();
	return 0;
}