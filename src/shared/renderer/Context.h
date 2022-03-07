#pragma once

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
    void shutdown();

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