#pragma once

#include "IndexBufferManagerGL.h"
#include "UniformManagerGL.h"

namespace spr {

	class ResourceManagerGL {
	public:
		void createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size);
		void destroy(IndexBufferHandle &handle);

		void createUniform(const UniformHandle &handle, const UniformRef &uniformRef);
		void destroy(UniformHandle &uniformHandle);

	public:
		inline const IndexBufferManagerGL &getIndexBufferManager() const { return m_IndexBufferManager; }
		inline const UniformManagerGL &getUniformManager() const { return m_UniformManager; }
		inline UniformManagerGL &getUniformManager() { return m_UniformManager; }

	private:
		IndexBufferManagerGL m_IndexBufferManager;
		UniformManagerGL m_UniformManager;
	};

}