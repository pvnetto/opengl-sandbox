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
	using UniformInfoBuffer = Buffer;
	using UniformDataBufferPtr = std::shared_ptr<UniformDataBuffer>;
	using UniformInfoBufferPtr = std::shared_ptr<UniformInfoBuffer>;

	using UniformHashMap = std::unordered_map<std::string, HandleType>;

	struct UniformRef {
		std::string Name;
		UniformType Type;
	};

	class UniformManager {
	public:
		void init(Context *context);

		UniformHandle createUniform(const char *name, UniformType type);
		void destroy(UniformHandle &uniformHandle);

		const UniformRef &getUniform(const UniformHandle &handle) const;
		UniformHandle getUniformByName(const char *name) const;
	
	private:
		HandleGenerator<UniformHandle> m_UniformHandles;

		UniformRef m_Uniforms[UniformHandle::capacity];
		UniformHashMap m_UniformHashMap;

	private:
		Context *m_Owner;
	};

}