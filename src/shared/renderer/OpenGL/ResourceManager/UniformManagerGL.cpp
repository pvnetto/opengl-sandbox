#include "UniformManagerGL.h"

#include <cassert>
#include <cstring>
#include <glad/glad.h>
#include <string>
#include <unordered_map>


namespace spr {

	void UniformManagerGL::createUniform(const UniformHandle& handle, const UniformRef &uniformRef) {
		uint32_t size = spr::getUniformSizeByType(uniformRef.Type);

		// Initializes empty block of data
		void *data = malloc(size);
		memset(data, 0, size);

		// Registers uniform and its data
		m_UniformValues[handle.idx] = data;
		m_UniformRegistry.add(handle, uniformRef.Name.c_str());
	}

	void UniformManagerGL::destroy(UniformHandle &uniformHandle) {
		// TODO: Free uniform data from memory
		m_UniformRegistry.remove(uniformHandle);
	}

	void UniformManagerGL::setUniformValue(const UniformHandle &handle, const void *data, uint32_t size) {
		memcpy(m_UniformValues[handle.idx], data, size);
	}

	void UniformManagerGL::setUniformValues(UniformDataBufferPtr uniformDataBuffer, uint32_t start, uint32_t end) {
		uniformDataBuffer->reset(start);

		while (uniformDataBuffer->getPos() != end) {
			UniformHandle handle = uniformDataBuffer->read<UniformHandle>();
			UniformType type = uniformDataBuffer->read<UniformType>();

			const uint32_t uniformSize = getUniformSizeByType(type);
			void *data = uniformDataBuffer->read(uniformSize);
			setUniformValue(handle, data, uniformSize);
		}
	}

}


namespace spr {

	const UniformHandle *UniformRegistry::find(const char *name) const {
		std::string nameStr{name};
		if (m_Uniforms.count(nameStr) > 0) {
			HandleType index = m_Uniforms.at(nameStr);
			return &m_Handles[index];
		}

		return nullptr;
	}

	const UniformHandle &UniformRegistry::add(UniformHandle handle, const char *name) {
		assert(handle.isValid() && "::ERROR: Uniform handle is invalid!");

		m_Uniforms.erase({name});
		m_Uniforms[{name}] = handle.idx;

		UniformHandle &outHandle = m_Handles[handle.idx];
		outHandle = handle;
		return outHandle;
	}

	void UniformRegistry::remove(UniformHandle handle) {
		for (const auto &pair : m_Uniforms) {
			if (pair.second == handle.idx) {
				m_Uniforms.erase(pair.first);
				break;
			}
		}
	}

}
