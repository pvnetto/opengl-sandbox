#include "Context.h"
#include "Runtime.h"
#include "shared/Event.h"

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


    void init() {
        /* Window init */
        const WindowState &state = *(WindowState *)glfwGetWindowUserPointer(s_Window);

        /* Context init */
        s_FrameData.UniformDataBuffer = UniformDataBuffer::alloc();

        /* Vendor init */
        glViewport(0, 0, state.Width, state.Height);
        glEnable(GL_DEPTH_TEST);
    }

    void shutdown() {
        glfwTerminate();
        glfwDestroyWindow(s_Window);
        s_Window = nullptr;
    }


    void clear() {
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void submit(ProgramHandle& program) {
        RenderItem renderItem;
        renderItem.Program = program;
        renderItem.UniformsStart = s_FrameData.nextRenderItemUniformStart();
        renderItem.UniformsEnd = s_FrameData.UniformDataBuffer->getPos();

        s_FrameData.RenderItems.push_back(renderItem);
    }

    void render() {
        for(const RenderItem& renderItem : s_FrameData.RenderItems) {
            // TODO: Read all uniforms in [RenderItem::UniformStart, RenderItem::UniformEnd] from UniformBuffer
            // TODO: Copy all read uniforms to persistent uniform data
            // TODO: Read all uniforms idx, loc from Program.ConstantUniformBuffer
            // TODO: Set all uniforms with values from persistent uniform data

            glUseProgram(renderItem.Program.idx);
        }
    }

    void cleanup() {
        s_FrameData.UniformDataBuffer->reset();
        s_FrameData.RenderItems.clear();
    }

}


namespace spr {

    FrameData& getFrameData() {
        return s_FrameData;
    }

    uint32_t FrameData::nextRenderItemUniformStart() {
        if(RenderItems.empty()) return 0;
        return RenderItems[RenderItems.size() - 1].UniformsEnd;
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