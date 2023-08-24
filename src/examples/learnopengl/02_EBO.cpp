#include "02_EBO.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

static constexpr float vertices[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

static constexpr unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

static const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 inPosition;

void main() {
	gl_Position = vec4(inPosition, 1.0f);
})";

static const char* fragmentShaderSource = R"(
#version 330 core

out vec4 fragmentColor;

void main() {
	fragmentColor = vec4(0.2f, 0.3f, 1.0f, 1.0f);
})";


void LOGL_02_EBO::DeclareBufferObjects() {	
	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

    glCreateBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbinds buffers so they're not accidentally used.
	// Beware: You should NOT unbind EBO here
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LOGL_02_EBO::DeclareShaderProgram() {
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

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int fragSuccess;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);

	if (!fragSuccess) {
		char info[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		std::cout << "Failed to compile fragment shader: " << info << "\n";
		return;
	}

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);

	int programSuccess;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &programSuccess);

	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void LOGL_02_EBO::OnAttach() {
	DeclareBufferObjects();
	DeclareShaderProgram();
}

void LOGL_02_EBO::OnUpdate() {
    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
