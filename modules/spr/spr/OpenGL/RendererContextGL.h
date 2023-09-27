#pragma once

#include "ResourceManager/ResourceManagerGL.h"

#include <cstdint>

namespace spr {
	struct FrameData;
	struct ContextInfo;

	class RendererContextGL {
	public:
		void init(Context *context, const ContextInfo& info);
		void shutdown();

		void clear();
		void render(const FrameData& frameData);

	private:
		void setUniforms(UniformInfoBufferPtr uniformInfoBuffer);


	public:
		inline ResourceManagerGL &getResourceManager() { return m_ResourceManager; }

	private:
		uint32_t m_DefaultVAO;

		ResourceManagerGL m_ResourceManager;
	};
}
