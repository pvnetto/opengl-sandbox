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
		void render(const FrameData& frameData);

	private:
		void processDrawCalls(const FrameData &frameData);
		void processBlitRequests(const FrameData &frameData);

		void applyColorState(const struct spr::ColorBufferState &state);
		void applyDepthState(const struct spr::DepthBufferState &state);
		void applyStencilState(const struct spr::StencilBufferState &state);
		void setUniforms(UniformInfoBufferPtr uniformInfoBuffer);


	public:
		inline ResourceManagerGL &getResourceManager() { return m_ResourceManager; }

	private:
		uint32_t m_DefaultVAO;

		ResourceManagerGL m_ResourceManager;
	};
}
