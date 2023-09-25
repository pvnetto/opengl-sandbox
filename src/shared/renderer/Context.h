#pragma once

#include "Handles.h"
#include "FrameDataManager.h"

#include "OpenGL/RendererContextGL.h"
#include "ResourceManager/ResourceManager.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace spr {
    class Context {
	public:
		void init();
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

// ===================================================
// ==== Windowing ====================================
// ===================================================
namespace spr {

    void createWindow(const char* windowName, int width, int height);
    void swapBuffers();
    bool isWindowOpen();

    GLFWwindow* getWindow();

    glm::vec2 getWindowSize();
    int getWindowWidth();
    int getWindowHeight();

    glm::vec2 getMousePos();

}
