#include "Context.h"

#include <spr/OpenGL/RendererContextGL.h>

#include <glad/glad.h>
#include <iostream>

namespace spr {

	void Context::init(const ContextInfo &info) {
		// TODO: Do we really need to pass Context down, or should we keep the global getter?
		// If we intend to keep Context as a singleton, the global getter makes sense, otherwise we should send it down
		m_FrameDataManager.init(this);
		m_ResourceManager.init(this);
		m_RendererContext.init(this, info);
	}

	void Context::shutdown() {
		m_RendererContext.shutdown();
	}

}
