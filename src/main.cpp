#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

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
	    -0.8f, -0.2f, 0.0f, // left
	    -0.4f, -0.2f, 0.0f, // right
	    -0.6f, 0.2f, 0.0f,  // top
	};

	float verticesB[] = {
	    // Triangle 2
	    0.0f, -0.2f, 0.0f, // left
	    0.4f, -0.2f, 0.0f, // right
	    0.2f, 0.2f, 0.0f,  // top
	};

	unsigned int vaoA, vboA, vaoB, vboB;
	glGenVertexArrays(1, &vaoA);
	glGenBuffers(1, &vboA);

	// Binds VAO, so all subsequent buffers are bound to it
	glBindVertexArray(vaoA);

	// Binds VBO after VAO
	glBindBuffer(GL_ARRAY_BUFFER, vboA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

	// glVertexAttribPointer should tell the shader how to read buffer data
	// In this case we are declaring vec3 parameters as subsequent float values in an array,
	// so size = 3, type = float and stride = 3 * sizeof(float)
	// Also, Attributes are declared on slot 0, so the shader should use layout = 0 to find them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbinds buffers so they're not accidentally used
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Sets second VAO
	glGenVertexArrays(1, &vaoB);
	glGenBuffers(1, &vboB);
	glBindVertexArray(vaoB);

	glBindBuffer(GL_ARRAY_BUFFER, vboB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 3) Declares shader program
	Shader shader("C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/02_vertex_out.vert",
	              "C:/Users/USUARIO/Desktop/Projects/Study/opengl-sandbox/src/shaders/02_frag_in.frag");

	while (!glfwWindowShouldClose(window)) {
		// === Processes inputs ===
		HandleWindowInput(window);

		// === Rendering ===
		glClearColor(0.5f, 0.2f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// 4) Use shader program, bind VAO and draw the triangle
		shader.Use();
		glBindVertexArray(vaoA);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(vaoB);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}