#include "ShaderManagerGL.h"

#include "spr/ResourceManager/ShaderType.h"

#include <iostream>
#include <cassert>
#include <glad/glad.h>

namespace spr {

	void ShaderManagerGL::createShader(const ShaderHandle handle, unsigned int shaderType, const char *shaderSrc) {
		m_Shaders[handle.idx].create(shaderType, shaderSrc);
	}

	void ShaderManagerGL::destroy(ShaderHandle &handle) {
		m_Shaders[handle.idx].destroy();
	}

	const ShaderInstanceGL &ShaderManagerGL::getShader(const ShaderHandle handle) const {
		assert(handle.isValid() && "::ERROR: Invalid index buffer");
		return m_Shaders[handle.idx];
	}

	namespace {
		GLuint getGLShaderTypeFromSPR(unsigned int sprShaderType) {
			if (sprShaderType == SPR_VERTEX_SHADER) return GL_VERTEX_SHADER;
			if (sprShaderType == SPR_FRAGMENT_SHADER) return GL_FRAGMENT_SHADER;

			assert(false && "::ERROR: Shader type is not supported");
			return GL_VERTEX_SHADER;
		}
	}

    void ShaderInstanceGL::create(unsigned int shaderType, const char *shaderSrc) {
		unsigned int glShaderType = getGLShaderTypeFromSPR(shaderType);

		ID = glCreateShader(glShaderType);
		glShaderSource(ID, 1, &shaderSrc, NULL);
		glCompileShader(ID);

		int success;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char info[512];
			glGetShaderInfoLog(ID, 512, NULL, info);
			assert(false && "::ERROR: Failed to compile shader");
			std::cout << "::ERROR: Failed to compile shader:: " << info << "\n";
		}
	}

	void ShaderInstanceGL::destroy() {
		glDeleteShader(ID);
		ID = 0;
	}

}