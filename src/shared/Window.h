#pragma once

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
    Window(const WindowProps &props = WindowProps());
    ~Window();

    void OnUpdate();

    inline int GetWidth() { return m_windowData.Width; }
    inline int GetHeight() { return m_windowData.Height; }

    GLFWwindow* GetNativeWindow() { return m_window; }

private:
    void Init(const WindowProps& props);
    void Shutdown();

private:
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
    };

    GLFWwindow* m_window;
    WindowData m_windowData; 
};
