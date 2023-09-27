#pragma once

#include <spr/Handles.h>

namespace spr {

	struct IndexBufferInstanceGL {
		uint32_t ID;
		uint32_t IndexCount;

		void create(const void *data, uint32_t indexCount);
		void destroy();
		void bind(uint32_t vaoId) const;
	};

	class IndexBufferManagerGL {
	public:
		void createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size);
		void destroy(IndexBufferHandle &handle);
		const IndexBufferInstanceGL &getIndexBuffer(const IndexBufferHandle &handle) const;

	private:
		IndexBufferInstanceGL m_IndexBuffers[IndexBufferHandle::capacity];

	};

}