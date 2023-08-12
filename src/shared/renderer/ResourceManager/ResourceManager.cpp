#include "ResourceManager.h"
#include "shared/renderer/Context.h"
#include "shared/renderer/ResourceManager/VertexAttributeLayout.h"

namespace spr {

	void ResourceManager::init(Context *owner) {
		m_Owner = owner;
		m_UniformManager.init(owner);
	}

	VertexBufferHandle ResourceManager::createVertexBuffer(const void *data, uint32_t size, const VertexAttributeLayout &layout) {
		VertexBufferHandle handle = m_VertexBufferHandles.allocHandle();
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		rendererResourceManager.createVertexBuffer(handle, data, size, layout);
		return handle;
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

	
	UniformHandle ResourceManager::createUniform(const char *name, UniformType type) {
		return m_UniformManager.createUniform(name, type);
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