#include "ResourceManager.h"
#include "shared/renderer/Context.h"

namespace spr {

	void ResourceManager::init(Context *owner) {
		m_Owner = owner;
		m_UniformManager.init(owner);
	}

	IndexBufferHandle ResourceManager::createIndexBuffer(const void *data, uint32_t size) {
		IndexBufferHandle handle = m_IndexBufferHandles.allocHandle();
		auto& rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
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

}