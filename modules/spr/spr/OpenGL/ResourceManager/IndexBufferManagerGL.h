#pragma once

#include <spr/Handles.h>

namespace spr {

	struct IndexBufferInstanceGL {
		uint32_t ID = 0;
		uint32_t IndexCount = 0;

		static IndexBufferInstanceGL DefaultIndexBuffer;

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