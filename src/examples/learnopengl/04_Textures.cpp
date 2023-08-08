#include "04_Textures.h"
#include "shared/Utils.h"
#include "shared/renderer/OpenGL/Helpers.h"

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
	m_texUniform0 = spr::createUniform("tex", spr::UniformType::Sampler);
	m_texUniform1 = spr::createUniform("anotherTex", spr::UniformType::Sampler);

	m_vertexBufferHandle = spr::createVertexBuffer(vertices, sizeof(vertices), layout);
	m_indexBufferHandle = spr::createIndexBuffer(indices, sizeof(indices));

	std::string vertexSrc = utils::readShaderFile("shaders/03_vertex_tex.vert");
	spr::ShaderHandle vertexHandle = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

	std::string fragSrc = utils::readShaderFile("shaders/03_frag_tex.frag");
	spr::ShaderHandle fragHandle = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

	m_shaderHandle = spr::createProgram(vertexHandle, fragHandle);

	// (OPTIONAL) Checks if immutable storage is enabled
	bool hasTextureStorage = isGLExtensionSupported("GL_ARB_texture_storage");
	std::cout << "Texture storage status: " << (hasTextureStorage ? "true" : "false") << "\n";

	// 2. Loads image from disk
	// BEWARE: Y coordinates are flipped on OpenGL, so textures must be flipped
	stbi_set_flip_vertically_on_load(true);

	int texWidth, texHeight, numOfChannels;
	unsigned char *brickData = stbi_load("assets/bricks.jpg", &texWidth, &texHeight, &numOfChannels, 0);
	if (!brickData) {
		std::cout << "Couldn't load texture\n";
	}

	// 3. Creates OpenGL Texture Object
	glCreateTextures(GL_TEXTURE_2D, 1, &m_brickTexture);

	// 4. Sets Texture Object data (immutable storage)
	glTextureStorage2D(m_brickTexture, 1, GL_RGB8, texWidth, texHeight);
	glTextureSubImage2D(m_brickTexture, 0, 0, 0, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, brickData);

	// 5. Sets Texture Object sampling parameters
	glTextureParameteri(m_brickTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_brickTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_brickTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_brickTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// (OPTIONAL) Generates other mipmap levels automatically
	glGenerateTextureMipmap(m_brickTexture);

	// 6. Binds Texture Object to Texture Unit
	glBindTextureUnit(0, m_brickTexture);

	stbi_image_free(brickData);


	// (OPTIONAL) Loads another texture and binds it to texture unit 1
	int tex2Width, tex2Height, tex2Channels;
	unsigned char *tex2Data = stbi_load("assets/yps.png", &tex2Width, &tex2Height, &tex2Channels, 0);
	if (!tex2Data) {
		std::cout << "Couldn't load texture\n";
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_otherTexture);
	glTextureStorage2D(m_otherTexture, 1, GL_RGBA8, tex2Width, tex2Height);
	glTextureSubImage2D(m_otherTexture, 0, 0, 0, tex2Width, tex2Height, GL_RGBA, GL_UNSIGNED_BYTE, tex2Data);
	glGenerateTextureMipmap(m_otherTexture);

	// (OPTIONAL, RECOMMENDED) Uses a Sampler Object instead of setting sampling parameters directly on the Texture
	// NOTE: This is more modern, and also recommended because that's how it works on the GPU
	glCreateSamplers(1, &m_otherTextureSampler);
	glSamplerParameteri(m_otherTextureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_otherTextureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_otherTextureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_otherTextureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTextureUnit(1, m_otherTexture);
	glBindSampler(1, m_otherTextureSampler);

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
	spr::clean();
}

void LOGL_04_Textures::OnDettach() {
	spr::destroy(m_texUniform0);
	spr::destroy(m_texUniform1);
	spr::destroy(m_shaderHandle);
	spr::destroy(m_vertexBufferHandle);
	spr::destroy(m_indexBufferHandle);

	// 7. Deletes textures/samplers after they're used
	glDeleteTextures(1, &m_brickTexture);
	glDeleteTextures(1, &m_otherTexture);
	glDeleteSamplers(1, &m_otherTextureSampler);
}
