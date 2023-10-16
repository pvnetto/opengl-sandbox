#include "VertexBufferManagerGL.h"

#include <cassert>
#include <glad/glad.h>

namespace spr {

	void VertexBufferInstanceGL::create(const void *data, uint32_t size, const VertexAttributeLayoutHandle &layoutHandle) {
		Size = size;
		LayoutHandle = layoutHandle;

		glCreateBuffers(1, &ID);
		glNamedBufferStorage(ID, size, data, 0);		// Immutable storage (read-only, as no flag was specified)
	}

	void VertexBufferInstanceGL::destroy() {
		glDeleteBuffers(1, &ID);
		LayoutHandle = kInvalidHandle;
	}

}

namespace spr {

	void VertexBufferManagerGL::createVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t size, const VertexAttributeLayout &layout) {
		VertexAttributeLayoutHandle layoutHandle = findOrCreateVertexAttributeLayout(layout);
		m_VertexBuffers[handle.idx].create(data, size, layoutHandle);
	}

	void VertexBufferManagerGL::destroy(VertexBufferHandle &handle) {
		m_VertexBuffers[handle.idx].destroy();
	}

	const VertexBufferInstanceGL &VertexBufferManagerGL::getVertexBuffer(const VertexBufferHandle handle) const {
		assert(handle.isValid() && "::ERROR: Invalid index buffer");
		return m_VertexBuffers[handle.idx];
	}

}