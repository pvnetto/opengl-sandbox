#include "07_Compute.h"

#include "shared/Utils.h"

#include <glad/glad.h>
#include <iostream>

void AOGL_07_Compute::OnAttach() {
	m_OutputSize = { 1024.f, 1024.f };

	CreateTexturedQuadShaderProgram();
	CreateBufferObjects();

	// 0. Declares compute shader
	{
		const std::string computeSrc = Utils::ReadShaderFile("shaders/example_compute.comp");
		unsigned int computeShader = CreateShaderObject(GL_COMPUTE_SHADER, computeSrc);

		m_ComputeShaderProgram = glCreateProgram();
		glAttachShader(m_ComputeShaderProgram, computeShader);
		glLinkProgram(m_ComputeShaderProgram);

		int programSuccess;
		glGetProgramiv(m_ComputeShaderProgram, GL_LINK_STATUS, &programSuccess);
		if (!programSuccess) {
			char info[512];
			glGetProgramInfoLog(m_ComputeShaderProgram, 512, NULL, info);
			std::cout << "Failed to link compute shader program: " << info << "\n";
			assert(false);
		}

		glDeleteShader(computeShader);
	}

	// 1. Declares in/out texture
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_OutputTextureID);

		// NOTE: Internal format must match what's specified in the in/out uniform
		const GLenum internalFormat = GL_RGBA32F;
		glTextureStorage2D(m_OutputTextureID, 1, internalFormat, m_OutputSize.x, m_OutputSize.y);

		glTextureParameteri(m_OutputTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_OutputTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_OutputTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_OutputTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glDisable(GL_DEPTH_TEST);
}

void AOGL_07_Compute::CreateTexturedQuadShaderProgram() {
	const std::string vertexSrc = Utils::ReadShaderFile("shaders/default_vertex.vert");
	unsigned int vertexShader = CreateShaderObject(GL_VERTEX_SHADER, vertexSrc);

	const std::string fragmentSrc = Utils::ReadShaderFile("shaders/draw_textured_quad.frag");
	unsigned int fragmentShader = CreateShaderObject(GL_FRAGMENT_SHADER, fragmentSrc);

	m_TexturedQuadShaderProgram = glCreateProgram();
	glAttachShader(m_TexturedQuadShaderProgram, vertexShader);
	glAttachShader(m_TexturedQuadShaderProgram, fragmentShader);
	glLinkProgram(m_TexturedQuadShaderProgram);

	int programSuccess;
	glGetProgramiv(m_TexturedQuadShaderProgram, GL_LINK_STATUS, &programSuccess);

	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_TexturedQuadShaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		assert(false);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

unsigned int AOGL_07_Compute::CreateShaderObject(unsigned int shaderType, const std::string &shaderSource) const {
	const char *shaderSrcC = shaderSource.data();

	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderSrcC, NULL);
	glCompileShader(shaderID);

	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info[512];
		glGetShaderInfoLog(shaderID, 512, NULL, info);
		std::cout << "Failed to compile shader: " << info << "\n";
		assert(false);
	}

	return shaderID;
}

void AOGL_07_Compute::CreateBufferObjects() {
	m_QuadData = Utils::GetScreenQuadData();
	glCreateBuffers(1, &m_QuadVertexBufferID);
	glNamedBufferData(m_QuadVertexBufferID, m_QuadData.VerticesSize * sizeof(float), m_QuadData.Vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &m_QuadIndexBufferID);
	glNamedBufferData(m_QuadIndexBufferID, m_QuadData.NumIndices * sizeof(unsigned int), m_QuadData.Indices, GL_STATIC_DRAW);

	glCreateVertexArrays(1, &m_VertexArrayObjectID);
	glVertexArrayElementBuffer(m_VertexArrayObjectID, m_QuadIndexBufferID);

	const int vertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_VertexArrayObjectID, vertexBufferBindingPoint, m_QuadVertexBufferID, NULL, m_QuadData.Layout.getStride());
	for (int i = 0, count = m_QuadData.Layout.getAttributeCount(); i < count; i++) {
		spr::VertexAttribute layoutAttribute;
		if (m_QuadData.Layout.getAttribute(i, layoutAttribute)) {
			const int attribLocation = glGetAttribLocation(m_TexturedQuadShaderProgram, layoutAttribute.Name.data());
			if (attribLocation >= 0) {
				glVertexArrayAttribFormat(m_VertexArrayObjectID, attribLocation, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
				glVertexArrayAttribBinding(m_VertexArrayObjectID, attribLocation, vertexBufferBindingPoint);
				glEnableVertexArrayAttrib(m_VertexArrayObjectID, attribLocation);			
			}
		}
	}
}

void AOGL_07_Compute::OnUpdate() {
	// 3. Binds compute shader uniforms
	{
		glUseProgram(m_ComputeShaderProgram);

		const int textureUnit = 0;
		const int textureLod = 0;
		const GLboolean bIsTextureLayered = GL_FALSE;
		const int layer = 0;
		const GLenum textureAccessPermission = GL_READ_WRITE;
		const GLenum textureInternalFormat = GL_RGBA32F;
		glBindImageTexture(textureUnit, m_OutputTextureID, textureLod, bIsTextureLayered, layer, textureAccessPermission, textureInternalFormat);

		const unsigned int textureUniformLocation = glGetUniformLocation(m_ComputeShaderProgram, "outputImage");
		glUniform1iv(textureUniformLocation, 1, &textureUnit);
	}
	
	// 4. Executes compute shader
	// NOTE: In this example, we have one work group for each 2x2 grid of the image, so we divide each dimension of the image by 2
	{
		const glm::vec3 numWorkGroups { m_OutputSize.x / 2.f, m_OutputSize.y / 2.f, 1.f };
		glDispatchCompute(static_cast<int>(numWorkGroups.x), static_cast<int>(numWorkGroups.y), static_cast<int>(numWorkGroups.z));
		
		// Prevents the texture from being read before the compute shader finishes writing to it
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		//glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}

	// (OPTIONAL) 5. Draws texture output from compute shader to a quad
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_TexturedQuadShaderProgram);

		// Binds the texture output from the compute shader to a texture unit
		const int textureUnit = 0;
		glBindTextureUnit(textureUnit, m_OutputTextureID);
		const unsigned int textureUniformLocation = glGetUniformLocation(m_TexturedQuadShaderProgram, "renderTargetTexture");
		glUniform1iv(textureUniformLocation, 1, &textureUnit);
		
		glBindVertexArray(m_VertexArrayObjectID);
		glDrawElements(GL_TRIANGLES, m_QuadData.NumIndices, GL_UNSIGNED_INT, NULL);
	}
}
