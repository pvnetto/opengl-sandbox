#pragma once

#include "spr/ResourceManager/VertexAttributeLayout.h"

namespace spr {

    struct VertexBufferInstanceGL {
		uint32_t ID;
		uint32_t ByteSize;
		uint32_t InstanceCount = 0;
		VertexAttributeLayoutHandle LayoutHandle;

		void create(const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayoutHandle &layoutHandle);
		void update(const void *data, uint32_t byteSize, uint32_t offset);
		void destroy();
	};

	class VertexBufferManagerGL {
	public:
		void createVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, const VertexAttributeLayout &layout);
		void createInstancedVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout);
		void updateVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t offset);
		void destroy(VertexBufferHandle &handle);

	    const VertexBufferInstanceGL &getVertexBuffer(const VertexBufferHandle handle) const;

	private:
		VertexBufferInstanceGL m_VertexBuffers[VertexBufferHandle::capacity];
	};

}