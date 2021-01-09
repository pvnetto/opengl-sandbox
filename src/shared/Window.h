#pragma once

#include "layers/Layer.h"
#include "Event.h"

#include <functional>
#include <GLFW/glfw3.h>
#include <string>

struct WindowProps {
	std::string Title;
	int Width, Height;

	WindowProps(const std::string &title = "OpenGL Sandbox", int width = 800, int height = 600)
	    : Title(title), Width(width), Height(height) {}
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event &)>;

    Window(const WindowProps &props = WindowProps());
    ~Window();

    void OnUpdate();
    void HandleEvent(Event& evt);

    inline int GetWidth() { return m_windowData.Width; }
    inline int GetHeight() { return m_windowData.Height; }

    GLFWwindow* GetNativeWindow() { return m_window; }

    void AttachLayer(Layer* layer);
    void DettachLayer(Layer* layer);

    std::vector<Layer*> m_layers;

    float lastFrame = 0;
    static float deltaTime;

private:
    void Init(const WindowProps& props);
    void Shutdown();

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
};
