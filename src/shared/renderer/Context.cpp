#include "Context.h"
#include "Runtime.h"
#include "Programs.h"
#include "shared/Event.h"

#include "OpenGL/Renderer.h"

#include <iostream>
#include <glad/glad.h>

// ===================================================
// ==== Renderer =====================================
// ===================================================
namespace spr {

    struct WindowState {
        const char* Title;
        int Width, Height;

        glm::vec2 LastMousePos;
        bool MouseFirstMove = true;
    };

    /* Window state */
    static WindowState s_State;
    static GLFWwindow* s_Window = nullptr;

    /* Context state */
    static FrameData s_FrameData;
    static DrawCallData s_CachedDrawCallData;


    void init() {
        /* Window init */
        const WindowState &state = *(WindowState *)glfwGetWindowUserPointer(s_Window);

        /* Context init */
        s_FrameData.UniformDataBuffer = UniformDataBuffer::alloc();

        /* Vendor init */
        glViewport(0, 0, state.Width, state.Height);
        glEnable(GL_DEPTH_TEST);

        rendererInit();
    }

    void shutdown() {
        glfwTerminate();
        glfwDestroyWindow(s_Window);
        s_Window = nullptr;
        rendererShutdown();
    }


    void clear() {
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void submit(ProgramHandle& program) {
        s_CachedDrawCallData.Program = program;
        s_CachedDrawCallData.UniformsStart = s_FrameData.lastDrawCallUniformEnd();
        s_CachedDrawCallData.UniformsEnd = s_FrameData.UniformDataBuffer->getPos();
        s_FrameData.DrawCalls.push_back(s_CachedDrawCallData);

        s_CachedDrawCallData.clear();
    }

    void cleanup() {
        s_FrameData.clear();
        s_CachedDrawCallData.clear();
    }

}


namespace spr {

    FrameData& getFrameData() {
        return s_FrameData;
    }

    DrawCallData& getCurrentDrawCallData() {
        return s_CachedDrawCallData;   
    }

    void FrameData::clear() {
        UniformDataBuffer->reset();
        DrawCalls.clear();
    }

    uint32_t FrameData::lastDrawCallUniformEnd() {
        if(DrawCalls.empty()) return 0;
        return DrawCalls[DrawCalls.size() - 1].UniformsEnd;
    }

    void DrawCallData::clear() {
        Program = kInvalidHandle;
        VertexBufferHandle = kInvalidHandle;
        IndexBufferHandle = kInvalidHandle;
        UniformsStart = 0;
        UniformsEnd = 0;
    }

}

// ===================================================
// ==== Windowing ====================================
// ===================================================
namespace spr {

    void createWindow(const char* windowName, int width, int height) {
        if(s_Window) return;

        s_State = WindowState();
        s_State.Title = windowName;
        s_State.Width = width;
        s_State.Height = height;

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        s_Window = glfwCreateWindow(width, height, windowName, NULL, NULL);
        glfwMakeContextCurrent(s_Window);

        glfwSetErrorCallback([](int error, const char *description) {
            std::cout << "::ERROR::GLFW::" << description << "\n";
        });

        glfwSetWindowUserPointer(s_Window, &s_State);
        glfwSetWindowSizeCallback(s_Window, [](GLFWwindow *window, int width, int height) {
            WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);
            state.Width = width;
            state.Height = height;
        });

        glfwSetCursorPosCallback(s_Window, [](GLFWwindow *window, double mouseX, double mouseY) {
            WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

            glm::vec2 mousePos = glm::vec2(mouseX, mouseY);
            if (state.MouseFirstMove) {
                state.LastMousePos = mousePos;
                state.MouseFirstMove = false;
            }

            glm::vec2 mouseDelta = glm::vec2(mousePos.x - state.LastMousePos.x, state.LastMousePos.y - mousePos.y);
            state.LastMousePos = mousePos;
            runtime::handleEvent(MouseMovedEvent(mouseX, mouseY, mouseDelta));
        });

        glfwSetMouseButtonCallback(s_Window, [](GLFWwindow *window, int button, int action, int mods) {
            WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);
            
            switch (action) {
            case GLFW_PRESS:
                runtime::handleEvent(MouseButtonPressedEvent(button));
                break;
            case GLFW_RELEASE:
                runtime::handleEvent(MouseButtonReleasedEvent(button));
                break;
            default:
                break;
            }
        });

        glfwSetKeyCallback(s_Window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
            WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

            switch(action) {
            case GLFW_PRESS:
                runtime::handleEvent(KeyPressedEvent(key));
                break;
            case GLFW_RELEASE:
                break;
            case GLFW_REPEAT:
                runtime::handleEvent(KeyPressedEvent(key));
                break;
            default:
                break;
            }
        });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "::ERROR::GLFW:: Failed to initialize glad\n";
            return;
        }
    }

    void swapBuffers() {
        glfwPollEvents();
		glfwSwapBuffers(s_Window);
    }

    bool isWindowOpen() {
        if(!s_Window) return false;
        return !glfwWindowShouldClose(s_Window);
    }

    GLFWwindow* getWindow() {
        return s_Window;   
    }

    glm::vec2 getWindowSize() {
        return { s_State.Width, s_State.Height };
    }

    int getWindowWidth() {
        return s_State.Width;
    }

    int getWindowHeight() {
        return s_State.Height;
    }

}