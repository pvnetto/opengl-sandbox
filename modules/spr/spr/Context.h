#pragma once

#include "Handles.h"
#include "FrameDataManager.h"

#include "OpenGL/RendererContextGL.h"
#include "ResourceManager/ResourceManager.h"

namespace spr {

	using GlLoaderResult = void(*)();
	using GlLoaderFn = GlLoaderResult(*)(const char*);

	struct ContextInfo {
		int Width = 0;
		int Height = 0;

		GlLoaderFn GlLoader = nullptr;
	};

    class Context {
	public:
		void init(const ContextInfo& info);
		void shutdown();

		inline FrameDataManager &getFrameDataManager() { return m_FrameDataManager; }
		inline ResourceManager &getResourceManager() { return m_ResourceManager; }
		inline RendererContextGL &getRendererContext() { return m_RendererContext; }

    private:
		FrameDataManager m_FrameDataManager;
		ResourceManager m_ResourceManager;
		RendererContextGL m_RendererContext;
    };
}
