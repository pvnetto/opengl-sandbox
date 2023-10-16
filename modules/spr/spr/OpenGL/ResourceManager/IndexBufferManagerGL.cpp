#include "IndexBufferManagerGL.h"

#include <cassert>
#include <glad/glad.h>

namespace spr {

	IndexBufferInstanceGL IndexBufferInstanceGL::DefaultIndexBuffer = {};

	void IndexBufferInstanceGL::create(const void *data, uint32_t size) {
		IndexCount = size / sizeof(unsigned int);

		glCreateBuffers(1, &ID);
		glNamedBufferData(ID, size, data, GL_STATIC_DRAW);
	}

	void IndexBufferInstanceGL::destroy() {
		glDeleteBuffers(1, &ID);
	}

	void IndexBufferInstanceGL::bind(uint32_t vaoId) const {
		glVertexArrayElementBuffer(vaoId, ID);
	}

	void IndexBufferManagerGL::createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size) {
		m_IndexBuffers[handle.idx].create(data, size);
	}

	void IndexBufferManagerGL::destroy(IndexBufferHandle &handle) {
		m_IndexBuffers[handle.idx].destroy();
	}

	const IndexBufferInstanceGL &IndexBufferManagerGL::getIndexBuffer(const IndexBufferHandle &handle) const {
		if (handle.isValid()) {
			return m_IndexBuffers[handle.idx];
		}
		else {
			return IndexBufferInstanceGL::DefaultIndexBuffer;
		}
	}
}