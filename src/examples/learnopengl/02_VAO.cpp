#include "02_VAO.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

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


static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPosition;
void main() {
	gl_Position = vec4(aPosition, 1.0f);
})";

static const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 fragmentColor;
void main() {
	fragmentColor = vec4(0.2f, 0.3f, 1.0f, 1.0f);
})";

static const char* yellowFragShaderSrc = R"(
#version 330 core
out vec4 fragmentColor;
void main() {
	fragmentColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
})";

void LOGL_02_VAO::DeclareBuffers() {
	// Declares first triangle
	glCreateVertexArrays(1, &m_vaoA);
	glBindVertexArray(m_vaoA);

	unsigned int vboA;
	glCreateBuffers(1, &vboA);
	glBindBuffer(GL_ARRAY_BUFFER, vboA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleA), verticesTriangleA, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Declares second triangle
	glCreateVertexArrays(1, &m_vaoB);
	glBindVertexArray(m_vaoB);

	unsigned int vboB;
	glCreateBuffers(1, &vboB);
	glBindBuffer(GL_ARRAY_BUFFER, vboB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleB), verticesTriangleB, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LOGL_02_VAO::DeclareShader(unsigned int& shaderId, const char* vertexShaderSource, const char* fragShaderSource) {
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
	DeclareShader(m_shaderProgram, vertexShaderSource, fragmentShaderSrc);
	DeclareShader(m_yellowShaderProgram, vertexShaderSource, yellowFragShaderSrc);
}

void LOGL_02_VAO::OnUpdate() {
	// 4) Use shader program, bind VAO and draw the triangle
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_vaoA);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(m_yellowShaderProgram);
	glBindVertexArray(m_vaoB);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}