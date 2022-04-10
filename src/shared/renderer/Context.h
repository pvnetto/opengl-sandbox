#pragma once

#include "Uniforms.h"
#include "Handles.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


// ===================================================
// ==== Renderer =====================================
// ===================================================
namespace spr {

    /* Initializes renderer */
    void init();
    void shutdown();

    /* Clears framebuffer data */
    void clear();

    /* Submits data to renderer */
    void submit(ProgramHandle& program);
    /* Renders primitives submitted to renderer */
    void render();
    /* Cleans current frame data. This should be called after rendering a frame */
    void cleanup();

}

namespace spr {

    struct RenderItem {
        ProgramHandle Program;
        uint32_t UniformsStart = 0;
        uint32_t UniformsEnd = 0;
    };

    struct FrameData {
        UniformDataBufferPtr UniformDataBuffer;
        std::vector<RenderItem> RenderItems;

        uint32_t nextRenderItemUniformStart();
    };

    FrameData& getFrameData();

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

}