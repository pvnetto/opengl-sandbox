#include "ResourceManager.h"
#include "spr/Context.h"
#include "spr/ResourceManager/VertexAttributeLayout.h"

namespace spr {

	void ResourceManager::init(Context *owner) {
		m_Owner = owner;
		m_UniformManager.init(owner);
	}

	VertexBufferHandle ResourceManager::createVertexBuffer(const void *data, uint32_t byteSize, const VertexAttributeLayout &layout) {
		VertexBufferHandle handle = m_VertexBufferHandles.allocHandle();
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createVertexBuffer(handle, data, byteSize, layout);
		return handle;
	}

	VertexBufferHandle ResourceManager::createInstancedVertexBuffer(const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout) {
		VertexBufferHandle handle = m_VertexBufferHandles.allocHandle();
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createInstancedVertexBuffer(handle, data, byteSize, instanceCount, layout);
		return handle;
	}

	void ResourceManager::updateVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t offset) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.updateVertexBuffer(handle, data, byteSize, offset);
	}

	void ResourceManager::destroy(VertexBufferHandle &handle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.destroy(handle);
		m_VertexBufferHandles.removeHandle(handle);
	}

	IndexBufferHandle ResourceManager::createIndexBuffer(const void *data, uint32_t size) {
		IndexBufferHandle handle = m_IndexBufferHandles.allocHandle();
		auto& rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createIndexBuffer(handle, data, size);
		return handle;
	}

	void ResourceManager::destroy(IndexBufferHandle &handle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.destroy(handle);
		m_IndexBufferHandles.removeHandle(handle);
	}

	FramebufferHandle ResourceManager::createFramebuffer(const FramebufferAttachmentMap &attachments) {
		FramebufferHandle handle = m_FramebufferHandles.allocHandle();
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createFramebuffer(handle, attachments);
		return handle;
	}

	void ResourceManager::destroy(FramebufferHandle &handle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.destroy(handle);
		m_FramebufferHandles.removeHandle(handle);
	}
	
	UniformHandle ResourceManager::createUniform(const char *name, UniformType type, uint32_t count) {
		return m_UniformManager.createUniform(name, type, count);
	}
	
	void ResourceManager::destroy(UniformHandle &uniformHandle) {
		m_UniformManager.destroy(uniformHandle);
	}

	ShaderHandle ResourceManager::createShader(unsigned int shaderType, const char *shaderSrc) {
		ShaderHandle handle = m_ShaderHandles.allocHandle();
		auto& rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createShader(handle, shaderType, shaderSrc);
		return handle;
	}

	void ResourceManager::destroy(ShaderHandle &handle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.destroy(handle);
		m_ShaderHandles.removeHandle(handle);
	}

	ProgramHandle ResourceManager::createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders) {
		ProgramHandle handle = m_ProgramHandles.allocHandle();
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createProgram(handle, vertexHandle, fragmentHandle);

		if (destroyShaders) {
			destroy(vertexHandle);
			destroy(fragmentHandle);
		}
		return handle;
	}

	void ResourceManager::destroy(ProgramHandle& handle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.destroy(handle);
		m_ProgramHandles.removeHandle(handle);
	}

	TextureHandle ResourceManager::createTexture(const struct TextureInfo &textureInfo, const void *data) {
		TextureHandle handle = m_TextureHandles.allocHandle();
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createTexture(handle, textureInfo, data);
		return handle;
	}

	void ResourceManager::destroy(TextureHandle &handle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.destroy(handle);
		m_TextureHandles.removeHandle(handle);
	}

}