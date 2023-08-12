#include "ResourceManagerGL.h"

#include "shared/renderer/Context.h"

namespace spr {

	void ResourceManagerGL::init(Context* owner) {
		m_Owner = owner;
	}

	void ResourceManagerGL::createVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t size, const VertexAttributeLayout &layout) {
		m_VertexBufferManager.createVertexBuffer(handle, data, size, layout);
	}

	void ResourceManagerGL::destroy(VertexBufferHandle &handle) {
		m_VertexBufferManager.destroy(handle);
	}

	void ResourceManagerGL::createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size) {
		m_IndexBufferManager.createIndexBuffer(handle, data, size);
	}

	void ResourceManagerGL::destroy(IndexBufferHandle &handle) {
		m_IndexBufferManager.destroy(handle);
	}

	void ResourceManagerGL::createUniform(const UniformHandle &handle, const UniformRef &uniformRef) {
		m_UniformManager.createUniform(handle, uniformRef);
	}

	void ResourceManagerGL::destroy(UniformHandle &uniformHandle) {
		m_UniformManager.destroy(uniformHandle);
	}

	void ResourceManagerGL::createShader(const ShaderHandle &handle, unsigned int shaderType, const char *shaderSrc) {
		m_ShaderManager.createShader(handle, shaderType, shaderSrc);
	}

	void ResourceManagerGL::destroy(ShaderHandle &shaderHandle) {
		m_ShaderManager.destroy(shaderHandle);
	}

	void ResourceManagerGL::createProgram(const ProgramHandle &programHandle, ShaderHandle &vertexHandle, ShaderHandle &fragmentHandle) {
		const ShaderInstanceGL &vertexShader = m_ShaderManager.getShader(vertexHandle);
		const ShaderInstanceGL &fragmentShader = m_ShaderManager.getShader(fragmentHandle);
		m_ProgramManager.createProgram(programHandle, vertexShader, fragmentShader, m_Owner->getResourceManager().getUniformManager());
	}

	void ResourceManagerGL::destroy(ProgramHandle &programHandle) {
		m_ProgramManager.destroy(programHandle);
	}

	void ResourceManagerGL::createTexture(const TextureHandle &handle, const struct TextureInfo &textureInfo, const void *data) {
		m_TextureManager.createTexture(handle, textureInfo, data);
	}

	void ResourceManagerGL::destroy(TextureHandle &handle) {
		m_TextureManager.destroy(handle);
	}

}