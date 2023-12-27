#include "GlUtils.h"

#include "Utils.h"

#include <glad/glad.h>

#include <string>
#include <cassert>
#include <stb_image.h>

namespace Utils {

	unsigned int CreateShaderProgramGL(const char *vertexPath, const char *fragmentPath) {
		const std::string vertexSrc = Utils::ReadShaderFile(vertexPath);
		const char *vertexSrcC = vertexSrc.c_str();
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSrcC, NULL);
		glCompileShader(vertexShader);

		int vertSuccess;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
		if (!vertSuccess) {
			char info[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, info);
			assert(false && "::ERROR: Failed to compile Vertex Shader");
			return 0;
		}

		const std::string fragSrc = Utils::ReadShaderFile(fragmentPath);
		const char *fragSrcC = fragSrc.c_str();
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragSrcC, NULL);
		glCompileShader(fragmentShader);

		int fragSuccess;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
		if (!fragSuccess) {
			char info[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, info);
			assert(false && "::ERROR: Failed to compile Fragment Shader");
			return 0;
		}

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		int programSuccess;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
		if (!programSuccess) {
			char info[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, info);
			assert(false && "::ERROR: Failed to link shader program");
			return 0;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}


	unsigned int CreateTextureGL(const char *texturePath) {
		stbi_set_flip_vertically_on_load(true);

		int texWidth, texHeight, numOfChannels;
		unsigned char *textureData = stbi_load(texturePath, &texWidth, &texHeight, &numOfChannels, 0);
		if (!textureData) {
			printf("Couldn't load texture\n");
		}

		unsigned int textureID;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

		const int internalFormat = numOfChannels == 4 ? GL_RGBA8 : GL_RGB8;
		const int format = numOfChannels == 4 ? GL_RGBA : GL_RGB;
		glTextureStorage2D(textureID, 1, internalFormat, texWidth, texHeight);
		glTextureSubImage2D(textureID, 0, 0, 0, texWidth, texHeight, format, GL_UNSIGNED_BYTE, textureData);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(textureData);

		stbi_set_flip_vertically_on_load(false);
		return textureID;
	}
}