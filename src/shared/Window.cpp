#include "Window.h"
#include "shared/layers/ExampleGUILayer.h"

#include <glad/glad.h>
#include <iostream>
#include <algorithm>

static bool s_GLFWInitialized = false;
Window *Window::s_instance = nullptr;

static void GLFWErrorCallback(int error, const char *description) {
	std::cout << "::ERROR::GLFW::" << description << "\n";
}

Window::Window(const WindowProps &props) {
	s_instance = this;

	Init(props);
	m_imGuiLayer = new ImGuiLayer();
	m_imGuiLayer->OnAttach();

	m_exampleLayer = new ExampleGUILayer();
	m_exampleLayer->OnAttach();
}

Window::~Window() {
	Shutdown();
	for (auto layer : m_layers)
		delete layer;
	
	delete m_imGuiLayer;
	m_imGuiLayer = nullptr;
	delete m_exampleLayer;
	m_exampleLayer = nullptr;
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
		if (data.MouseFirstMove) {
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

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

		switch(action) {
		case GLFW_PRESS:
			data.EventCallback(KeyPressedEvent(key));
			break;
		case GLFW_RELEASE:
			break;
		case GLFW_REPEAT:
			data.EventCallback(KeyPressedEvent(key));
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

	if (m_imGuiLayer) {
		m_imGuiLayer->Begin();

		m_exampleLayer->OnImGuiRender();
		for (int i = 0; i < m_layers.size(); i++) {
			m_layers[i]->OnImGuiRender();
		}
		
		m_imGuiLayer->End();
	}

	float currentFrame = (float)glfwGetTime();
	m_deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}

	// Quick hack to make resetting work
	if(m_newLayers.size() > 0) {
		Reset();
		AttachLayer(m_newLayers);
		m_newLayers.clear();
	}
}

void Window::AttachLayer(Layer *layer) {
	m_layers.push_back(layer);
	layer->OnAttach();
}

void Window::AttachLayer(std::vector<Layer *> layers) {
	for (auto layer : layers)
		AttachLayer(layer);
}

void Window::DettachLayer(Layer *layer) {
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end()) {
		Layer *dettached = *it;
		(dettached)->OnDettach();
		m_layers.erase(it);

		delete dettached;
	}
}

void Window::ScheduleReset(std::vector<Layer*> layers) {
	m_newLayers = layers;
}

void Window::Reset() {
	while (m_layers.size() > 0) {
		DettachLayer(m_layers[0]);
	}
}

void Window::HandleEvent(Event &evt) {
	for (auto it = m_layers.end(); it != m_layers.begin();) {
		(*--it)->OnEvent(evt);
		if (evt.m_handled)
			return;
	}
}
