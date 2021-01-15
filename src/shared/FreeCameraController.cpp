#include "FreeCameraController.h"
#include "Window.h"

#define ROTATION_SPEED 6.f

FreeCameraController::FreeCameraController(Camera &camera, float speed) : m_camera(&camera), m_moveSpeed(speed) {}

void FreeCameraController::HandleKeyboardInput() {
	glm::vec3 moveDirection(0, 0, 0);

	auto window = Window::Get()->GetNativeWindow();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		moveDirection = m_camera->GetForward();
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		moveDirection = -m_camera->GetForward();
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		moveDirection = -m_camera->GetRight();
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		moveDirection = m_camera->GetRight();
	} else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		moveDirection = m_camera->GetUp();
	} else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		moveDirection = -m_camera->GetUp();
	}

	m_camera->AddPosition(moveDirection * m_moveSpeed * Window::Get()->DeltaTime());
}

void FreeCameraController::HandleEvent(Event &evt) {
	if (MouseMovedEvent *mouseEvt = dynamic_cast<MouseMovedEvent *>(&evt)) {

		if (m_moving) {
			// x, y are inverted for rotation
			glm::vec3 rotationDelta = glm::vec3(mouseEvt->MouseDelta().y, mouseEvt->MouseDelta().x, 0.f);
			m_camera->AddRotation(rotationDelta * ROTATION_SPEED * Window::Get()->DeltaTime());
		}

		evt.m_handled = true;
	}
	else if (MouseButtonPressedEvent *pressedEvt = dynamic_cast<MouseButtonPressedEvent *>(&evt)) {
		if (pressedEvt->GetMouseButton() == GLFW_MOUSE_BUTTON_2) {
			m_moving = true;
			evt.m_handled = true;
		}
	}
	else if (MouseButtonReleasedEvent *releasedEvt = dynamic_cast<MouseButtonReleasedEvent *>(&evt)) {
		if (releasedEvt->GetMouseButton() == GLFW_MOUSE_BUTTON_2) {
			m_moving = false;
			evt.m_handled = true;
		}
	}
}