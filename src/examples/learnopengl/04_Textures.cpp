#include "04_Textures.h"
#include "shared/Utils.h"
#include "spr/OpenGL/Helpers.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <cassert>

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
	    .add({"inPosition", spr::AttributeType::Float, 3})
	    .add({"inUV", spr::AttributeType::Float, 2})
	    .end();

	// 1. Defines texture as integer uniforms, so we can pass a texture unit index to the shader
	m_TexUniform0 = spr::createUniform("tex", spr::UniformType::Sampler);
	m_TexUniform1 = spr::createUniform("anotherTex", spr::UniformType::Sampler);

	m_VertexBufferHandle = spr::createVertexBuffer(vertices, sizeof(vertices), layout);
	m_IndexBufferHandle = spr::createIndexBuffer(indices, sizeof(indices));

	std::string vertexSrc = Utils::ReadShaderFile("shaders/03_vertex_tex.vert");
	spr::ShaderHandle vertexHandle = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = Utils::ReadShaderFile("shaders/03_frag_tex.frag");
	spr::ShaderHandle fragHandle = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_ShaderHandle = spr::createProgram(vertexHandle, fragHandle);

	// (OPTIONAL) Checks if immutable storage is enabled
	assert(isGLExtensionSupported("GL_ARB_texture_storage") && "::ERROR: GL_ARB_texture_storage is not enabled");

	// 2. Loads image from disk
	// BEWARE: Y coordinates are flipped on OpenGL, so textures must be flipped
	stbi_set_flip_vertically_on_load(true);

	int texWidth, texHeight, numOfChannels;
	unsigned char *brickData = stbi_load("assets/bricks.jpg", &texWidth, &texHeight, &numOfChannels, 0);
	if (!brickData) {
		std::cout << "Couldn't load texture\n";
	}

	// 3. Creates OpenGL Texture Object
	glCreateTextures(GL_TEXTURE_2D, 1, &m_BrickTexture);

	// 4. Sets Texture Object data (immutable storage)
	glTextureStorage2D(m_BrickTexture, 1, GL_RGB8, texWidth, texHeight);
	glTextureSubImage2D(m_BrickTexture, 0, 0, 0, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, brickData);

	// 5. Sets Texture Object sampling parameters
	glTextureParameteri(m_BrickTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_BrickTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_BrickTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_BrickTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// (OPTIONAL) Generates other mipmap levels automatically
	glGenerateTextureMipmap(m_BrickTexture);

	// 6. Binds Texture Object to Texture Unit
	glBindTextureUnit(0, m_BrickTexture);

	stbi_image_free(brickData);


	// (OPTIONAL) Loads another texture and binds it to texture unit 1
	int tex2Width, tex2Height, tex2Channels;
	unsigned char *tex2Data = stbi_load("assets/yps.png", &tex2Width, &tex2Height, &tex2Channels, 0);
	if (!tex2Data) {
		std::cout << "Couldn't load texture\n";
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_OtherTexture);
	glTextureStorage2D(m_OtherTexture, 1, GL_RGBA8, tex2Width, tex2Height);
	glTextureSubImage2D(m_OtherTexture, 0, 0, 0, tex2Width, tex2Height, GL_RGBA, GL_UNSIGNED_BYTE, tex2Data);
	glGenerateTextureMipmap(m_OtherTexture);

	// (OPTIONAL, RECOMMENDED) Uses a Sampler Object instead of setting sampling parameters directly on the Texture
	// NOTE: This is more modern, and also recommended because that's how it works on the GPU
	glCreateSamplers(1, &m_OtherTextureSampler);
	glSamplerParameteri(m_OtherTextureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_OtherTextureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_OtherTextureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_OtherTextureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTextureUnit(1, m_OtherTexture);
	glBindSampler(1, m_OtherTextureSampler);

	stbi_image_free(tex2Data);
}

void LOGL_04_Textures::OnUpdate() {
	// 7. Passes Texture Unit indices as Uniforms
	const int tex0Unit = 0, tex1Unit = 1;
	spr::setUniform(m_TexUniform0, &tex0Unit);
	spr::setUniform(m_TexUniform1, &tex1Unit);

	spr::setVertexBuffer(m_VertexBufferHandle);
	spr::setIndexBuffer(m_IndexBufferHandle);

	spr::submit(m_ShaderHandle);

	spr::render();
	spr::clean();
}

void LOGL_04_Textures::OnDetach() {
	spr::destroy(m_TexUniform0);
	spr::destroy(m_TexUniform1);
	spr::destroy(m_ShaderHandle);
	spr::destroy(m_VertexBufferHandle);
	spr::destroy(m_IndexBufferHandle);

	// 7. Deletes textures/samplers after they're used
	glDeleteTextures(1, &m_BrickTexture);
	glDeleteTextures(1, &m_OtherTexture);
	glDeleteSamplers(1, &m_OtherTextureSampler);
}
