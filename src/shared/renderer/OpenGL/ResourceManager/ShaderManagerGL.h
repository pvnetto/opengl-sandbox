#pragma once

#include "shared/renderer/Handles.h"

namespace spr {

    struct ShaderInstanceGL {
		uint32_t ID;

		void create(unsigned int shaderType, const char *shaderSrc);
		void destroy();
	};

	class ShaderManagerGL {
	public:
		void createShader(const ShaderHandle &handle, unsigned int shaderType, const char *shaderSrc);
		void destroy(ShaderHandle& handle);

	    const ShaderInstanceGL& getShader(const ShaderHandle& handle) const;

	private:
		ShaderInstanceGL m_Shaders[ShaderHandle::capacity];
	};

}