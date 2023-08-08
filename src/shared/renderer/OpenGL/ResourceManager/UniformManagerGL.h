#pragma once

#include "shared/renderer/Handles.h"
#include "shared/renderer/ResourceManager/UniformManager.h"

namespace spr {

	class UniformRegistry {
	public:
		UniformRegistry() = default;
		~UniformRegistry() = default;

		const UniformHandle *find(const char *name) const;
		const UniformHandle &add(UniformHandle handle, const char *name);
		void remove(UniformHandle handle);

	private:
		UniformHashMap m_Uniforms;
		UniformHandle m_Handles[UniformHandle::capacity];
	};

	class UniformManagerGL {
	public:
		void createUniform(const UniformHandle& handle, const UniformRef &uniformRef);
		void destroy(UniformHandle &uniformHandle);

		void setUniformValue(const UniformHandle &handle, const void *data, uint32_t size);
		void setUniformValues(UniformDataBufferPtr uniformDataBuffer, uint32_t start, uint32_t end);
		
		template <typename T>
		T *getUniformValue(const UniformHandle &handle) {
			return static_cast<T *>(m_UniformValues[handle.idx]);
		}

	private:
		UniformRegistry m_UniformRegistry;
		// TODO: This is leaking memory FOR SURE. I should use a better container to handle this (array<variant>?)
		// Data in this array persists through frames, so constant uniforms can be set only once.
		void *m_UniformValues[UniformHandle::capacity];
	};

}