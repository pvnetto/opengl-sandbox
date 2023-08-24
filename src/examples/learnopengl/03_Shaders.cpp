#include "03_Shaders.h"

#include "shared/Utils.h"

#include <glad/glad.h>
#include <iostream>

static const float vertices[] = {
	-0.8f, -0.2f, 0.0f, // left
	-0.4f, -0.2f, 0.0f, // right
	-0.6f, 0.2f, 0.0f,  // top
};

static const float verticesB[] = {
	0.0f, -0.2f, 0.0f, // left
	0.4f, -0.2f, 0.0f, // right
	0.2f, 0.2f, 0.0f,  // top
};


void LOGL_03_Shaders::CreateBuffers() {
	spr::VertexAttributeLayout layout;
	layout.begin()
			.add({ "inPosition", spr::AttributeType::Float, 3 })
			.end();


	m_TriangleVertexBuffer = spr::createVertexBuffer(vertices, sizeof(vertices), layout);
}

void LOGL_03_Shaders::CreateShaders() {
	std::string vertexSrc = Utils::ReadShaderFile("shaders/02_vertex_offset.vert");
	spr::ShaderHandle vertexHandle = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = Utils::ReadShaderFile("shaders/frag_basic.frag");
	spr::ShaderHandle fragHandle = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_Uniform = spr::createUniform("horizontalOffset", spr::UniformType::Float);
	m_ShaderProgram = spr::createProgram(vertexHandle, fragHandle);
}


void LOGL_03_Shaders::OnAttach() {
	CreateBuffers();
	CreateShaders();
}

void LOGL_03_Shaders::OnUpdate() {
	const float horizontalOffset = (float) std::sin(glfwGetTime()) * 1.0f;
	spr::setUniform(m_Uniform, &horizontalOffset);
	spr::setVertexBuffer(m_TriangleVertexBuffer);
	spr::submit(m_ShaderProgram);

	spr::render();
	
	spr::clean();
}

void LOGL_03_Shaders::OnDetach() {
	spr::destroy(m_ShaderProgram);
	spr::destroy(m_Uniform);
	spr::destroy(m_TriangleVertexBuffer);
}
