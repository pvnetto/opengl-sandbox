#include "02_VAO.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

// clang-format off
static constexpr float verticesTriangleA[] = {
    -0.8f, -0.2f, 0.0f, // left
    -0.4f, -0.2f, 0.0f, // right
    -0.6f, 0.2f, 0.0f,  // top
};

static constexpr float verticesTriangleB[] = {
    0.0f, -0.2f, 0.0f,
    0.4f, -0.2f, 0.0f,
    0.2f, 0.2f, 0.0f,
};
// clang-format on

static const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 inPosition;
void main() {
	gl_Position = vec4(inPosition, 1.0f);
})";

static const char *fragmentShaderSrc = R"(
#version 330 core
out vec4 fragmentColor;
void main() {
	fragmentColor = vec4(0.2f, 0.3f, 1.0f, 1.0f);
})";

static const char *yellowFragShaderSrc = R"(
#version 330 core
out vec4 fragmentColor;
void main() {
	fragmentColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
})";

void LOGL_02_VAO::DeclareBuffers() {
	// 1. Declares VAO for first triangle
	glCreateVertexArrays(1, &m_VAOa);
	glBindVertexArray(m_VAOa);

	// 2. Creates vertex buffer for first triangle and fills it with vertex data
	unsigned int vboA;
	glCreateBuffers(1, &vboA);
	glBindBuffer(GL_ARRAY_BUFFER, vboA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleA), verticesTriangleA, GL_STATIC_DRAW);

	// 3. Declares vertex layout that the VAO will use
	// NOTE: This is actually bound to the VAO, which now holds the layout and a reference to the buffer bound to GL_ARRAY_BUFFER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 4. Does the same process for another triangle
	glCreateVertexArrays(1, &m_VAOb);
	glBindVertexArray(m_VAOb);

	unsigned int vboB;
	glCreateBuffers(1, &vboB);
	glBindBuffer(GL_ARRAY_BUFFER, vboB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleB), verticesTriangleB, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
}

void LOGL_02_VAO::DeclareShader(unsigned int &shaderId, const char *vertexShaderSource, const char *fragShaderSource) {
	// Creates/compiles vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int vertSuccess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
	if (!vertSuccess) {
		char info[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Failed to compile vertex shader: " << info << "\n";
		return;
	}

	// Creates/Compiles fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);

	int fragSuccess;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
	if (!fragSuccess) {
		char info[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		std::cout << "Failed to compile fragment shader: " << info << "\n";
		return;
	}

	// Creates/links shader program
	shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexShader);
	glAttachShader(shaderId, fragmentShader);
	glLinkProgram(shaderId);

	int programSuccess;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &programSuccess);

	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(shaderId, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		return;
	}

	// Deletes both shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void LOGL_02_VAO::OnAttach() {
	DeclareBuffers();
	DeclareShader(m_ShaderProgram, vertexShaderSource, fragmentShaderSrc);
	DeclareShader(m_YellowShaderProgram, vertexShaderSource, yellowFragShaderSrc);
}

void LOGL_02_VAO::OnUpdate() {
	glClear(GL_COLOR_BUFFER_BIT);

	// 5. Binds VAO and draws the triangle
	glUseProgram(m_ShaderProgram);
	glBindVertexArray(m_VAOa);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(m_YellowShaderProgram);
	glBindVertexArray(m_VAOb);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
