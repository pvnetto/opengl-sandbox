#pragma once

#include "VertexBufferManagerGL.h"
#include "IndexBufferManagerGL.h"
#include "UniformManagerGL.h"

namespace spr {

	class ResourceManagerGL {
	public:
		void createVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t size, const VertexAttributeLayout &layout);
		void destroy(VertexBufferHandle &handle);

		void createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size);
		void destroy(IndexBufferHandle &handle);

		void createUniform(const UniformHandle &handle, const UniformRef &uniformRef);
		void destroy(UniformHandle &uniformHandle);

	public:
		inline const VertexBufferManagerGL &getVertexBufferManager() const { return m_VertexBufferManager; }
		inline const IndexBufferManagerGL &getIndexBufferManager() const { return m_IndexBufferManager; }
		inline const UniformManagerGL &getUniformManager() const { return m_UniformManager; }
		inline UniformManagerGL &getUniformManager() { return m_UniformManager; }

	private:
		VertexBufferManagerGL m_VertexBufferManager;
		IndexBufferManagerGL m_IndexBufferManager;
		UniformManagerGL m_UniformManager;
	};

}