#include "SimpleRenderer.h"

#include "Event.h"
#include "layers/Layer.h"
#include "layers/ExampleGUILayer.h"
#include "layers/SandboxLayer.h"
#include "ImGui/ImGuiLayer.h"

#include <iostream>
#include <functional>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace spr {

    struct WindowState {
        const char* Title;
        int Width, Height;

        glm::vec2 LastMousePos;
        bool MouseFirstMove = true;
    };

    static GLFWwindow* s_Window = nullptr;
    static WindowState s_State;

    static void handleEvent(Event &evt);

    void init() {
        const WindowState &state = *(WindowState *)glfwGetWindowUserPointer(s_Window);

        glViewport(0, 0, state.Width, state.Height);
        glEnable(GL_DEPTH_TEST);
    }

    void clear() {
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void shutdown() {
        glfwDestroyWindow(s_Window);
        s_Window = nullptr;
    }

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
            handleEvent(MouseMovedEvent(mouseX, mouseY, mouseDelta));
        });

        glfwSetMouseButtonCallback(s_Window, [](GLFWwindow *window, int button, int action, int mods) {
            WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);
            
            switch (action) {
            case GLFW_PRESS:
                handleEvent(MouseButtonPressedEvent(button));
                break;
            case GLFW_RELEASE:
                handleEvent(MouseButtonReleasedEvent(button));
                break;
            default:
                break;
            }
        });

        glfwSetKeyCallback(s_Window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
            WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

            switch(action) {
            case GLFW_PRESS:
                handleEvent(KeyPressedEvent(key));
                break;
            case GLFW_RELEASE:
                break;
            case GLFW_REPEAT:
                handleEvent(KeyPressedEvent(key));
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

}


namespace spr { namespace runtime {
    
    static std::vector<Layer*>  s_Layers;
    static std::vector<Layer*>  s_NewLayers;
    static ImGuiLayer*          s_ImGuiLayer    = nullptr;
    static ExampleGUILayer*     s_ExampleLayer  = nullptr;

    static bool s_InitializedRuntime = false;

    float s_DeltaTime = 0.f;

    void attachLayer(Layer* layer) {
        s_Layers.push_back(layer);
        layer->OnAttach();
    }

    static void attachLayers(std::vector<Layer*> layers) {
        for(const auto& layer : layers) {
            attachLayer(layer);
        }
    }

    void dettachLayer(Layer* layer) {
        auto it = std::find(s_Layers.begin(), s_Layers.end(), layer);
        if (it != s_Layers.end()) {
            Layer *dettached = *it;
            (dettached)->OnDettach();
            s_Layers.erase(it);

            delete dettached;
        }
    }

    static void dettachAllLayers() {
        while (s_Layers.size() > 0) {
            dettachLayer(s_Layers[0]);
        }
    }

    static void initializeLayers() {
        s_InitializedRuntime = true;
        s_ExampleLayer = new ExampleGUILayer();
        s_ImGuiLayer = new ImGuiLayer();

        attachLayer(new SandboxLayer());
    }

    void update() {
        static float lastFrame = 0.f;

        if(!s_InitializedRuntime) {
            initializeLayers();
        }

        // Executes layers
        for (Layer *layer : s_Layers)
            layer->OnUpdate();

        // Executes ImGui layers
        if (s_ImGuiLayer) {
            s_ImGuiLayer->Begin();

            s_ExampleLayer->OnImGuiRender();
            for (int i = 0; i < s_Layers.size(); i++) {
                s_Layers[i]->OnImGuiRender();
            }
            
            s_ImGuiLayer->End();
        }

        // Calculates delta time
        float currentFrame = (float)glfwGetTime();
        s_DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handles window close event
        if (glfwGetKey(s_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(s_Window, true);
        }

        // Quick hack to make resetting work
        if(s_NewLayers.size() > 0) {
            dettachAllLayers();
            attachLayers(s_NewLayers);
            s_NewLayers.clear();
        }   
    }
}
}

namespace spr {

    void handleEvent(Event &evt) {
        for (auto it = runtime::s_Layers.end(); it != runtime::s_Layers.begin();) {
            (*--it)->OnEvent(evt);
            if (evt.m_handled)
                return;
        }
    }

}