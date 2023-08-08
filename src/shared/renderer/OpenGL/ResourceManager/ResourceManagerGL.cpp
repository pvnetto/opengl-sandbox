#include "ResourceManagerGL.h"

namespace spr {

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

}