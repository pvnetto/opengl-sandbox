#include "VertexBufferManagerGL.h"

#include <cassert>
#include <glad/glad.h>

namespace spr {

	void VertexBufferInstanceGL::create(const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayoutHandle &layoutHandle) {
		ByteSize = byteSize;
		LayoutHandle = layoutHandle;
		InstanceCount = instanceCount;

		// Instanced vertex buffers are dynamic by default, others are not
		const GLbitfield storageFlags = InstanceCount > 0 ? GL_DYNAMIC_STORAGE_BIT : 0;
		glCreateBuffers(1, &ID);
		glNamedBufferStorage(ID, byteSize, data, storageFlags);
	}

	void VertexBufferInstanceGL::update(const void *data, uint32_t byteSize, uint32_t offset) {
		assert(InstanceCount > 0 && "spr::ERROR: For now, we only support dynamic storage for instanced vertex buffers");
		glNamedBufferSubData(ID, offset, byteSize, data);
	}

	void VertexBufferInstanceGL::destroy() {
		glDeleteBuffers(1, &ID);
		LayoutHandle = k_InvalidHandle;
	}

}

namespace spr {

	void VertexBufferManagerGL::createVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, const VertexAttributeLayout &layout) {
		constexpr uint32_t instanceCount = 0;
		const VertexAttributeLayoutHandle layoutHandle = findOrCreateVertexAttributeLayout(layout);
		m_VertexBuffers[handle.idx].create(data, byteSize, instanceCount, layoutHandle);
	}

	void VertexBufferManagerGL::createInstancedVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout) {
		const VertexAttributeLayoutHandle layoutHandle = findOrCreateVertexAttributeLayout(layout);
		m_VertexBuffers[handle.idx].create(data, byteSize, instanceCount, layoutHandle);
	}

	void VertexBufferManagerGL::updateVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t offset) {
		m_VertexBuffers[handle.idx].update(data, byteSize, offset);
	}

	void VertexBufferManagerGL::destroy(VertexBufferHandle &handle) {
		m_VertexBuffers[handle.idx].destroy();
	}

	const VertexBufferInstanceGL &VertexBufferManagerGL::getVertexBuffer(const VertexBufferHandle handle) const {
		assert(handle.isValid() && "::ERROR: Invalid index buffer");
		return m_VertexBuffers[handle.idx];
	}

}