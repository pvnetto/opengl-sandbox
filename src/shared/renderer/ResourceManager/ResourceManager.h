#pragma once

#include "UniformManager.h"
#include "shared/renderer/Handles.h"

namespace spr {
	class Context;
}

namespace spr {

	class ResourceManager {
	public:
		void init(Context *owner);

		IndexBufferHandle createIndexBuffer(const void *data, uint32_t size);		
		void destroy(IndexBufferHandle &handle);

		UniformHandle createUniform(const char *name, UniformType type);
		void destroy(UniformHandle &uniformHandle);

	public:
		inline const UniformManager &getUniformManager() const { return m_UniformManager; }

	private:
		Context *m_Owner = nullptr;

		HandleGenerator<IndexBufferHandle> m_IndexBufferHandles;
		
		UniformManager m_UniformManager;
	};

}