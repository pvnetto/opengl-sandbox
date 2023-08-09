#include "UniformManager.h"

#include "shared/renderer/Context.h"

#include <unordered_map>
#include <string>
#include <cstring>
#include <cassert>
#include <glad/glad.h>

namespace spr {

	void UniformManager::init(Context *context) {
		m_Owner = context;
	}

	UniformHandle UniformManager::createUniform(const char* name, UniformType type) {
		HandleType index = kInvalidHandle;
		UniformRef *uniformRef = nullptr;
		UniformHandle handle;

		if (m_UniformHashMap.count({name}) > 0) {
			handle.idx = m_UniformHashMap[name];
			uniformRef = &m_Uniforms[handle.idx];
			assert(uniformRef->Type == type && "::ERROR: Uniform type mismatch");
		}
		else {
			handle = m_UniformHandles.allocHandle();
			uniformRef = &m_Uniforms[handle.idx];
			uniformRef->Name = name;
			uniformRef->Type = type;

			// Adds ref index to hashmap
			m_UniformHashMap[{name}] = handle.idx;
		}

		auto& rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		auto &rendererUniformManager = rendererResourceManager.getUniformManager();
		rendererUniformManager.createUniform(handle, *uniformRef);
		return handle;
	}

	void UniformManager::destroy(UniformHandle& uniformHandle) {
		auto &rendererResourceManager = m_Owner->getRendererContext().getResourceManager();
		auto &rendererUniformManager = rendererResourceManager.getUniformManager();
		rendererUniformManager.destroy(uniformHandle);
		
		const UniformRef &uniform = m_Uniforms[uniformHandle.idx];
		m_UniformHashMap.erase(uniform.Name);
		m_UniformHandles.removeHandle(uniformHandle);
	}

	const UniformRef &UniformManager::getUniform(const UniformHandle &handle) const {
		assert(handle.isValid() && "::ERROR: Invalid uniform");
		return m_Uniforms[handle.idx];
	}

	// TODO: Worst case here is terrible. Maybe we should use UniformReg to handle this (check UniformManagerGL)
	UniformHandle UniformManager::getUniformByName(const char *name) const {
		for (uint32_t i = 0; i < UniformHandle::capacity; i++) {
			if (m_Uniforms[i].Name == name) {
				return { i };
			}
		}

		assert(false && "::ERROR: Uniform doesn't exist. You should call spr::createUniform before calling this.");
		return { kInvalidHandle };
	}

}