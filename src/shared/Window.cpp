#include "Window.h"
#include <glad/glad.h>
#include <iostream>

static bool s_GLFWInitialized = false;
float Window::deltaTime = 0;

static void GLFWErrorCallback(int error, const char *description) {
	std::cout << "::ERROR::GLFW::" << description << "\n";
}

Window::Window(const WindowProps &props) {
	Init(props);
}

Window::~Window() {
	for (auto layer : m_layers)
		delete layer;
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

	m_windowData.EventCallback = std::bind(&Window::HandleEvent, this, std::placeholders::_1);

	glfwSetWindowUserPointer(m_window, &m_windowData);
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double mouseX, double mouseY) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		
		glm::vec2 mousePos = glm::vec2(mouseX, mouseY);
		if(data.MouseFirstMove) {
			data.LastMousePos = mousePos;
			data.MouseFirstMove = false;
		}

		glm::vec2 mouseDelta = glm::vec2(mousePos.x - data.LastMousePos.x, data.LastMousePos.y - mousePos.y);
		data.LastMousePos = mousePos;
		data.EventCallback(MouseMovedEvent(mouseX, mouseY, mouseDelta));
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		switch (action) {
		case GLFW_PRESS:
			data.EventCallback(MouseButtonPressedEvent(button));
			break;
		case GLFW_RELEASE:
			data.EventCallback(MouseButtonReleasedEvent(button));
			break;
		default:
			break;
		}
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize Glad :(\n";
		return;
	}
}

void Window::Shutdown() {
	glfwDestroyWindow(m_window);
}

void Window::OnUpdate() {
	for (Layer *layer : m_layers)
		layer->OnUpdate();
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Window::AttachLayer(Layer *layer) {
	m_layers.push_back(layer);
	layer->OnAttach();
}

void Window::DettachLayer(Layer *layer) {
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end()) {
		m_layers.erase(it);
		(*it)->OnDettach();
	}
}

void Window::HandleEvent(Event &evt) {
	for (auto it = m_layers.end(); it != m_layers.begin();) {
		(*--it)->OnEvent(evt);
		if (evt.m_handled)
			return;
	}
}
