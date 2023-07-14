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

    /* Submits draw call data to renderer */
    void submit(ProgramHandle& program);
    /* Renders all draw calls submitted to renderer */
    void render();
    /* Cleans current frame data. This should be called after rendering a frame */
    void cleanup();

}

namespace spr {

    struct DrawCallData {
        ProgramHandle Program;
        VertexBufferHandle VertBufferHandle;
        IndexBufferHandle IndBufferHandle;

        uint32_t UniformsStart;
        uint32_t UniformsEnd;

        void clear();
    };

    struct FrameData {
        UniformDataBufferPtr UniformDataBuffer;
        std::vector<DrawCallData> DrawCalls;

        void clear();
        uint32_t lastDrawCallUniformEnd();
    };
    
    FrameData& getFrameData();
    DrawCallData& getCurrentDrawCallData();

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
