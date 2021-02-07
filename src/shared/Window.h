#pragma once

#include "layers/Layer.h"
#include "ImGui/ImGuiLayer.h"
#include "Event.h"

#include <functional>
#include <GLFW/glfw3.h>
#include <string>

struct WindowProps {
	std::string Title;
	int Width, Height;

	WindowProps(const std::string &title = "OpenGL Sandbox", int width = 1280, int height = 720)
	    : Title(title), Width(width), Height(height) {}
};

class Window {
public:
    static Window* s_instance;

    std::vector<Layer*> m_layers;

    using EventCallbackFn = std::function<void(Event &)>;

    Window(const WindowProps &props = WindowProps());
    ~Window();

    void OnUpdate();
    void HandleEvent(Event& evt);

    inline float DeltaTime() { return m_deltaTime; }
    inline int GetWidth() { return m_windowData.Width; }
    inline int GetHeight() { return m_windowData.Height; }
    inline glm::vec2 GetSize() { return { m_windowData.Width, m_windowData.Height }; }

    GLFWwindow* GetNativeWindow() { return m_window; }

    void AttachLayer(Layer* layer);
    void AttachLayer(std::vector<Layer*> layers);
    void DettachLayer(Layer* layer);
    
    void ScheduleReset(std::vector<Layer*> layers);

	inline static Window* Get() { return s_instance; }

private:
    void Init(const WindowProps& props);
    void Shutdown();
    void Reset();

private:
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        EventCallbackFn EventCallback;

        // Mouse state
        glm::vec2 LastMousePos;
        bool MouseFirstMove = true;
    };

    GLFWwindow* m_window;
    WindowData m_windowData;
    ImGuiLayer* m_imGuiLayer;
    Layer* m_exampleLayer;

    float lastFrame = 0;
    float m_deltaTime = 0;

    std::vector<Layer*> m_newLayers;
};
