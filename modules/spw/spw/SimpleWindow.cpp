#include "SimpleWindow.h"

#include "Event.h"

#include <cassert>
#include <GLFW/glfw3.h>

namespace spw {

	struct WindowState {
		const char *Title;
		int Width, Height;

		spw::Vec2i LastMousePos;
		bool MouseFirstMove = true;
	};

	static WindowState s_State;
	static GLFWwindow *s_Window = nullptr;
	static OnEventFn s_EventListener = nullptr;

	static void broadcastEvent(const Event& event);

	void createWindow(const char *windowName, int width, int height) {
		if (s_Window) return;

		s_State = WindowState();
		s_State.Title = windowName;
		s_State.Width = width;
		s_State.Height = height;

		if (!glfwInit()) {
			assert(false && "::ERROR: Could not initialize GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		s_Window = glfwCreateWindow(width, height, windowName, NULL, NULL);
		if (!s_Window) {
			assert(false && "::ERROR: Could not initialize window");
		}

		glfwMakeContextCurrent(s_Window);

		glfwSetErrorCallback([](int error, const char *description) {
			printf("::ERROR::GLFW:: %s\n", description);
		});

		glfwSetWindowUserPointer(s_Window, &s_State);
		glfwSetWindowSizeCallback(s_Window, [](GLFWwindow *window, int width, int height) {
			WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);
			state.Width = width;
			state.Height = height;
		});

		glfwSetCursorPosCallback(s_Window, [](GLFWwindow *window, double mouseX, double mouseY) {
			WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

			spw::Vec2i mousePos = spw::Vec2i(mouseX, mouseY);
			if (state.MouseFirstMove) {
				state.LastMousePos = mousePos;
				state.MouseFirstMove = false;
			}

			spw::Vec2i mouseDelta {
			    mousePos.X - state.LastMousePos.Y,
				state.LastMousePos.X - mousePos.Y
			};
			state.LastMousePos = mousePos;

			const MouseMovedEvent mouseMovedEvent { mouseX, mouseY, mouseDelta };
			broadcastEvent(mouseMovedEvent);
		});

		glfwSetMouseButtonCallback(s_Window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowState &state = *(WindowState *)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				const MouseButtonPressedEvent mousePressedEvent { button };
				broadcastEvent(mousePressedEvent);
				break;
			}
			case GLFW_RELEASE: {
				const MouseButtonReleasedEvent mouseReleasedEvent { button };
				broadcastEvent(mouseReleasedEvent);
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
				const KeyPressedEvent keyPressedEvent { key };
				broadcastEvent(keyPressedEvent);
				break;
			}
			case GLFW_RELEASE:
				break;
			case GLFW_REPEAT: {
				const KeyPressedEvent keyPressedEvent { key };
				broadcastEvent(keyPressedEvent);
				break;
			}
			default:
				break;
			}
		});
	}

	void destroyWindow() {
		glfwTerminate();
		glfwDestroyWindow(s_Window);
		s_Window = nullptr;
	}

	void requestCloseWindow() {
		glfwSetWindowShouldClose(s_Window, true);
	}

	void registerEventListener(OnEventFn eventListener) {
		s_EventListener = eventListener;
	}

	void broadcastEvent(const Event& evt) {
		if (s_EventListener) {
			s_EventListener(evt);
		}
	}

	bool isKeyPressed(int key) {
		return glfwGetKey(s_Window, key);
	}

	void pollEvents() {
		glfwPollEvents();
	}

	void swapBuffers() {
		glfwSwapBuffers(s_Window);
	}

	float getTime() {
		return (float) glfwGetTime();
	}

	void* getWindowImpl() {
		return s_Window;
	}

	spw::Vec2i getWindowSize() {
		return { s_State.Width, s_State.Height };
	}

	int getWindowWidth() {
		return s_State.Width;
	}

	int getWindowHeight() {
		return s_State.Height;
	}

	spw::Vec2i getMousePos() {
		return s_State.LastMousePos;
	}

}
