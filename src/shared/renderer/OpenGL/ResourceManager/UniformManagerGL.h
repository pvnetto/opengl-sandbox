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
		void init(Context *owner);

		void createUniform(const UniformHandle& handle, const UniformRef &uniformRef);
		void destroy(UniformHandle &handle);

		void setUniformValue(const UniformHandle &handle, const void *data, uint32_t size);
		void setUniformValues(UniformDataBufferPtr uniformDataBuffer, uint32_t start, uint32_t end);
		
		template <typename T>
		T *getUniformValue(const UniformHandle &handle, uint8_t index = 0) {
			const UniformManager &uniformManager = m_Owner->getResourceManager().getUniformManager();
			const UniformRef &uniform = uniformManager.getUniform(handle);

			T *value = static_cast<T *>(m_UniformValues[handle.idx]);
			return value + index * spr::getUniformSizeByType(uniform.Type);
		}

	private:
		UniformRegistry m_UniformRegistry;
		// TODO: This could end up leaking memory. I should use a better container to handle this (array<variant>?)
		void *m_UniformValues[UniformHandle::capacity];

	private:
		Context *m_Owner;
	};

}