#pragma once

#include "UniformManager.h"
#include "shared/renderer/Handles.h"

namespace spr {
	class Context;
	class VertexAttributeLayout;
}

namespace spr {

	class ResourceManager {
	public:
		void init(Context *owner);

		VertexBufferHandle createVertexBuffer(const void *data, uint32_t size, const VertexAttributeLayout &layout);		
		void destroy(VertexBufferHandle &handle);

		IndexBufferHandle createIndexBuffer(const void *data, uint32_t size);		
		void destroy(IndexBufferHandle &handle);

		UniformHandle createUniform(const char *name, UniformType type, uint32_t count);
		void destroy(UniformHandle &handle);

		ShaderHandle createShader(unsigned int shaderType, const char *shaderSrc);
		void destroy(ShaderHandle &handle);

		ProgramHandle createProgram(ShaderHandle &vertexHandle, ShaderHandle &fragmentHandle, bool destroyShaders = true);
		void destroy(ProgramHandle &handle);

		TextureHandle createTexture(const struct TextureInfo& textureInfo, const void *data);
		void destroy(TextureHandle &handle);

	public:
		inline const UniformManager &getUniformManager() const { return m_UniformManager; }

	private:
		Context *m_Owner = nullptr;

		HandleGenerator<VertexBufferHandle> m_VertexBufferHandles;
		HandleGenerator<IndexBufferHandle> m_IndexBufferHandles;
		HandleGenerator<ShaderHandle> m_ShaderHandles;
		HandleGenerator<ProgramHandle> m_ProgramHandles;
		HandleGenerator<TextureHandle> m_TextureHandles;
		
		UniformManager m_UniformManager;
	};

}