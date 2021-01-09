#include "Window.h"
#include <glad/glad.h>
#include <iostream>

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char *description) {
	std::cout << "::ERROR::GLFW::" << description << "\n";
}

Window::Window(const WindowProps &props) {
	Init(props);
}

Window::~Window() {
	Shutdown();
}

void Window::Init(const WindowProps &props) {
	m_windowData.Title = props.Title;
	m_windowData.Width = props.Width;
	m_windowData.Height = props.Height;

	if (!s_GLFWInitialized) {
		int success = glfwInit();

		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, &m_windowData);
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;
    });

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize Glad :(\n";
		return;
	}
}

void Window::Shutdown() {
    glfwDestroyWindow(m_window);
}