#include "Context.h"

#include "Programs.h"
#include "Runtime.h"
#include "shared/Event.h"
#include "shared/renderer/OpenGL/RendererContext.h"

#include <glad/glad.h>
#include <iostream>

namespace spr {

	struct WindowState {
		const char *Title;
		int Width, Height;

		glm::vec2 LastMousePos;
		bool MouseFirstMove = true;
	};

	/* Window state */
	static WindowState s_State;
	static GLFWwindow *s_Window = nullptr;

	void Context::init() {
		m_FrameDataManager.init(this);
		m_ResourceManager.init(this);

		// TODO: Use swapchain to handle viewport size (??), resize according to framebuffer size
		const WindowState &state = *(WindowState *)glfwGetWindowUserPointer(s_Window);
		glViewport(0, 0, state.Width, state.Height);
		m_RendererContext.init();
	}

	void Context::shutdown() {
		glfwTerminate();
		glfwDestroyWindow(s_Window);
		s_Window = nullptr;
		m_RendererContext.shutdown();
	}

}

// ===================================================
// ==== Windowing ====================================
// ===================================================
namespace spr {

	void createWindow(const char *windowName, int width, int height) {
		if (s_Window) return;

		s_State = WindowState();
		s_State.Title = windowName;
		s_State.Width = width;
		s_State.Height = height;

		if (!glfwInit()) {
			std::cout << "::ERROR: Could not initialize GLFW" << "\n";
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		s_Window = glfwCreateWindow(width, height, windowName, NULL, NULL);
		if (!s_Window) {
			std::cout << "::ERROR: Could not initialize window" << "\n";
		}

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

			auto mouseMovedEvent = MouseMovedEvent(mouseX, mouseY, mouseDelta);
			runtime::handleEvent(mouseMovedEvent);
		});

		glfwSetMouseButtonCallback(s_Window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				auto mousePressedEvent = MouseButtonPressedEvent(button);
				runtime::handleEvent(mousePressedEvent);
				break;
			}
			case GLFW_RELEASE: {
				auto mouseReleasedEvent = MouseButtonReleasedEvent(button);
				runtime::handleEvent(mouseReleasedEvent);
				break;
			}
			default:
				break;
			}
		});

		glfwSetKeyCallback(s_Window, [](GLFWwindow *window, int key, int scancode, int action, int mod) {
			WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				auto keyPressedEvent = KeyPressedEvent(key);
				runtime::handleEvent(keyPressedEvent);
				break;
			}
			case GLFW_RELEASE:
				break;
			case GLFW_REPEAT: {
				auto keyPressedEvent = KeyPressedEvent(key);
				runtime::handleEvent(keyPressedEvent);
				break;
			}
			default:
				break;
			}
		});

		if (bool bGladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); !bGladSuccess) {
			std::cout << "::ERROR: Failed to load GL" << "\n";	
		}

		printf("%s\n", glGetString(GL_VERSION));
	}

	void swapBuffers() {
		glfwPollEvents();
		glfwSwapBuffers(s_Window);
	}

	bool isWindowOpen() {
		if (!s_Window) return false;
		return !glfwWindowShouldClose(s_Window);
	}

	GLFWwindow *getWindow() {
		return s_Window;
	}

	glm::vec2 getWindowSize() {
		return {s_State.Width, s_State.Height};
	}

	int getWindowWidth() {
		return s_State.Width;
	}

	int getWindowHeight() {
		return s_State.Height;
	}

}
