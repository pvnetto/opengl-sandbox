#include "05_Transform.h"

#include "shared/Utils.h"
#include "shared/RenderUtils.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

static spr::SamplerInfo GetDefaultSamplerInfo() {
	static spr::SamplerInfo defaultSampler;
	return defaultSampler;
}

void LOGL_05_Transform::OnAttach() {
	m_CubeVertexBuffer = Utils::LoadCube();

	// 0. Declares the transform Uniform in the renderer, so that we can pass data to it later on
	m_TexUniform = spr::createUniform("tex", spr::UniformType::Sampler);
	m_AnotherTexUniform = spr::createUniform("anotherTex", spr::UniformType::Sampler);
	m_TransformUniform = spr::createUniform("transform", spr::UniformType::Mat4x4);

	// 1. Declares transform Uniform on the Vertex Shader (we want to transform per-vertex)
	std::string vertexSrc = Utils::ReadShaderFile("shaders/04_vertex_transform.vert");
	spr::ShaderHandle vertexShader = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = Utils::ReadShaderFile("shaders/03_frag_tex.frag");
	spr::ShaderHandle fragmentShader = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_ShaderProgram = spr::createProgram(vertexShader, fragmentShader);

	m_BrickTexture = Utils::LoadTexture("assets/bricks.jpg");
	m_LadybugTexture = Utils::LoadTexture("assets/yps.png");
}

void LOGL_05_Transform::OnUpdate() {
	// 2. Enables depth testing, allowing the Vertex Shader to write to the Depth Buffer, which is used by the ROP to determine
	// which pixels are in front of each other (thus enabling 3D rendering), more on this later.
	// NOTE: We don't need to call this here, the renderer is already doing it
	// glEnable(GL_DEPTH_TEST);

	// 3. Clears Depth Buffer before rendering, as well as the Color Buffer, as usual
	// NOTE: We don't need to call this here, the renderer is already doing it
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 4. Calculates the cube's transform
	glm::mat4 transform(1.0f);
	//transform = glm::translate(transform, glm::vec3(glm::sin((float)glfwGetTime()) * 0.5f, 0.2f, 0.0f));
	//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(1.2f, 1.2f, 1.2f));

	// 5. Passes the transform to the Shader Program as an Uniform
	const int texUnit = 0, anotherTexUnit = 1;
	spr::setUniform(m_TexUniform, &texUnit);
	spr::setUniform(m_AnotherTexUniform, &anotherTexUnit);
	spr::setUniform(m_TransformUniform, glm::value_ptr(transform));

	spr::setTexture(0, m_BrickTexture, GetDefaultSamplerInfo());
	spr::setTexture(1, m_LadybugTexture, GetDefaultSamplerInfo());

	spr::setVertexBuffer(m_CubeVertexBuffer);

	spr::submit(m_ShaderProgram);

	spr::render();
	spr::clean();
}

void LOGL_05_Transform::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);

	spr::destroy(m_ShaderProgram);
	spr::destroy(m_BrickTexture);
	spr::destroy(m_LadybugTexture);

	spr::destroy(m_TexUniform);
	spr::destroy(m_AnotherTexUniform);
	spr::destroy(m_TransformUniform);
}
