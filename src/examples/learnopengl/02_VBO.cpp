#include "02_VBO.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

static constexpr float vertices[] = {
	// Triangle 1
	-0.8f, -0.2f, 0.0f,     // left
	-0.4f, -0.2f, 0.0f,     // right
	-0.6f, 0.2f, 0.0f,      // top

	// Triangle 2
	0.0f, -0.2f, 0.0f,     // left
	0.4f, -0.2f, 0.0f,     // right
	0.2f, 0.2f, 0.0f,      // top
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

void LOGL_02_VBO::DeclareBuffers() {
	glCreateVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glCreateBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LOGL_02_VBO::DeclareShader() {
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

	// Creates/compiles fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
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
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);

	int programSuccess;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		return;
	}

	// Deletes both shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);    
}

void LOGL_02_VBO::OnAttach() {
	DeclareBuffers();
	DeclareShader();
}

void LOGL_02_VBO::OnUpdate() {
    // 4) Use shader program, bind VAO and draw the triangle
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);	// Vertices are drawn counter-clockwise when using glDrawArrays 
}
