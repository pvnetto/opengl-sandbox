#include "04_Textures.h"
#include "shared/Utils.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

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

void LOGL_04_Textures::OnAttach() {
	// 0. Declares UVs in vertex layout attributes
	spr::VertexAttributeLayout layout;
	layout.begin()
	    .add({"aPosition", spr::AttributeType::Float, 3})
	    .add({"aUVCoords", spr::AttributeType::Float, 2})
	    .end();

	// 1. Defines texture as integer uniforms, so we can pass a texture unit index to the shader
	std::string vertexSrc = utils::readShaderFile("shaders/03_vertex_tex.vert");
	spr::ShaderHandle vertexHandle = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = utils::readShaderFile("shaders/03_frag_tex.frag");
	spr::ShaderHandle fragHandle = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_shaderHandle = spr::createProgram(vertexHandle, fragHandle);

	// 2. Loads texture from memory and creates OpenGL texture
	// BEWARE: Y coordinates are flipped on OpenGL, so textures must be flipped
	stbi_set_flip_vertically_on_load(true);

	int texWidth, texHeight, numOfChannels;
	unsigned char *brickData = stbi_load("assets/bricks.jpg", &texWidth, &texHeight, &numOfChannels, 0);
	if (!brickData) {
		std::cout << "Couldn't load texture\n";
	}

	// 3. Creates OpenGL texture
	unsigned int brickTexture;
	glGenTextures(1, &brickTexture);

	// 4. Binds texture to texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, brickData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(brickTexture);

	stbi_image_free(brickData);

	// (OPTIONAL) Loads another texture and binds it to texture unit 1
	int tex2Width, tex2Height, tex2Channels;
	unsigned char *tex2Data = stbi_load("assets/yps.png", &tex2Width, &tex2Height, &tex2Channels, 0);
	if (!tex2Data) {
		std::cout << "Couldn't load texture\n";
	}

	unsigned int tex2;
	glGenTextures(1, &tex2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2Width, tex2Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2Data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(tex2);

	stbi_image_free(tex2Data);
}

void LOGL_04_Textures::OnUpdate() {
	const int tex0 = 0, tex1 = 1;
	spr::setUniform(m_texUniform0, &tex0);
	spr::setUniform(m_texUniform1, &tex1);
	spr::setVertexBuffer(m_vertexBufferHandle);
	spr::setIndexBuffer(m_indexBufferHandle);

	spr::submit(m_shaderHandle);
	spr::render();

	spr::cleanup();
}

void LOGL_04_Textures::OnDettach() {
	spr::destroy(m_texUniform0);
	spr::destroy(m_texUniform1);
	spr::destroy(m_shaderHandle);
	spr::destroy(m_vertexBufferHandle);
	spr::destroy(m_indexBufferHandle);
}

