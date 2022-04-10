#include "03_Shaders.h"

#include "shared/Utils.h"

#include <glad/glad.h>
#include <iostream>

static const float verticesA[] = {
	-0.8f, -0.2f, 0.0f, // left
	-0.4f, -0.2f, 0.0f, // right
	-0.6f, 0.2f, 0.0f,  // top
};

static const float verticesB[] = {
	0.0f, -0.2f, 0.0f, // left
	0.4f, -0.2f, 0.0f, // right
	0.2f, 0.2f, 0.0f,  // top
};

void LOGL_03_Shaders::DeclareBuffers() {
	// First VAO
	unsigned int vboA, vboB;
	glGenVertexArrays(1, &m_vaoA);
	glGenBuffers(1, &vboA);
	glBindVertexArray(m_vaoA);

	glBindBuffer(GL_ARRAY_BUFFER, vboA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Second VAO
	glGenVertexArrays(1, &m_vaoB);
	glGenBuffers(1, &vboB);
	glBindVertexArray(m_vaoB);

	glBindBuffer(GL_ARRAY_BUFFER, vboB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LOGL_03_Shaders::DeclareShader() {
	std::string vertexSrc = utils::readShaderFile("../../src/shaders/02_vertex_offset.vert");
	spr::ShaderHandle vertexHandle = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = utils::readShaderFile("../../src/shaders/frag_basic.frag");
	spr::ShaderHandle fragHandle = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_uniformHandle = spr::createUniform("horizontalOffset", spr::UniformType::Float);
	m_shaderProgramHandle = spr::createProgram(vertexHandle, fragHandle);
}


void LOGL_03_Shaders::OnAttach() {
	DeclareBuffers();
	DeclareShader();
}

void LOGL_03_Shaders::OnUpdate() {
	float horizontalOffset = std::sin(glfwGetTime()) * 1.0f;
	spr::setUniform(m_uniformHandle, &horizontalOffset);
	spr::submit(m_shaderProgramHandle);
	spr::render();

	glBindVertexArray(m_vaoA);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(m_vaoB);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	spr::cleanup();
}

void LOGL_03_Shaders::OnDettach() {
	spr::destroy(m_shaderProgramHandle);
	spr::destroy(m_uniformHandle);
}