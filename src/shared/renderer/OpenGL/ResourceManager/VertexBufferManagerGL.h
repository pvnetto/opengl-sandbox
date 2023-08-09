#pragma once

#include "shared/renderer/VertexAttributeLayout.h"

namespace spr {

    struct VertexBufferInstanceGL {
		uint32_t ID;
		uint32_t Size;
		VertexAttributeLayoutHandle LayoutHandle;

		void create(const void *data, uint32_t size, const VertexAttributeLayoutHandle &layoutHandle);
		void destroy();
		void bind() const;
	};

	class VertexBufferManagerGL {
	public:
		void createVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t size, const VertexAttributeLayout &layout);
		void destroy(VertexBufferHandle &handle);

	    const VertexBufferInstanceGL &getVertexBuffer(const VertexBufferHandle &handle) const;

	private:
		VertexBufferInstanceGL m_VertexBuffers[VertexBufferHandle::capacity];
	};

}