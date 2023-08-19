#pragma once

#include "UniformType.h"
#include "shared/renderer/Utils.h"
#include "shared/renderer/Handles.h"

#include <string>
#include <unordered_map>

namespace spr {

	class Context;

	enum class UniformType;
	template <typename T>
	class UniformBuffer;

	using UniformDataBuffer = Buffer;
	using UniformDataBufferPtr = std::shared_ptr<UniformDataBuffer>;

	using UniformHashMap = std::unordered_map<std::string, HandleType>;

	struct UniformRef {
		std::string Name;
		UniformType Type;
		uint32_t Count;
	};

	class UniformManager {
	public:
		void init(Context *context);

		UniformHandle createUniform(const char *name, UniformType type, uint32_t count);
		void destroy(UniformHandle &uniformHandle);

		const UniformRef &getUniform(const UniformHandle &handle) const;
		UniformHandle getUniformByName(const char *name) const;
	
	private:
		HandleGenerator<UniformHandle> m_UniformHandles;

		UniformRef m_Uniforms[UniformHandle::capacity];

		/* Optimizes Uniform lookup by caching them in a hashmap. */
		UniformHashMap m_UniformHashMap;

	private:
		Context *m_Owner;
	};

}