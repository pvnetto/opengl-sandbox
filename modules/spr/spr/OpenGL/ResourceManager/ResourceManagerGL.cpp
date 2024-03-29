#include "ResourceManagerGL.h"

#include "spr/Context.h"

namespace spr {

	void ResourceManagerGL::init(Context* owner) {
		m_Owner = owner;
		m_UniformManager.init(owner);
	}

	void ResourceManagerGL::createVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t byteSize, const VertexAttributeLayout &layout) {
		m_VertexBufferManager.createVertexBuffer(handle, data, byteSize, layout);
	}

	void ResourceManagerGL::createInstancedVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout) {
		m_VertexBufferManager.createInstancedVertexBuffer(handle, data, byteSize, instanceCount, layout);
	}

	void ResourceManagerGL::updateVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t byteSize, uint32_t offset) {
		m_VertexBufferManager.updateVertexBuffer(handle, data, byteSize, offset);
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

	void ResourceManagerGL::createFramebuffer(const FramebufferHandle &handle, const FramebufferAttachmentMap &attachments) {
		FramebufferAttachmentMapGL attachmentMapGl;
		for (const auto [attachmentType, textureHandle] : attachments) {
			attachmentMapGl.emplace(attachmentType, m_TextureManager.getTexture(textureHandle));
		}
		m_FramebufferManager.createFramebuffer(handle, attachmentMapGl);
	}

	void ResourceManagerGL::destroy(FramebufferHandle &handle) {
		m_FramebufferManager.destroy(handle);
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