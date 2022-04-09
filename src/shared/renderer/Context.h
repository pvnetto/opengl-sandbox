#pragma once

#include "Uniforms.h"
#include "Handles.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


// ===================================================
// ==== Renderer =====================================
// ===================================================
namespace spr {

    void init();
    void clear();
    void submit(ProgramHandle& program);
    void draw();
    void shutdown();

}

namespace spr {

    struct RenderItem {
        ProgramHandle Program;
        uint32_t UniformsStart = 0;
        uint32_t UniformsEnd = 0;
    };

    struct FrameData {
        SimpleUniformBufferPtr UniformBuffer;
        std::vector<RenderItem> RenderItems;    
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