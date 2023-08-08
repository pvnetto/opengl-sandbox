#pragma once

#include "ResourceManager/ResourceManagerGL.h"

#include <cstdint>

namespace spr {
	struct FrameData;

	class RendererContextGL {
	public:
		void init();
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
