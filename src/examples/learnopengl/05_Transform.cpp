#include "05_Transform.h"

#include "shared/Utils.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

// clang-format off
static float vertices[] = {
	// Coordinates              // UVs
	-0.5f, -0.5f, 0.0f,         0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,          1.0f, 0.0f,
	0.5f, 0.5f, 0.0f,           1.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,          0.0f, 1.0f,
};

static unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0,
};
// clang-format on

spr::TextureHandle LOGL_05_Transform::LoadTexture(const char *path) {
	int texWidth, texHeight, numOfChannels;
	unsigned char *textureData = stbi_load(path, &texWidth, &texHeight, &numOfChannels, 0);
	if (!textureData) {
		std::cout << "Couldn't load texture\n";
	}

	spr::TextureInfo textureInfo;
	textureInfo.Width = texWidth;
	textureInfo.Height = texHeight;
	textureInfo.Format = numOfChannels == 4 ? spr::TextureFormat::RGBA8 : spr::TextureFormat::RGB8;
	spr::TextureHandle handle = spr::createTexture(textureInfo, textureData);
	stbi_image_free(textureData);
	return handle;
}

static spr::SamplerInfo GetDefaultSamplerInfo() {
	static spr::SamplerInfo defaultSampler;
	return defaultSampler;
}

void LOGL_05_Transform::OnAttach() {
	spr::VertexAttributeLayout layout;
	layout.begin()
	    .add({"aPosition", spr::AttributeType::Float, 3})
	    .add({"aUVCoords", spr::AttributeType::Float, 2})
	    .end();

	m_CubeVertexBuffer = spr::createVertexBuffer(vertices, sizeof(vertices), layout);
	m_CubeIndexBuffer = spr::createIndexBuffer(indices, sizeof(indices));

	// 0. Declares the Uniforms in the renderer, so that we can pass data to it later on
	m_TexUniform = spr::createUniform("tex", spr::UniformType::Sampler);
	m_AnotherTexUniform = spr::createUniform("anotherTex", spr::UniformType::Sampler);
	m_TransformUniform = spr::createUniform("transform", spr::UniformType::Mat4x4);

	// 1. Declares transform Uniform on the Vertex Shader (we want to transform per-vertex)
	std::string vertexSrc = utils::readShaderFile("shaders/04_vertex_transform.vert");
	spr::ShaderHandle vertexShader = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = utils::readShaderFile("shaders/03_frag_tex.frag");
	spr::ShaderHandle fragmentShader = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_ShaderProgram = spr::createProgram(vertexShader, fragmentShader);

	m_BrickTexture = LoadTexture("assets/bricks.jpg");
	m_LadybugTexture = LoadTexture("assets/yps.png");
}

void LOGL_05_Transform::OnUpdate() {
	// 2. Enables depth testing, allowing the Vertex Shader to write to the Depth Buffer, which is used by the ROP to determine
	// which pixels are in front of each other (hence, making it possible to do 3D rendering), more on this later.
	glEnable(GL_DEPTH_TEST);

	// 3. Clears Depth Buffer before rendering, as well as the Color Buffer, as usual
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 4. Calculates the cube's transform
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(glm::sin((float)glfwGetTime()) * 0.5f, 0.2f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(1.2f, 1.2f, 1.2f));

	// 5. Passes the transform to the Shader Program as an Uniform
	const int texUnit = 0, anotherTexUnit = 1;
	spr::setUniform(m_TexUniform, &texUnit);
	spr::setUniform(m_AnotherTexUniform, &anotherTexUnit);
	spr::setUniform(m_TransformUniform, glm::value_ptr(transform));

	spr::setTexture(0, m_BrickTexture, GetDefaultSamplerInfo());
	spr::setTexture(1, m_LadybugTexture, GetDefaultSamplerInfo());

	spr::setVertexBuffer(m_CubeVertexBuffer);
	spr::setIndexBuffer(m_CubeIndexBuffer);

	spr::submit(m_ShaderProgram);

	spr::render();
	spr::clean();
}

void LOGL_05_Transform::OnDetach() {
	spr::destroy(m_CubeVertexBuffer);
	spr::destroy(m_CubeIndexBuffer);

	spr::destroy(m_ShaderProgram);
	spr::destroy(m_BrickTexture);
	spr::destroy(m_LadybugTexture);

	spr::destroy(m_TexUniform);
	spr::destroy(m_AnotherTexUniform);
	spr::destroy(m_TransformUniform);
}
